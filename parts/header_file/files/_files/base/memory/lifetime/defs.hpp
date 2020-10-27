//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace lifetime_n{
	class copy_t;

	class move_t;
	//

	template<class T>
	class construct_t;
	template<class T,class...Args>
	constexpr bool construct_able=::std::is_constructible_v<T,Args...>;
	template<class T,class...Args>
	constexpr bool construct_nothrow=::std::is_nothrow_constructible_v<T,Args...>;
	template<class T,class...Args>
	constexpr bool construct_trivial=::std::is_trivially_constructible_v<T,Args...>;

	class destruct_t;
	template<class T>
	constexpr bool destruct_able=::std::is_destructible_v<T>;
	template<class T>
	constexpr bool destruct_nothrow=::std::is_nothrow_destructible_v<T>;
	template<class T>
	constexpr bool destruct_trivial=::std::is_trivially_destructible_v<T>;
	//

	class copy_construct_t;
	template<class T>
	constexpr bool copy_construct_able=::std::is_copy_constructible_v<T>;
	template<class T>
	constexpr bool copy_construct_nothrow=::std::is_nothrow_copy_constructible_v<T>;
	template<class T>
	constexpr bool copy_construct_trivial=::std::is_trivially_copy_constructible_v<T>;

	class move_construct_t;
	template<class T>
	constexpr bool move_construct_able=::std::is_move_constructible_v<T>;
	template<class T>
	constexpr bool move_construct_nothrow=::std::is_nothrow_move_constructible_v<T>;
	template<class T>
	constexpr bool move_construct_trivial=::std::is_trivially_move_constructible_v<T>;
	//

	class copy_assign_t;
	template<class T>
	constexpr bool copy_assign_able=::std::is_copy_assignable_v<T>;
	template<class T>
	constexpr bool copy_assign_nothrow=::std::is_nothrow_copy_assignable_v<T>;
	template<class T>
	constexpr bool copy_assign_trivial=::std::is_trivially_copy_assignable_v<T>;

	class move_assign_t;
	template<class T>
	constexpr bool move_assign_able=::std::is_move_assignable_v<T>;
	template<class T>
	constexpr bool move_assign_nothrow=::std::is_nothrow_move_assignable_v<T>;
	template<class T>
	constexpr bool move_assign_trivial=::std::is_trivially_move_assignable_v<T>;
	//

	struct base_construct_t{};
	template<typename T>
	struct construct_t:base_construct_t{
		typedef base_construct_t base_t;
		template<class...Args>
		static constexpr bool able=construct_able<T,Args...>;
		template<class...Args>
		static constexpr bool nothrow=construct_nothrow<T,Args...>;
		template<class...Args>
		static constexpr bool trivial=construct_trivial<T,Args...>;

		template<class...Args,enable_if(able<Args...>)>
		[[nodiscard]]T operator()(Args&&...rest)const noexcept(nothrow<Args...>){
			return T(forward<Args>(rest)...);
		}
		struct array_construct_t{
			T*_to;
			size_t _size;
			template<class...Args,enable_if(able<Args...>)>
			T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t construct an array for never_in_array type.");
				auto tmp=_size;
				while(tmp--)new(&_to[tmp])T(forward<Args>(rest)...);
				return _to;
			}
		};
		struct placement_construct_t{
			T*_to;
			template<class...Args,enable_if(able<Args...>)>
			T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				new(_to)T(forward<Args>(rest)...);
				return _to;
			}
			[[nodiscard]]constexpr array_construct_t operator[](size_t size)const noexcept{return{_to,size};}
		};
		[[nodiscard]]constexpr placement_construct_t operator[](T*p)const noexcept{return{p};}
	};
	template<typename T>
	constexpr construct_t<T>construct{};

	constexpr struct destruct_t{
		template<class T>
		static constexpr bool able=destruct_able<T>||(::std::is_array_v<T>&&able<::std::remove_extent_t<T>>);
		template<class T>
		static constexpr bool nothrow=destruct_nothrow<T>||(::std::is_array_v<T>&&nothrow<::std::remove_extent_t<T>>);
		template<class T>
		static constexpr bool trivial=destruct_trivial<T>||(::std::is_array_v<T>&&trivial<::std::remove_extent_t<T>>);

		template<class T,enable_if(able<T>)>
		void base_call(T*to)const noexcept(nothrow<T>){
			if constexpr(!trivial<T>)
				if constexpr(::std::is_array_v<T>)
					for(auto&i : *to)
						base_call(addressof(i));
				else to->~T();
		}

		template<class T,enable_if(able<T>)>
		void base_call([[maybe_unused]]T*begin,[[maybe_unused]]size_t size)const noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			if constexpr(!trivial<T>)
				while(size--)base_call(begin+size);
		}

		template<class T,enable_if(able<T>)>
		void operator()(T*begin)const noexcept(nothrow<T>){
			base_call(begin);
		}

		struct array_destruct_t{
			size_t _size;
			template<class T,enable_if(able<T>)>
			void operator()(T*begin)const noexcept(nothrow<T>){
				base_call(begin,_size);
			}
		};

		[[nodiscard]]constexpr array_destruct_t operator[](size_t size)const noexcept{return{size};}

		struct not_t{};
		/*适用于unget(this,not destruct);*/
		not_t operator!()const noexcept{return not_t{};}
	}destruct{};

	constexpr struct re_construct_t{
		template<class T,class...Args>
		static constexpr bool able=destruct.able<T>&&construct<T>.able<Args...>;
		template<class T,class...Args>
		static constexpr bool nothrow=destruct.nothrow<T>&&construct<T>.nothrow<Args...>;
		template<class T,class...Args>
		static constexpr bool trivial=destruct.trivial<T>&&construct<T>.trivial<Args...>;

		template<class T,enable_if(able<T>)>
		T* operator()(T*to)const noexcept(nothrow<T>){
			destruct(to);
			construct<T>[to]();
			return to;
		}
		template<class T>
		struct array_re_construct_t{
			T*_to;
			size_t _size;
			template<class...Args,enable_if(able<T,Args...>)>
			T* operator()(Args&&...rest)const noexcept(nothrow<T,Args...>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You cannot perform array operations on never_in_array type.");
				destruct[size](begin);
				construct<T>[begin][size](forward<Args>(rest)...);
				return _to;
			}
		};
		template<class T>
		struct placement_re_construct_t{
			T*_to;
			template<class...Args,enable_if(able<T,Args...>)>
			T* operator()(Args&&...rest)const noexcept(nothrow<T,Args...>){
				destruct(to);
				construct<T>[to](forward<Args>(rest)...);
				return _to;
			}
			[[nodiscard]]constexpr array_re_construct_t<T> operator[](size_t size)const noexcept{return{_to,size};}
		};
		template<class T>
		[[nodiscard]]constexpr placement_re_construct_t<T> operator[](T*p)const noexcept{return{p};}
	}re_construct;

	constexpr struct copy_assign_t{
		template<class T>
		static constexpr bool r_able=copy_assign_able<T>?
									 copy_assign_trivial<T>||!(copy_construct_trivial<T>&&destruct.trivial<T>)
									 :false;//trivial操作优先考虑
		template<class T>
		static constexpr bool able=r_able<T>?true:(copy_construct_able<T>&&destruct.able<T>);
		template<class T>
		static constexpr bool nothrow=r_able<T>?copy_assign_nothrow<T>:
											(copy_construct_nothrow<T>&&destruct.nothrow<T>);
		template<class T>
		static constexpr bool trivial=r_able<T>?copy_assign_trivial<T>:
											(copy_construct_trivial<T>&&destruct.trivial<T>);

		template<class T,enable_if(able<T>)>
		T& operator()(T&a,const T&b)const noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				a=b;
			else{
				destruct(&a);
				construct<T>[&a](b);
			}
			return a;
		}
	}copy_assign{};

	constexpr struct move_assign_t{
		template<class T>
		static constexpr bool r_able=move_assign_able<T>;
		template<class T>
		static constexpr bool able=r_able<T>?true:copy_assign.able<T>;
		template<class T>
		static constexpr bool nothrow=r_able<T>?move_assign_nothrow<T>:copy_assign.nothrow<T>;
		template<class T>
		static constexpr bool trivial=r_able<T>?move_assign_trivial<T>:copy_assign.trivial<T>;

		template<class T,enable_if(able<T>)>
		T& operator()(T&a,T&&b)const noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				a=b;
			else{
				copy_assign(a,b);
			}
			return a;
		}
	}move_assign{};

	constexpr struct copy_construct_t{
		template<class T>
		static constexpr bool r_able=copy_construct_able<T>?
									 copy_construct_trivial<T>||!(construct<T>.trivial<>&&copy_assign_trivial<T>)
									 :false;//trivial操作优先考虑
		template<class T>
		static constexpr bool able=r_able<T>?true:(construct<T>.able<>&&copy_assign_able<T>);
		template<class T>
		static constexpr bool nothrow=r_able<T>?copy_construct_nothrow<T>:
										(construct<T>.nothrow<>&&copy_assign_nothrow<T>);
		template<class T>
		static constexpr bool trivial=r_able<T>?copy_construct_trivial<T>:
										(construct<T>.trivial<>&&copy_assign_trivial<T>);

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,const T*from)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,from,sizeof(T)));
			else{
				if constexpr(r_able<T>)
					new(to)T(from);
				else{
					construct<T>[to]();
					*to=*from;
				}
				return to;
			}
		}

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,const T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,from,sizeof(T)*size));
			else{
				while(size--)
					base_call(to+size,from+size)
				return to;
			}
		}

		template<class T,enable_if(able<T>)>
		T*operator()(T*to,const T*from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		template<class T,enable_if(able<T>)>
		T*operator()(note::to_t<T*>to,note::from_t<const T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T,enable_if(able<T>)>
		T*operator()(note::from_t<const T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,const T&from)noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				construct<T>[to](from);
			else{
				construct<T>[to]();
				*to=from;
			}
			return to;
		}

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,const T&from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			while(size--)
				base_call(to+size,from);
			return to;
		}

		template<class T,enable_if(able<T>)>
		T*operator()(T*to,const T&from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		struct array_copy_construct_t{
			size_t _size;
			template<class T,enable_if(able<T>)>
			T*operator()(T*to,const T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T,enable_if(able<T>)>
			T*operator()(T*to,const T&from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::to_t<T*>to,note::from_t<const T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::from_t<const T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]array_copy_construct_t operator[](size_t a)const noexcept{return{a};}
	}copy_construct{};

	constexpr struct move_construct_t{
		template<class T>
		static constexpr bool r_able=move_construct_able<T>?
									 move_construct_trivial<T>||!copy_construct.trivial<T>
									 :false;//trivial操作优先考虑
		template<class T>
		static constexpr bool able=r_able<T>?true:copy_construct.able<T>;
		template<class T>
		static constexpr bool nothrow=r_able<T>?move_construct_nothrow<T>:
												copy_construct.nothrow<T>;
		template<class T>
		static constexpr bool trivial=r_able<T>?move_construct_trivial<T>:
												copy_construct.trivial<T>;

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,T*from)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,from,sizeof(T)));
			else{
				if constexpr(r_able<T>){
					construct<T>[to](::std::move(*from));
					return to;
				}else
					return copy_construct(to,from);
			}
		}
		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,from,sizeof(T)*size));
			else{
				if constexpr(r_able<T>){
				    while(size--)
				    	construct<T>[to+size](::std::move(from+size));
					return to;
				}else
					return copy_construct[size](to,from);
			}
		}

		template<class T,enable_if(able<T>)>
		T*operator()(T*to,T*from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		template<class T,enable_if(able<T>)>
		T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T,enable_if(able<T>)>
		T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		struct array_move_construct_t{
			size_t _size;
			template<class T,enable_if(able<T>)>
			T*operator()(T*to,T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]array_move_construct_t operator[](size_t a)const noexcept{return{a};}
	}move_construct{};

	constexpr struct move_t{
		template<class T>
		static constexpr bool able=move_construct.able<T>&&destruct.able<T>;
		template<class T>
		static constexpr bool nothrow=move_construct.nothrow<T>&&destruct.nothrow<T>;
		template<class T>
		static constexpr bool trivial=move_construct.trivial<T>&&destruct.trivial<T>;

		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,T*from)noexcept(nothrow<T>){
			move_construct(to,from);
			destruct(from);
			return to;
		}
		template<class T,enable_if(able<T>)>
		static T*base_call(T*to,T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			move_construct[size](to,from);
			destruct[size](from);
			return to;
		}

		template<class T,enable_if(able<T>)>
		T*operator()(T*to,T*from)const noexcept(nothrow<T>)
		{return base_call(to,from,size);}

		template<class T,enable_if(able<T>)>
		T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from(),size);}

		template<class T,enable_if(able<T>)>
		T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from(),size);}

		struct array_move_t{
			size_t _size;
			template<class T,enable_if(able<T>)>
			T*operator()(T*to,T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T,enable_if(able<T>)>
			T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]array_move_t operator[](size_t a)const noexcept{return{a};}

		//特殊使用
		template<class T>
		[[nodiscard]]constexpr ::std::remove_reference_t<T>&& operator()(T&&a)const noexcept{
			return static_cast<::std::remove_reference_t<T>&&>(a);
		}
	}move{};

	constexpr struct copy_t{
		//特殊使用
		template<class T,enable_if(copy_construct_able<T>?true:(construct<T>.able<>&&copy_assign_able<T>))>
		[[nodiscard]]constexpr T operator()(const T&a)const noexcept{
			if constexpr(copy_construct_able<T>)
				return construct<T>(a);
			else
				return construct<T>()=a;
		}
	}copy{};
}

//file_end

