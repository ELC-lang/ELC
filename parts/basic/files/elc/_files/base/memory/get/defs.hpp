//defs.hpp
//at namespace elc::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace get_n{
	/*
	有想过增加overhead以避免resize时还需要提供原始size的情况，并增加getsize方法用来避免size的多处存储
	最后因为不一致性（单个分配与多个分配）放弃了
	
	可能的话,应当避免细节暴露才是.
	若是get更常用于大规模分配,添加头也就无可厚非了不是吗?
	
	可是你不能保证get以后一定多用于大规模分配啊？
	
	单个分配时malloc都会有头,多一个size_t也无妨吧.
	
	。。
	好像也是
	
	干脆移入alloc模块吧.
	
	那alloc_by_pool返回的东西该带头么
	
	?
	
	。
	
	....
	
	。。
	
	理应该带....
	
	。。。
	
	!
	
	？
	
	允许自定义get_size就bene了.
	
	哦哦
	*/
	struct build_by_get_only{};

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
		static void base_call(T*&arg,size_t to_size)noexcept(nothrow<T>){
			size_t from_size=get_size_of_alloc(arg);
			if(to_size)
				if(from_size > to_size){
					destruct(arg+to_size-1,from_size-to_size);
					realloc(arg,to_size);
				}else if(from_size){
					if constexpr(move.trivial<T>)
						realloc(arg,to_size);
					else{
						T*tmp=alloc<T>(to_size);
						move[from_size](note::from(arg),note::to(tmp));
						free(arg);
						arg=tmp;
					}
					construct<T>[arg+from_size-1][to_size-from_size]();
				}else
					arg=get<T>[to_size]();
			else{
				unget(arg);
				arg=null_ptr;
			}
		}
		
		template<typename T,enable_if(able<T>)>
		inline void operator()(T*&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
		}
	}get_resize{};
}
