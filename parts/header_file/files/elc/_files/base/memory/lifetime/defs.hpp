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

	template<class T> requires(::std::is_trivially_copyable_v<T>)
	force_inline T* super_speed_trivial_copy_from_one(T*to,const T&value,size_t size)noexcept{
		if constexpr(sizeof(T)==sizeof(unsigned char))
			::std::memset((unsigned char*)to,(unsigned char)value,size);
		elseif constexpr(sizeof(T)==sizeof(wchar_t))
			::std::wmemset((wchar_t*)to,(wchar_t)value,size);
		else
			if(is_all_byte_zero(value))
				::std::memset(to,zero,size*sizeof(T));
			else
				::std::fill_n(to,size,value);
		return to;
	}
	template<class T> requires(::std::is_trivially_copyable_v<T>)
	force_inline T* super_speed_trivial_copy_from_one(T*to,const T&value)noexcept{
		if constexpr(sizeof(T)>=sizeof(::std::max_align_t))
			if(is_all_byte_zero(value))
				::std::memset(to,zero,sizeof(T));
			else
				*to=value;
		else
			*to=value;
		return to;
	}

	/*!
	所有的construct_t实例都派生于此
	*/
	struct base_construct_t{};
	/*!
	用于构造实例
	用法详见变量模板construct
	*/
	template<typename T>
	struct construct_t:base_construct_t{
		typedef base_construct_t base_t;
		template<class...Args>
		static constexpr bool able=construct_able<T,Args...>;
		template<class...Args>
		static constexpr bool nothrow=construct_nothrow<T,Args...>;
		template<class...Args>
		static constexpr bool trivial=construct_trivial<T,Args...>;

		template<class...Args> requires able<Args...>
		[[nodiscard]]force_inline T operator()(Args&&...rest)const noexcept(nothrow<Args...>){
			return T(forward<Args>(rest)...);
		}
		struct array_construct_t{
			T*_to;
			size_t _size;
			constexpr void never_in_array_check()const noexcept{
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t construct an array for never_in_array type.");
			}
			template<class...Args> requires able<Args...>
			force_inline void base_call(Args&&...rest)const noexcept(nothrow<Args...>){
				auto tmp=_size;
				while(tmp--)new(_to+tmp)T(forward<Args>(rest)...);
			}
			template<class...Args> requires able<Args...>
			force_inline T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				never_in_array_check();
				base_call(forward<Args>(rest)...);
				return _to;
			}
			//复制构造速度优化
			force_inline T* operator()(const T&v)const noexcept(nothrow<const T&>)requires able<const T&>{
				never_in_array_check();
				if constexpr(trivial<const T&> && ::std::is_trivially_copyable_v<T>)
					super_speed_trivial_copy_from_one(_to,v,_size);
				else
					base_call(forward<const T&>(v));
				return _to;
			}
			//默认构造逻辑优化
			force_inline T* operator()()const noexcept(nothrow<>)requires able<>{
				never_in_array_check();
				if constexpr(!trivial<>)
					base_call();
				return _to;
			}
		};
		struct placement_construct_t{
			T*_to;
			template<class...Args> requires able<Args...>
			force_inline void base_call(Args&&...rest)const noexcept(nothrow<Args...>){
				new(_to)T(forward<Args>(rest)...);
			}
			template<class...Args> requires able<Args...>
			force_inline T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				base_call(forward<Args>(rest)...);
				return _to;
			}
			//复制构造速度优化
			force_inline T* operator()(const T&v)const noexcept(nothrow<const T&>)requires able<const T&>{
				if constexpr(trivial<const T&> && ::std::is_trivially_copyable_v<T>)
					super_speed_trivial_copy_from_one(_to,v);
				else
					base_call(forward<const T&>(v));
				return _to;
			}
			//默认构造逻辑优化
			force_inline T* operator()()const noexcept(nothrow<>)requires able<>{
				if constexpr(!trivial<>)
					base_call();
				return _to;
			}
			[[nodiscard]]force_inline constexpr array_construct_t operator[](size_t size)const noexcept{return{_to,size};}
		};
		[[nodiscard]]force_inline constexpr placement_construct_t operator[](T*p)const noexcept{return{p};}
	};
	/*!
	constexpr变量模板，用于构造实例
	用法：
	construct<类型>.able<构造参数类型>		->	bool
	construct<类型>.nothrow<构造参数类型>	->	bool
	construct<T>(参数)						->	T		（以参数构建T类型实例）

	construct<T>[T*ptr](参数)				->	T*		（以参数在ptr地址处构建T类型实例，返回ptr）
	construct<T>[T*ptr][size_t size](参数)	->	T*		（以参数在ptr地址处构建size个T类型实例，返回ptr）
	*/
	template<typename T>
	constexpr construct_t<T>construct{};

	/*!
	用于销毁实例
	用法：
	destruct.able<类型>		->	bool
	destruct.nothrow<类型>	->	bool

	destruct(T*ptr)					-> void		（在ptr地址处析构T类型实例，T可为数组）
	destruct[size_t size](T*ptr)	-> void		（析构以ptr地址起始共size个T类型实例，T可为数组）
	*/
	constexpr struct destruct_t{
		template<class T>
		static constexpr bool able=destruct_able<T>;
		template<class T>
		static constexpr bool nothrow=destruct_nothrow<T>;
		template<class T>
		static constexpr bool trivial=destruct_trivial<T>;

		template<class T> requires able<T>
		static force_inline void base_call(T*to)noexcept(nothrow<T>){
			if constexpr(!trivial<T>)
				if constexpr(::std::is_array_v<T>)
					for(auto&i : *to)
						base_call(addressof(i));
				else
					push_and_disable_msvc_warning(26457)
					to->~T();
					pop_msvc_warning()
		}

		template<class T> requires able<T>
		static void base_call([[maybe_unused]]T*begin,[[maybe_unused]]size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			if constexpr(!trivial<T>)
				while(size--)base_call(begin+size);
		}

		template<class T> requires able<T>
		force_inline void operator()(T*begin)const noexcept(nothrow<T>){
			base_call(begin);
		}

		struct array_destruct_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline void operator()(T*begin)const noexcept(nothrow<T>){
				base_call(begin,_size);
			}
		};

		[[nodiscard]]force_inline constexpr array_destruct_t operator[](size_t size)const noexcept{return{size};}

		struct not_t{};
		/*适用于unget(this,not destruct);*/
		force_inline constexpr not_t operator!()const noexcept{return not_t{};}
	}destruct{};

	/*!
	用于重新构造实例：先摧毁，再原地构建
	用法：
	re_construct.able<类型>		->	bool
	re_construct.nothrow<类型>	->	bool

	re_construct(T*ptr)						->	T*		（以无参数在ptr地址处重新构建T类型实例，返回ptr）
	re_construct[T*ptr](参数)				->	T*		（以参数在ptr地址处重新构建T类型实例，返回ptr）
	re_construct[T*ptr][size_t size](参数)	->	T*		（以参数在ptr地址处重新构建size个T类型实例，返回ptr）
	常见用法：
	re_construct(this)
	*/
	constexpr struct re_construct_t{
		template<class T,class...Args>
		static constexpr bool able=destruct.able<T>&&construct<T>.able<Args...>;
		template<class T,class...Args>
		static constexpr bool nothrow=destruct.nothrow<T>&&construct<T>.nothrow<Args...>;
		template<class T,class...Args>
		static constexpr bool trivial=destruct.trivial<T>&&construct<T>.trivial<Args...>;

		template<class T> requires able<T>
		force_inline T* operator()(T*to)const noexcept(nothrow<T>){
			destruct(to);
			construct<T>[to]();
			return to;
		}
		template<class T>
		struct array_re_construct_t{
			T*_to;
			size_t _size;
			template<class...Args> requires able<T,Args...>
			T* operator()(Args&&...rest)const noexcept(nothrow<T,Args...>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You cannot perform array operations on never_in_array type.");
				destruct[_size](_to);
				construct<T>[_to][_size](forward<Args>(rest)...);
				return _to;
			}
		};
		template<class T>
		struct placement_re_construct_t{
			T*_to;
			template<class...Args> requires able<T,Args...>
			T* operator()(Args&&...rest)const noexcept(nothrow<T,Args...>){
				destruct(_to);
				construct<T>[_to](forward<Args>(rest)...);
				return _to;
			}
			[[nodiscard]]force_inline constexpr array_re_construct_t<T> operator[](size_t size)const noexcept{return{_to,size};}
		};
		template<class T>
		[[nodiscard]]force_inline constexpr placement_re_construct_t<T> operator[](T*p)const noexcept{return{p};}
	}re_construct;

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

		template<class T> requires able<T>
		static T*base_call(T*to,const T*from)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,add_const(from),sizeof(T)));
			else{
				if constexpr(r_able<T>)
					new(to)T(*from);
				else{
					construct<T>[to]();
					*to=*from;
				}
				return to;
			}
		}

		template<class T> requires able<T>
		static T*base_call(T*to,const T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,add_const(from),sizeof(T)*size));
			else{
				while(size--)
					base_call(to+size,from+size);
				return to;
			}
		}

		template<class T> requires able<T>
		force_inline T*operator()(T*to,const T*from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		template<class T> requires able<T>
		force_inline T*operator()(note::to_t<T*>to,note::from_t<const T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T> requires able<T>
		force_inline T*operator()(note::from_t<const T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from());}


		template<class T> requires able<T>
		force_inline T*operator()(T*to,const T*from,size_t size)const noexcept(nothrow<T>)
		{return base_call(to,from,size);}

		template<class T> requires able<T>
		force_inline T*operator()(note::to_t<T*>to,note::from_t<const T*>from,size_t size)const noexcept(nothrow<T>)
		{return base_call(to(),from(),size);}

		template<class T> requires able<T>
		force_inline T*operator()(note::from_t<const T*>from,note::to_t<T*>to,size_t size)const noexcept(nothrow<T>)
		{return base_call(to(),from(),size);}

		template<class T> requires able<T>
		static T*base_call(T*to,const T&from)noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				construct<T>[to](from);
			else{
				construct<T>[to]();
				*to=from;
			}
			return to;
		}

		template<class T> requires able<T>
		static T*base_call(T*to,const T&from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			if constexpr(::std::is_trivially_copyable_v<T>)
				super_speed_trivial_copy_from_one(to,from,size);
			else
				while(size--)
					base_call(to+size,from);
			return to;
		}

		template<class T> requires able<T>
		force_inline T*operator()(T*to,const T&from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		struct array_copy_construct_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline T*operator()(T*to,const T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(T*to,const T&from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,note::from_t<const T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::from_t<const T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]force_inline constexpr array_copy_construct_t operator[](size_t a)const noexcept{return{a};}
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

		template<class T> requires able<T>
		static T*base_call(T*to,T*from)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,add_const(from),sizeof(T)));
			else{
				if constexpr(r_able<T>){
					construct<T>[to](::std::move(*from));
					return to;
				}else
					return copy_construct(to,from);
			}
		}
		template<class T> requires able<T>
		static T*base_call(T*to,T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			if constexpr(trivial<T>)
				return reinterpret_cast<T*>(::std::memcpy(to,add_const(from),sizeof(T)*size));
			else{
				if constexpr(r_able<T>){
					while(size--)
						construct<T>[to+size](::std::move(from[size]));
					return to;
				}else
					return copy_construct[size](to,from);
			}
		}

		template<class T> requires able<T>
		force_inline T*operator()(T*to,T*from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		template<class T> requires able<T>
		force_inline T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T> requires able<T>
		force_inline T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		struct array_move_construct_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline T*operator()(T*to,T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]force_inline constexpr array_move_construct_t operator[](size_t a)const noexcept{return{a};}
	}move_construct{};

	constexpr struct move_t{
		template<class T>
		static constexpr bool able=move_construct.able<T>&&destruct.able<T>;
		template<class T>
		static constexpr bool nothrow=move_construct.nothrow<T>&&destruct.nothrow<T>;
		template<class T>
		static constexpr bool trivial=move_construct.trivial<T>&&destruct.trivial<T>;

		template<class T> requires able<T>
		static T*base_call(T*to,T*from)noexcept(nothrow<T>){
			move_construct(to,from);
			destruct(from);
			return to;
		}
		template<class T> requires able<T>
		static T*base_call(T*to,T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
			move_construct[size](to,from);
			destruct[size](from);
			return to;
		}

		template<class T> requires able<T>
		force_inline T*operator()(T*to,T*from)const noexcept(nothrow<T>)
		{return base_call(to,from);}

		template<class T> requires able<T>
		force_inline T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		template<class T> requires able<T>
		force_inline T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>)
		{return base_call(to(),from());}

		struct array_move_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline T*operator()(T*to,T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]force_inline constexpr array_move_t operator[](size_t a)const noexcept{return{a};}

		//特殊使用
		template<class T>
		[[nodiscard]]constexpr remove_ref<T>&& operator()(T&&a)const noexcept{
			return static_cast<remove_ref<T>&&>(a);
		}
	}move{};

	constexpr struct copy_t{
		//特殊使用
		//UF
		template<class T> requires(copy_construct.able<T>)
		[[nodiscard]]constexpr T operator()(const T&a)const noexcept{
			return copy_construct<T>(a);
		}
	}copy{};

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

		template<class T> requires able<T>
		static T& base_call(T&a,const T&b)noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				a=b;
			else{
				destruct(&a);
				construct<T>[&a](b);
			}
			return a;
		}
		template<class T> requires able<T>
		static T* base_call(T*to,const T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				::std::memcpy(to,add_const(from),size*sizeof(T));
			else{
				while(size--)
					base_call(to[size],from[size]);
			}
			return to;
		}
		template<class T> requires able<T>
		static T* base_call(T* to,const T& from,size_t size)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				super_speed_trivial_copy_from_one(to,from,size);
			else
				while(size--)
					base_call(to[size],from);
			return to;
		}

		template<class T> requires able<T>
		force_inline T& operator()(T&a,const T&b)const noexcept(nothrow<T>){
			return base_call(a,b);
		}

		struct array_copy_assign_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline T*operator()(T*to,const T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,note::from_t<const T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::from_t<const T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(const T&from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return base_call(to(),from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,const T&from)const noexcept(nothrow<T>){
				return base_call(to(),from,_size);
			}
		};
		[[nodiscard]]force_inline constexpr array_copy_assign_t operator[](size_t a)const noexcept{return{a};}
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

		template<class T> requires able<T>
		static T& base_call(T&a,T&&b)noexcept(nothrow<T>){
			if constexpr(r_able<T>)
				a=move(b);
			else{
				copy_assign(a,b);
			}
			return a;
		}
		template<class T> requires able<T>
		static T* base_call(T*to,T*from,size_t size)noexcept(nothrow<T>){
			if constexpr(trivial<T>)
				::std::memcpy(to,add_const(from),size*sizeof(T));
			else{
				while(size--)
					base_call(to[size],from[size]);
			}
			return to;
		}

		template<class T> requires able<T>
		force_inline T& operator()(T&a,T&b)const noexcept(nothrow<T>){
			return base_call(a,move(b));
		}

		struct array_move_assign_t{
			size_t _size;
			template<class T> requires able<T>
			force_inline T*operator()(T*to,T*from)const noexcept(nothrow<T>){
				return base_call(to,from,_size);
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::to_t<T*>to,note::from_t<T*>from)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
			template<class T> requires able<T>
			force_inline T*operator()(note::from_t<T*>from,note::to_t<T*>to)const noexcept(nothrow<T>){
				return operator()(to(),from());
			}
		};
		[[nodiscard]]force_inline constexpr array_move_assign_t operator[](size_t a)const noexcept{return{a};}
	}move_assign{};
}

//file_end

