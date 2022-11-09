//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/// @brief 提供gc定义
namespace gc_n{
	/// @brief 当alloc失败时调用此函数
	void gc_for_alloc()noexcept;
}
/// @brief 底层内存管理泛用类型的模板定义
namespace alloc_n{
	using ::elc::defs::memory::gc_n::gc_for_alloc;

	//允许自定义对象的alloc/free/realloc/get_size_of_alloc方法：为pool留的后门（大概）
	//这里是缺省时的默认方法定义
	#include "default_method/defs.hpp"

	enable_adl(the_alloc_method);
	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T))void*alloc_method(type_info_t<T>)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		if constexpr(was_not_an_ill_form(the_alloc_method(type_info<T>)))
			return the_alloc_method(type_info<T>);
		else
			return default_method::alloc_method(type_info<T>);
	}
	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T)*size)void*alloc_method(type_info_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		//size被保证不为0
		if constexpr(was_not_an_ill_form(the_alloc_method(type_info<T>,size)))
			return the_alloc_method(type_info<T>,size);
		else
			return default_method::alloc_method(type_info<T>,size);
	}
	enable_adl(the_get_size_of_alloc_method);
	template<typename T>
	inline size_t get_size_of_alloc_method(_in_param const T*arg)noexcept{
		//arg保证不与null_ptr相等
		if constexpr(was_not_an_ill_form(the_get_size_of_alloc_method(arg)))
			return the_get_size_of_alloc_method(arg);
		else
			return default_method::get_size_of_alloc_method(arg);
	}
	enable_adl(the_free_method);
	template<typename T>
	inline void free_method(_in_param T*arg)noexcept{
		if constexpr(was_not_an_ill_form(the_free_method(arg)))
			the_free_method(arg);
		else
			default_method::free_method(arg);
	}
	enable_adl(the_realloc_method);
	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T)*new_size)void*realloc_method(_in_param T*ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc_for_alloc，但ptr值必须保持有效以保证gc_for_alloc后再次realloc有效
		//new_size被保证不为0
		//align维持不变
		//但只允许在扩大数据块时可选的移动数据块
		if constexpr(was_not_an_ill_form(the_realloc_method(ptr,new_size)))
			return the_realloc_method(ptr,new_size);
		else
			return default_method::realloc_method(ptr,new_size);
	}

	/// @brief 所有alloc_t的基类
	/// @details 尽管意义不明，但这个类就是在这里
	struct base_alloc_t{};
	
	/// @brief 用于对特定类型进行alloc的类
	/// @warning 这个类不会对内存进行初始化，如需要进行new like操作，请使用 get_t
	template<class T>
	struct alloc_t:base_alloc_t{
		typedef base_alloc_t base_t;
		[[nodiscard]]static _return_nevernull_has_size(1)T*base_call()noexcept{
			void*tmp;
			while(!assign(tmp,alloc_method(type_info<T>)))gc_for_alloc();
			return reinterpret_cast<T*>(tmp);
		}
		[[nodiscard]]static _return_nevernull_has_size(size)T*base_call(size_t size)noexcept{
			const APIs::alloc::source_location_guard slg{1};
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You can\'t alloc an array for never_in_array type.");
			if(size){//null_ptr不一定等价于nullptr，请勿删除本行
				void*tmp;
				while(!assign(tmp,alloc_method(type_info<T>,size)))gc_for_alloc();
				return reinterpret_cast<T*>(tmp);
			}else return null_ptr;
		}
		[[nodiscard]]/*static*/force_inline _return_nevernull_has_size(1)T*operator()()const noexcept{return base_call();}
		[[nodiscard]]/*static*/force_inline _return_nevernull_has_size(size)T*operator()(size_t size)const noexcept{return base_call(size);}
		struct alloc_array_t{
			size_t _size;
			[[nodiscard]]force_inline _return_nevernull_has_size(_size)T*operator()()const noexcept{return base_call(_size);}
		};
		[[nodiscard]]force_inline constexpr alloc_array_t operator[](size_t a)const noexcept{return{a};}
	};
	/// @brief 用于对特定类型进行alloc的类实例
	/// @see alloc_t
	/// @warning 不会对内存进行初始化，如需要进行new like操作，请使用 get_t
	template<class T>
	constexpr alloc_t<T>alloc{};

	/// @brief 用于对特定类型进行free
	/// @warning 这个类不会对类实例进行析构，如需要进行new like操作，请使用 get_t & unget_t
	constexpr struct free_t{
		typedef free_t base_t;
		template<class T>
		static void base_call(_in_param T*p)noexcept{
			const APIs::alloc::source_location_guard slg{1};
			if(p!=null_ptr)//null_ptr不一定等价于nullptr，请勿删除本行
				free_method(p);
		}
		template<class T>
		/*static*/force_inline void operator()(_in_param T*p)const noexcept{base_call(p);}
	}free{};

	/// @brief 用于对特定类型进行realloc
	/// @warning 这个类不会对类实例进行移动函数调用，也不会创造或结束生命周期，如需要进行new like操作，请使用 get_resize_t
	constexpr struct realloc_t{
		typedef realloc_t base_t;
		template<class T>
		static void base_call(_inout_opt_param _out_param_with_writes(nsize)T*&ptr,size_t nsize)noexcept{
			const APIs::alloc::source_location_guard slg{1};
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
				//template_warning("For never_in_array type,realloc will free ptr when new_size=0 else do nothing.");
			if(nsize){//null_ptr不一定等价于nullptr，请勿删除本行
				if constexpr(type_info<T>.not_has_attribute(never_in_array))
					if(ptr!=null_ptr){//null_ptr不一定等价于nullptr，请勿删除本行
						void*tmp;
						while(!assign(tmp,realloc_method(ptr,nsize)))gc_for_alloc();
						ptr=reinterpret_cast<T*>(tmp);
					}else
						ptr=alloc<T>(nsize);
			}else{
				free(ptr);
				ptr=null_ptr;
			}
		}
		template<class T>
		/*static*/force_inline void operator()(_inout_opt_param _out_param_with_writes(nsize)T*&ptr,size_t nsize)const noexcept{
			base_call(ptr,nsize);
		}
		struct realloc_array_t{
			size_t _size;
			template<class T>
			[[nodiscard]]force_inline T*operator()(_inout_opt_param _out_param_with_writes(_size)T*&ptr)const noexcept{return base_call(ptr,_size);}
		};
		[[nodiscard]]force_inline constexpr realloc_array_t operator[](size_t a)const noexcept{return{a};}
	}realloc{};

	/// @brief 用于自特定类型的内存分配结果获取分配大小
	/// 对于null_ptr，返回0
	/// 对于单个对象的分配，返回1
	constexpr struct get_size_of_alloc_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=noexcept(get_size_of_alloc_method(declvalue(const T*)));

		template<typename T> requires able<T>
		static size_t base_call(_in_param const T*arg)noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg{1};
			if(arg==null_ptr)
				return 0;
			return get_size_of_alloc_method(arg);
		}

		template<typename T> requires able<T>
		force_inline size_t operator()(_in_param const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}get_size_of_alloc{};

	/// @brief 对于特定类型的内存分配结果，获取分配大小并复制一份同样大小的内存
	/// @warning 这个类不会对类实例进行复制函数调用，也不会创造或结束生命周期，如需要进行new like操作，请使用 copy_get_t
	constexpr struct copy_alloc_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=get_size_of_alloc.nothrow<T>;

		template<typename T> requires able<T>
		static T*base_call(_in_param const T*arg)noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg{1};
			return alloc<T>(get_size_of_alloc(arg));
		}

		template<typename T> requires able<T>
		force_inline T*operator()(_in_param const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}copy_alloc{};
}

//file_end

