//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace abstract_base_n{
	/*
	ELC的虚基类模块，适用于unget
	处理虚基类到实例类的偏移与模板实例化差异
	原理：
		虚函数间接调用以从base_t&转为T&
		运行时计算偏移以从T*转换base_t*
	用法：is_common_attribute
	*/
	common_attribute_t abstract_base;
}
namespace get_n{
	using abstract_base_n::abstract_base;
	//struct build_by_get_only{};已定义于 "../../base_defs/special_attribute.hpp"

	/*减小数据块大小并转移原有实例的生命周期，但并不析构旧的实例*/
	template<typename T>
	void alloc_size_cut(T*arg,size_t to_size){
		realloc(arg,to_size);
	}
	/*扩大数据块大小并转移原有实例的生命周期，但并不构造新的实例*/
	template<typename T>
	void alloc_size_grow(T*&arg,size_t to_size){
		if constexpr(move.trivial<T>)
			realloc(arg,to_size);
		else{
			T*tmp=alloc<T>(to_size);
			auto from_size=get_size_of_alloc(arg);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[from_size](note::from(arg),note::to(tmp));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[from_size](note::from(arg),note::to(tmp));
			}
			free(arg);
			arg=tmp;
		}
	}

	struct base_get_t{};
	template<typename T>
	struct get_t:base_get_t{
		typedef base_get_t base_t;
		template<class...Args>
		static constexpr bool able=construct<T>.able<Args...>&&destruct.able<T>;
		template<class...Args>
		static constexpr bool nothrow=construct<T>.nothrow<Args...>;

		template<class...Args> requires able<Args...>
		[[nodiscard]]T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
			return construct<T>[alloc<T>()](forward<Args>(rest)...);
		}

		struct array_get_t{
			size_t _size;
			template<class...Args> requires able<Args...>
			[[nodiscard]]T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t get an array for never_in_array type.");
				return construct<T>[alloc<T>(_size)][_size](forward<Args>(rest)...);
			}
		};
		[[nodiscard]]constexpr array_get_t operator[](size_t size)const noexcept{return{size};}

		static constexpr struct as_array_t{
			template<typename U>
			static constexpr bool able=copy_construct.able<T>&&destruct.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=copy_construct.nothrow<T>;

			[[nodiscard]]T* operator()(array_like_view_t<const T>a)const noexcept(nothrow<void>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t get an array for never_in_array type.");
				auto size=a.size();
				auto aret=alloc<T>[size]();
				copy_construct[size](note::from(a.begin()),note::to(aret));
				return aret;
			}
		}as_array{};

		static constexpr struct apply_end_t{
			template<typename U>
			static constexpr bool able=copy_construct.able<T>&&move.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=copy_construct.nothrow<T>&&move.nothrow<T>;

			T* operator()(note::to_t<T*&> to,array_like_view_t<const T>a)const noexcept(nothrow<void>){
				auto&ptr=to.value;
				auto from_size=get_size_of_alloc(ptr);
				auto a_size=a.size();
				alloc_size_grow(ptr,from_size+a_size);
				copy_construct[a_size](note::from(a.begin()),note::to(ptr+from_size));
				return ptr;
			}
		}apply_end{};

		static constexpr struct remove_t{
			template<typename U>
			static constexpr bool able=destruct.able<T>&&move.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=destruct.nothrow<T>&&move.nothrow<T>;

			bool operator()(array_like_view_t<const T>a,note::from_t<T*>from)const noexcept(nothrow<void>){
				auto ptr=from.value;
				auto from_size=get_size_of_alloc(ptr);
				T*ptr_to_a=in_range(a,{ptr,note::size(from_size)});
				auto a_size=a.size();
				if(!ptr_to_a)
					return false;

				destruct[a_size](ptr_to_a);
				move[(ptr+from_size)-(ptr_to_a+a_size)](note::from(ptr_to_a+a_size),note::to(ptr_to_a));
				alloc_size_cut(ptr,from_size-a_size);
				return true;
			}
		}remove{};
	};
	template<typename T>
	constexpr get_t<T>get{};


	constexpr struct unget_t{
		template<typename T>
		static constexpr bool able=destruct.able<T>;
		template<typename T>
		static constexpr bool nothrow=type_info<T>.not_has_attribute(abstract_base)&&destruct.nothrow<T>;

		template<typename T> requires able<T>
		void operator()(T*a)const noexcept(nothrow<T>){
			if(a!=null_ptr){
				if constexpr(type_info<T>.has_attribute(abstract_base))
					attribute_ptr_cast<abstract_base>(a)->abstract_method_unget_this();
				else{
					if constexpr(!destruct.nothrow<T>)
						template_warning("the destructer of T was not noexcept,this may cause memory lack.");
					destruct[get_size_of_alloc(a)](a);
					free(a);
				}
			}
		}
		/*适用于unget(this,not destruct);*/
		template<typename T> requires able<T>
		void operator()(T*a,decltype(destruct)::not_t)const noexcept(nothrow<T>){
			free(a);
		}
	}unget{};

	constexpr struct get_resize_t{
		template<typename T>
		static constexpr bool able=construct<T>.able<>&&destruct.able<T>&&move.able<T>;
		template<typename T>
		static constexpr bool nothrow=type_info<T>.not_has_attribute(abstract_base)&&construct<T>.nothrow<>&&destruct.nothrow<T>&&move.nothrow<T>;

		template<typename T> requires able<T>
		static void base_call(T*&arg,const size_t to_size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array)){
				template_warning("For never_in_array type,get_resize will unget ptr when new_size=0 else do nothing.");
				if(to_size)
					return;
				unget(arg);
				arg=null_ptr;
			}elseif constexpr(type_info<T>.has_attribute(abstract_base)){
				arg=attribute_ptr_cast<abstract_base>(arg)->abstract_method_get_resize_this(to_size);
			}else{
				const size_t from_size=get_size_of_alloc(arg);
				if(from_size==to_size)
					return;
				elseif(from_size > to_size){
					destruct[from_size-to_size](arg+to_size-1);
					alloc_size_cut(arg,to_size);
				}elseif(from_size){
					alloc_size_grow(arg,to_size);
					construct<T>[arg+from_size-1][to_size-from_size]();
				}else
					arg=get<T>[to_size]();
			}
		}

		template<typename T> requires able<T>
		inline void operator()(T*&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
		}
		template<typename T> requires able<T>
		[[nodiscard]]inline T* operator()(T*&&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
			return arg;
		}
	}get_resize{};

	constexpr struct get_size_of_get_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=noexcept(get_size_of_alloc(declvalue(const T*)));

		template<typename T> requires able<T>
		static size_t base_call(const T*arg)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(abstract_base))
				return attribute_ptr_cast<abstract_base>(arg)->abstract_method_get_size_of_get_for_this();
			else
				return copy_construct(note::from(arg),note::to(copy_alloc(arg)),get_size_of_get(arg));
		}

		template<typename T> requires able<T>
		size_t operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}get_size_of_get{};

	constexpr struct copy_get_t{
		template<typename T>
		static constexpr bool able=copy_construct.able<T>;
		template<typename T>
		static constexpr bool nothrow=copy_construct.nothrow<T>;

		template<typename T> requires able<T>
		static T*base_call(const T*arg)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(abstract_base))
				return attribute_ptr_cast<abstract_base>(arg)->abstract_method_copy_get_this();
			else
				return copy_construct(note::from(arg),note::to(copy_alloc(arg)),get_size_of_get(arg));
		}

		template<typename T> requires able<T>
		T*operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}copy_get{};
}

//file_end

