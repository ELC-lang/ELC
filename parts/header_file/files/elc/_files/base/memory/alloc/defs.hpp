//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace gc_n{
	void gc_for_alloc()noexcept;
}
namespace alloc_n{
	using ::elc::defs::memory::gc_n::gc_for_alloc;

	//允许自定义对象的alloc/free/realloc/get_size_of_alloc方法：为pool留的后门（大概）
	//这里是缺省时的默认方法定义
	#include "default_method/defs.hpp"
	
	void the_alloc_method()noexcept=delete;
	template<typename T>
	inline void*alloc_method(type_info_t<T>)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		if constexpr(was_not_an_ill_form(the_alloc_method(type_info<T>)))
			return the_alloc_method(type_info<T>);
		else
			return default_method::alloc_method(type_info<T>);
	}
	template<typename T>
	inline void*alloc_method(type_info_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		//size被保证不为0
		if constexpr(was_not_an_ill_form(the_alloc_method(type_info<T>,size)))
			return the_alloc_method(type_info<T>,size);
		else
			return default_method::alloc_method(type_info<T>,size);
	}
	void the_get_size_of_alloc_method()noexcept=delete;
	template<typename T>
	inline size_t get_size_of_alloc_method(const T*arg)noexcept{
		//arg保证不与null_ptr相等
		if constexpr(was_not_an_ill_form(the_get_size_of_alloc_method(arg)))
			return the_get_size_of_alloc_method(arg);
		else
			return default_method::get_size_of_alloc_method(arg);
	}
	void the_free_method()noexcept=delete;
	template<typename T>
	inline void free_method(T*arg)noexcept{
		if constexpr(was_not_an_ill_form(the_free_method(arg)))
			the_free_method(arg);
		else
			default_method::free_method(arg);
	}
	void the_realloc_method()noexcept=delete;
	template<typename T>
	inline void*realloc_method(T*&ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc_for_alloc，但ptr值必须保持有效以保证gc_for_alloc后再次realloc有效
		//new_size被保证不为0
		//align维持不变
		//但只允许在扩大数据块时可选的移动数据块
		if constexpr(was_not_an_ill_form(the_realloc_method(ptr,new_size)))
			return the_realloc_method(ptr,new_size);
		else
			return default_method::realloc_method(ptr,new_size);
	}

	//众所周知,cpp是面向对象语言.
	struct base_alloc_t{};
	template<class T>
	struct alloc_t:base_alloc_t{
		typedef base_alloc_t base_t;
		[[nodiscard]]static T*base_call()noexcept{
			void*tmp;
			while(!assign(tmp,alloc_method(type_info<T>)))gc_for_alloc();
			return reinterpret_cast<T*>(tmp);
		}
		[[nodiscard]]static T*base_call(size_t size)noexcept{
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You can\'t alloc an array for never_in_array type.");
			if(size){//null_ptr不一定等价于nullptr，请勿删除本行
				void*tmp;
				while(!assign(tmp,alloc_method(type_info<T>,size)))gc_for_alloc();
				return reinterpret_cast<T*>(tmp);
			}else return null_ptr;
		}
		[[nodiscard]]/*static*/T*operator()()const noexcept{return base_call();}
		[[nodiscard]]/*static*/T*operator()(size_t size)const noexcept{return base_call(size);}
		struct alloc_array_t{
			size_t _size;
			[[nodiscard]]T*operator()()const noexcept{return base_call(_size);}
		};
		[[nodiscard]]alloc_array_t operator[](size_t a)const noexcept{return{a};}
	};
	template<class T>
	constexpr alloc_t<T>alloc{};

	constexpr struct free_t{
		typedef free_t base_t;
		template<class T>
		static void base_call(T*p)noexcept{
			if(p!=null_ptr)//null_ptr不一定等价于nullptr，请勿删除本行
				free_method(p);
		}
		template<class T>
		/*static*/void operator()(T*p)const noexcept{base_call(p);}
	}free{};

	constexpr struct realloc_t{
		typedef realloc_t base_t;
		template<class T>
		static void base_call(T*&ptr,size_t nsize)noexcept{
			if constexpr(type_info<T>.has_attribute(never_in_array))
				template_error("You cannot perform array operations on never_in_array type.");
				//template_warning("For never_in_array type,realloc will free ptr when new_size=0 else do nothing.");
			if(nsize){//null_ptr不一定等价于nullptr，请勿删除本行
				if constexpr(type_info<T>.not_has_attribute(never_in_array))
					if(ptr!=null_ptr){//null_ptr不一定等价于nullptr，请勿删除本行
						while(!realloc_method(ptr,nsize))gc_for_alloc();
					}else
						ptr=alloc<T>(nsize);
			}else{
				free(ptr);
				ptr=null_ptr;
			}
		}
		template<class T>
		/*static*/void operator()(T*&ptr,size_t nsize)const noexcept{
			base_call(ptr,nsize);
		}
		struct realloc_array_t{
			size_t _size;
			template<class T>
			[[nodiscard]]T*operator()(T*&ptr)const noexcept{return base_call(ptr,_size);}
		};
		[[nodiscard]]realloc_array_t operator[](size_t a)const noexcept{return{a};}
	}realloc{};

	constexpr struct get_size_of_alloc_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=noexcept(get_size_of_alloc_method(declvalue(const T*)));

		template<typename T> requires able<T>
		static size_t base_call(const T*arg)noexcept(nothrow<T>){
			if(arg==null_ptr)
				return 0;
			return get_size_of_alloc_method(arg);
		}

		template<typename T> requires able<T>
		size_t operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}get_size_of_alloc{};

	constexpr struct copy_alloc_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=get_size_of_alloc.nothrow<T>;

		template<typename T> requires able<T>
		static T*base_call(const T*arg)noexcept(nothrow<T>){
			return alloc<T>(get_size_of_alloc(arg));
		}

		template<typename T> requires able<T>
		T*operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}copy_alloc{};
}

//file_end

