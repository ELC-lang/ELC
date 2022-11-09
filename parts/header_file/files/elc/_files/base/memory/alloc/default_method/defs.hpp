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
	#if defined(_MSC_VER)
		#if defined(ELC_TEST_CHECK_MEMORY_LACK)
			#pragma detect_mismatch("ELC_TEST_CHECK_MEMORY_LACK","true")
		#else
			#pragma detect_mismatch("ELC_TEST_CHECK_MEMORY_LACK","false")
		#endif
	#endif

	/*!
	Reallocates a block of memory.
	
	@param ptr The pointer to the block of memory to reallocate.
	@param nsize The new size of the block of memory.
	@param align The alignment of the block of memory.
	
	@returns The new pointer to the block of memory.
	*/
	[[nodiscard]]inline _return_maybenull_has_size(nsize)byte*base_realloc(_in_param byte*ptr,size_t nsize,size_t align)noexcept{
		byte*p=::elc::APIs::alloc::realloc(ptr,nsize,align);
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
			if(nsize==0)
				stest_uneventlog(ptr);
			if(p){
				ELC_TEST_EVENTNAME("base_realloc调用");
				if(p!=ptr)
					stest_uneventlog(ptr);
				stest_eventlog(p);
			}
		#endif
		return p;
	}
	/*!
	Allocates aligned memory.
	
	@param align The alignment of the memory.
	@param size The size of the memory.
	
	@returns A pointer to the allocated memory.
	*/
	[[nodiscard]]inline _return_maybenull_has_size_not_inited(size)byte*base_aligned_alloc(size_t align,size_t size)noexcept{
		byte*p=::elc::APIs::alloc::aligned_alloc(align,size);
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
			if(p){
				ELC_TEST_EVENTNAME("base_aligned_alloc调用");
				stest_eventlog(p);
			}
		#endif
		return p;
	}
	/*!
	Frees a pointer allocated by base_alloc.
	
	@param p The pointer to free.
	@param align The alignment of the pointer.
	*/
	inline void base_free(_in_param byte*p,size_t align)noexcept{
		//传入需释放的数据块起始点与大小（字节）
		#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
			auto tmp=stest_geteventlistfromlog(p);
			if(!tmp){
				stest_putsf(L"释放了已释放或未分配指针%p，当前的事件记录如下：",p);
				stest_printeventlist(stderr,stest_geteventlist());
				stest_wait();
				return;
			}else{
				stest_deleteevent(tmp);
				stest_uneventlog(p);
			}
		#endif
		::elc::APIs::alloc::free(p,align);
	}
	/*!
	Computes the size of an allocation.
	
	@param arg The pointer to the start of the allocation.
	@param align The alignment of the allocation.
	
	@returns The size of the allocation.
	*/
	inline size_t base_get_size_of_alloc(_in_param const byte*arg, size_t align)noexcept{
		//arg保证不与null_ptr相等
		const auto tmp= ::elc::APIs::alloc::get_size_of_alloc(arg,align);
		return tmp;
	}
	//BLOCK_END

	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T))void*alloc_method(type_info_t<T>)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		return ::std::assume_aligned<alignof(T)>(base_aligned_alloc(alignof(T),sizeof(T)));
	}
	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T)*size)void*alloc_method(type_info_t<T>,size_t size)noexcept{
		//return空指针被允许，会引起gc_for_alloc
		//size被保证不为0
		if constexpr(type_info<T>.has_attribute(never_in_array))
			template_error("You cannot perform array operations on never_in_array type.");
		return ::std::assume_aligned<alignof(T)>(base_aligned_alloc(alignof(T),sizeof(T)*size));
	}
	template<typename T>
	inline size_t get_size_of_alloc_method(_in_param const T*arg)noexcept{
		//arg保证不与null_ptr相等
		return base_get_size_of_alloc(cast_to_data(arg),alignof(T))/sizeof(T);
	}
	template<typename T>
	inline void free_method(_in_param T*arg)noexcept{
		base_free(cast_to_data(arg),alignof(T));
	}
	template<typename T>
	inline _return_maybenull_has_size_in_bytes(sizeof(T)*new_size)void*realloc_method(_in_param T*ptr,size_t new_size)noexcept{
		//return空指针被允许，会引起gc_for_alloc，但ptr值必须保持有效以保证gc_for_alloc后再次realloc有效
		//new_size被保证不为0
		//align维持不变
		//但只允许在扩大数据块时可选的移动数据块
		if constexpr(type_info<T>.has_attribute(never_in_array))
			template_error("You cannot perform array operations on never_in_array type.");
		return reinterpret_cast<T*>(base_realloc(cast_to_data(ptr),sizeof(T)*new_size,alignof(T)));
	}
}

//file_end

