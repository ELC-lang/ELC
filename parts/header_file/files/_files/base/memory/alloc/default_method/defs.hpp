//defs.hpp
//at namespace elc::defs::memory::alloc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace default_method{
	//BLOCK:for debug
	[[nodiscard]]inline void*base_realloc(void*ptr,size_t osize,size_t nsize)noexcept{
		void*p=::elc::APIs::alloc::realloc(ptr,osize,nsize);
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
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
		void*p=::elc::APIs::alloc::aligned_alloc(align,size);
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
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
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
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
		::elc::APIs::alloc::free(p,size);
	}
	//BLOCK_END

	#include "overhead.hpp"

	template<typename T>
	inline void*alloc_method(type_info_t<T>)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		if constexpr(type_info<T>.has_attribute(never_in_array))
			return ::std::assume_aligned<alignof(T)>(base_aligned_alloc(alignof(T),sizeof(T)));
		else{
			using namespace overhead_n;
			void*tmp=base_aligned_alloc(correct_align(type_info<T>),correct_size<T>(sizeof(T)));
			if(tmp){
				set_overhead(tmp,1);
				return ::std::assume_aligned<alignof(T)>(correct_pointer<T>(tmp));
			}
			else return nullptr;
		}
	}
	template<typename T>
	inline void*alloc_method(type_info_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		//size被保证不为0
		if constexpr(type_info<T>.has_attribute(never_in_array))
			template_error("You cannot perform array operations on never_in_array type.");
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
		if constexpr(type_info<T>.has_attribute(never_in_array))
			return 1;
		else{
			using namespace overhead_n;
			return get_overhead(recorrect_pointer(const_cast<T*>(arg)));
		}
	}
	template<typename T>
	inline void free_method(T*arg)noexcept{
		if constexpr(type_info<T>.has_attribute(never_in_array))
			base_free(arg,sizeof(T));
		else{
			using namespace overhead_n;
			base_free(recorrect_pointer(arg),correct_size<T>(get_overhead(recorrect_pointer(arg))));
		}
	}
	template<typename T>
	inline void*realloc_method(T*&ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc_for_alloc，但ptr值必须保持有效以保证gc_for_alloc后再次realloc有效
		//new_size被保证不为0
		//align维持不变
		//但只允许在扩大数据块时可选的移动数据块
		if constexpr(type_info<T>.has_attribute(never_in_array))
			template_error("You cannot perform array operations on never_in_array type.");
		using namespace overhead_n;
		void*tmp=base_realloc(recorrect_pointer(ptr),get_size_of_alloc_method(ptr),correct_size<T>(sizeof(T)*new_size));
		if(tmp){
			set_overhead(tmp,new_size);
			ptr=reinterpret_cast<T*>(correct_pointer<T>(tmp));
			return ptr;
		}
		else return nullptr;
	}
}

//file_end

