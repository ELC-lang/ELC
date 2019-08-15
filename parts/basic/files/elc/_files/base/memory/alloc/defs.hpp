//defs.hpp
//at namespace elc::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace alloc_n{
	//BLOCK:for debug
	[[nodiscard]]inline void*base_alloc(size_t size)noexcept{
		void*p= ::std::malloc(size);
		#if defined(ELC_TEST_ON)
			if(p){
				stest_entryevent(L"base_alloc调用");
				stest_eventlog(p);
				stest_exitevent();
			}
		#endif
		return p;
	}
	[[nodiscard]]inline void*base_realloc(void*ptr,size_t nsize)noexcept{
		void*p= ::std::realloc(ptr,nsize);
		#if defined(ELC_TEST_ON)
			if(p){
				stest_entryevent(L"base_realloc调用");
				if(p!=ptr)
					stest_uneventlog(ptr);
				stest_eventlog(p);
				stest_exitevent();
			}
		#endif
		return p;
	}
	[[nodiscard]]inline void*base_aligned_alloc(size_t align,size_t size)noexcept{
		void*p= ::std::aligned_alloc(align,size);
		#if defined(ELC_TEST_ON)
			if(p){
				stest_entryevent(L"base_aligned_alloc调用");
				stest_eventlog(p);
				stest_exitevent();
			}
		#endif
		return p;
	}
	inline void base_free(void*p)noexcept{
		#if defined(ELC_TEST_ON)
			stest_uneventlog(p);
		#endif
		::std::free(p);
	}
	//BLOCK_END

	//允许自定义对象的alloc/free/realloc/get_size_of_alloc方法：为pool留的后门（大概）
	//这里是缺省时的默认方法定义
	#include"overhead.hpp"

	template<typename T>
	inline T*alloc_method(type_pack_t<T>)noexcept{
		//return空指针被允许，会引起gc
		void*tmp=base_aligned_alloc(correct_align(alignof(T)),correct_size(sizeof(T)));
		if(tmp){
			set_overhead(tmp,1);
			return ::std::assume_aligned<alignof(T)>(correct_pointer(tmp));
		}
		return nullptr;
	}
	template<typename T>
	inline T*alloc_method(type_pack_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc
		//size被保证不为0
		void*tmp=base_aligned_alloc(correct_align(alignof(T)),correct_size(sizeof(T)*size));
		if(tmp){
			set_overhead(tmp,size);
			return ::std::assume_aligned<alignof(T)>(correct_pointer(tmp));
		}
		return nullptr;
	}
	template<typename T>
	inline size_t get_size_of_alloc_method(T*arg)noexcept{
		return get_overhead(recorrect_pointer(arg));
	}
	template<typename T>
	inline void free_method(T*arg)noexcept{
		base_free(recorrect_pointer(arg));
	}
	template<typename T>
	inline void*realloc_method(T*&ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc，但ptr值必须保持有效以保证gc后再次realloc有效
		//new_size被保证不为0
		void*tmp=base_realloc(recorrect_pointer(ptr),correct_size(sizeof(T)*new_size));
		if(tmp){
			set_overhead(tmp,new_size);
			ptr=correct_pointer(tmp);
			return ptr;
		}
		return tmp;
	}
	//

	//众所周知,cpp是面向对象语言.
	struct base_alloc_t{};
	template<class T>
	struct alloc_t:base_alloc_t{
		typedef base_alloc_t base_t;
		[[nodiscard]]static T*base_call()noexcept{
			void*tmp;
			while(!assign(tmp,alloc_method(type_pack<T>)))gc();
			return reinterpret_cast<T*>(tmp);
		}
		[[nodiscard]]static T*base_call(size_t size)noexcept{
			if(size){//辣鸡c标准没有规定0大小分配的返回值，所以检查
				void*tmp;
				while(!assign(tmp,alloc_method(type_pack<T>,size)))gc();
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
			if(nsize){//辣鸡c标准没有规定0大小分配的返回值，所以检查
				if(ptr!=null_ptr){//null_ptr不一定等价于nullptr，请勿删除本行
					while(realloc_method(ptr,nsize))gc();
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
			[[nodiscard]]T*operator()()const noexcept{return base_call(_size);}
		};
		[[nodiscard]]realloc_array_t operator[](size_t a)const noexcept{return{a};}
	}realloc{};
}
