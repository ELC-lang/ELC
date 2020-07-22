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
	//BLOCK:for debug
	[[nodiscard]]inline void*base_realloc(void*ptr,size_t nsize)noexcept{
		void*p=elc::APIs::alloc::realloc(ptr,nsize);
		#if defined(ELC_TEST_ON)
			if(nsize==0)
				stest_uneventlog(ptr);
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
		void*p=elc::APIs::alloc::aligned_alloc(align,size);
		#if defined(ELC_TEST_ON)
			if(p){
				stest_entryevent(L"base_aligned_alloc调用");
				stest_eventlog(p);
				stest_exitevent();
			}
		#endif
		return p;
	}
	inline void base_free(void*p,size_t size)noexcept{
		//传入需释放的数据块起始点与大小（字节）
		#if defined(ELC_TEST_ON)
			auto tmp=stest_geteventlistfromlog(p);
			if(!tmp){
				stest_putsf(L"释放了已释放或未分配指针%p，当前的事件记录如下：",p);
				stest_printeventlist(stderr,stest_geteventlist());
				stest_wait();
				p=nullptr;
			}else{
				stest_deleteevent(tmp);
				stest_uneventlog(p);
			}
		#endif
		elc::APIs::alloc::free(p,size);
	}
	//BLOCK_END

	//允许自定义对象的alloc/free/realloc/get_size_of_alloc方法：为pool留的后门（大概）
	//这里是缺省时的默认方法定义
	#include "overhead.hpp"

	template<typename T>
	inline void*alloc_method(type_info_t<T>)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		using namespace overhead_n;
		void*tmp=base_aligned_alloc(correct_align(type_info<T>),correct_size<T>(sizeof(T)));
		if(tmp){
			set_overhead(tmp,1);
			return ::std::assume_aligned<alignof(T)>(correct_pointer<T>(tmp));
		}
		else return nullptr;
	}
	template<typename T>
	inline void*alloc_method(type_info_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		//size被保证不为0
		using namespace overhead_n;
		void*tmp=base_aligned_alloc(correct_align(type_info<T>),correct_size<T>(sizeof(T)*size));
		if(tmp){
			set_overhead(tmp,size);
			return ::std::assume_aligned<alignof(T)>(correct_pointer<T>(tmp));
		}
		else return nullptr;
	}
	template<typename T>
	inline size_t get_size_of_alloc_method(const T*arg)noexcept{
		//arg保证不与null_ptr相等
		using namespace overhead_n;
		return get_overhead(recorrect_pointer(const_cast<T*>(arg)));
	}
	template<typename T>
	inline void free_method(T*arg)noexcept{
		using namespace overhead_n;
		base_free(recorrect_pointer(arg),correct_size<T>(get_overhead(recorrect_pointer(arg))));
	}
	template<typename T>
	inline void*realloc_method(T*&ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc_for_alloc，但ptr值必须保持有效以保证gc_for_alloc后再次realloc有效
		//new_size被保证不为0
		using namespace overhead_n;
		void*tmp=base_realloc(recorrect_pointer(ptr),correct_size<T>(sizeof(T)*new_size));
		if(tmp){
			set_overhead(tmp,new_size);
			ptr=reinterpret_cast<T*>(correct_pointer<T>(tmp));
			return ptr;
		}
		else return nullptr;
	}
	//

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
				template_warning("For never_in_array type,realloc will free ptr when new_size=0 else do nothing.");
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

	template<class T>
	inline size_t get_size_of_alloc(const T*arg)noexcept_as(get_size_of_alloc_method(declvalue(const T*))){
		if(arg==null_ptr)
			return 0;
		return get_size_of_alloc_method(arg);
	}

	template<class T>
	inline T*copy_alloc(const T*arg)noexcept{
		return alloc<T>(get_size_of_alloc(arg));
	}
}

//file_end

