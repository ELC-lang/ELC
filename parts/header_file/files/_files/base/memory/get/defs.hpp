//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace get_n{
	//struct build_by_get_only{};已定义于 "../../base_defs/special_attribute.hpp"

	struct base_get_t{};
	template<typename T>
	struct get_t:base_get_t{
		typedef base_get_t base_t;
		template<class...Args>
		static constexpr bool able=construct<T>.able<Args...>&&destruct.able<T>;
		template<class...Args>
		static constexpr bool nothrow=construct<T>.nothrow<Args...>;

		template<class...Args,enable_if(able<Args...>)>
		T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
			return construct<T>[alloc<T>()](forward<Args>(rest)...);
		}
		struct array_get_t{
			size_t _size;
			template<class...Args,enable_if(able<Args...>)>
			T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t get an array for never_in_array type.");
				return construct<T>[alloc<T>(_size)][_size](forward<Args>(rest)...);
			}
		};
		[[nodiscard]]constexpr array_get_t operator[](size_t size)const noexcept{return{size};}
	};
	template<typename T>
	constexpr get_t<T>get{};

	constexpr struct unget_t{
		template<typename T>
		static constexpr bool able=destruct.able<T>;
		template<typename T>
		static constexpr bool nothrow=destruct.nothrow<T>;

		template<typename T,enable_if(able<T>)>
		void operator()(T*a)const noexcept(nothrow<T>){
			if(a!=null_ptr){
				if constexpr(!destruct.nothrow<T>)
					template_warning("the destructer of T was not noexcept,this may cause memory lack.");
				destruct(a,get_size_of_alloc(a));
				free(a);
			}
		}
	}unget{};

	constexpr struct get_resize_t{
		template<typename T>
		static constexpr bool able=construct<T>.able<>&&destruct.able<T>&&move.able<T>;
		template<typename T>
		static constexpr bool nothrow=construct<T>.nothrow<>&&destruct.nothrow<T>&&move.nothrow<T>;

		template<typename T,enable_if(able<T>)>
		static void base_call(T*&arg,const size_t to_size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array)){
				template_warning("For never_in_array type,get_resize will unget ptr when new_size=0 else do nothing.");
				if(to_size)
					return;
				unget(arg);
				arg=null_ptr;
			}else{
				const size_t from_size=get_size_of_alloc(arg);
				if(from_size==to_size)
					return;
				elseif(from_size > to_size){
					destruct(arg+to_size-1,from_size-to_size);
					realloc(arg,to_size);
				}elseif(from_size){
					if constexpr(move.trivial<T>)
						realloc(arg,to_size);
					else{
						if constexpr(!move.nothrow<T>)
							template_warning("the move of T was not noexcept,this may cause memory lack.");
						T*tmp=alloc<T>(to_size);
						move[from_size](note::from(arg),note::to(tmp));
						free(arg);
						arg=tmp;
					}
					construct<T>[arg+from_size-1][to_size-from_size]();
				}else
					arg=get<T>[to_size]();
			}
		}

		template<typename T,enable_if(able<T>)>
		inline void operator()(T*&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
		}
	}get_resize{};

	template<class T>
	inline size_t get_size_of_get(const T*arg)noexcept_as(get_size_of_alloc(declvalue(const T*))){
		return get_size_of_alloc(arg);
	}

	constexpr struct copy_get_t{
		template<typename T>
		static constexpr bool able=copy_construct.able<T>;
		template<typename T>
		static constexpr bool nothrow=copy_construct.nothrow<T>;

		template<typename T,enable_if(able<T>)>
		static T*base_call(const T*arg)noexcept(nothrow<T>){
			return copy_construct(note::from(arg),note::to(copy_alloc(arg)),get_size_of_get(arg));
		}

		template<typename T,enable_if(able<T>)>
		T*operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}copy_get{};
}

//file_end

