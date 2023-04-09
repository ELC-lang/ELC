//alloc.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
/*
APIs说明
elc依赖的基础函数.
调整这些,实现快速移植,快速优化与行为自定义.
*/
#if !defined(ELC_APIS_alloc)
	#define ELC_APIS_alloc
	#if defined(ELC_MEMORY_ALLOC_LOCATION_RECORD)
		#include <stacktrace>//for operate_source_location
	#endif
	#include "_tools/decl_system_type.hpp"
	//overhead默认启用（即使windows下可以通过_aligned_msize获取块大小），参见https://developercommunity.visualstudio.com/t/_aligned_realloc-does-not-reduce-block-s/10334132
	#define ELC_USING_OVERHEADED_ALLOC
	#if SYSTEM_TYPE != windows
		#if defined(_DEBUG)
			#include <crtdbg.h>
		#else
			#include <malloc.h>
		#endif
	#else
		#include <cstdlib>
	#endif
	#include "../../header_file/files/elc/base_defs"
	/// @brief alloc相关基础函数
	namespace elc::APIs::alloc{
		#include "../../_share/_defs.hpp"

		using namespace ::elc::defs;//remove_const||byte
		#if defined(ELC_USING_OVERHEADED_ALLOC)
			#include "alloc/default_method/overhead.hpp"//overhead
		#endif

		#include "alloc/debug_info/source_location_guard.hpp"//operate_source_location & source_location_guard

		//BLOCK: Counting memory allocations
		#if defined(_MSC_VER)
			#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
				#pragma detect_mismatch("ELC_TEST_COUNT_MEMORY_ALLOC","true")
			#else
				#pragma detect_mismatch("ELC_TEST_COUNT_MEMORY_ALLOC","false")
			#endif
		#endif

		#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
			[[nodiscard]]inline size_t get_size_of_alloc(const byte*p,size_t align)noexcept;
			namespace count_info{
				distinctive inline size_t alloc_count=0;
				distinctive inline size_t free_count=0;
				distinctive inline size_t alloc_size=0;
				distinctive inline size_t free_size=0;
				distinctive inline size_t memory_using=0;
				distinctive inline size_t memory_using_max=0;
				inline void update_memory_using(ptrdiff_t diff)noexcept{
					memory_using+=diff;
					if(memory_using>memory_using_max)
						memory_using_max=memory_using;
				}
				inline void clear()noexcept{
					alloc_count=0;
					free_count=0;
					alloc_size=0;
					free_size=0;
					memory_using=0;
					memory_using_max=0;
				}
			}
		#endif
		//BLOCK_END

		#if defined(ELC_MEMORY_ALLOC_LOCATION_RECORD) && defined(ELC_SPEED_TEST)
			#error "ELC_MEMORY_ALLOC_LOCATION_RECORD and ELC_SPEED_TEST can't be defined at the same time."
		#endif

		inline constexpr bool use_debug_alloc_lib=
			#if in_debug && !defined(ELC_SPEED_TEST)
				true
			#else
				false
			#endif
		;

		#if SYSTEM_TYPE != windows
			struct default_operators_provider_t{
				static void* aligned_alloc(size_t align,size_t size)noexcept{
					return ::std::aligned_alloc(align,size);
				}
				static void aligned_free(void*ptr,[[maybe_unused]]size_t align)noexcept{
					::std::free(ptr);
				}
				static void* aligned_realloc(void*ptr,size_t size,[[maybe_unused]]size_t align)noexcept{
					return ::std::realloc(ptr,size);
				}
				static size_t aligned_blocksize([[maybe_unused]]const void*ptr,[[maybe_unused]]size_t align)noexcept{
					return 0;//无法获取，所以依赖overhead
				}
			};
		#else
			struct default_operators_provider_t{
				static void* aligned_alloc(size_t align,size_t size)noexcept{
					if constexpr(use_debug_alloc_lib)
						return _aligned_malloc_dbg(size,align,operate_source_location.file(),operate_source_location.line());
					else
						return _aligned_malloc(size,align);
				}
				static void aligned_free(void*ptr,[[maybe_unused]]size_t align)noexcept{
					if constexpr(use_debug_alloc_lib)
						_aligned_free_dbg(ptr);
					else
						_aligned_free(ptr);
				}
				static void* aligned_realloc(void*ptr,size_t size,size_t align)noexcept{
					if constexpr(use_debug_alloc_lib)
						return _aligned_realloc_dbg(ptr,size,align,operate_source_location.file(),operate_source_location.line());
					else
						return _aligned_realloc(ptr,size,align);
				}
				static size_t aligned_blocksize(const void*ptr,size_t align)noexcept{
					if constexpr(use_debug_alloc_lib)
						return _aligned_msize_dbg(remove_const(ptr),align,0);
					else
						return _aligned_msize(remove_const(ptr),align,0);
				}
			};
		#endif
		
		#if defined(ELC_USING_OVERHEADED_ALLOC)
			template<class base_operators>
			struct overheaded_operators_provider_t{
				static void* aligned_alloc(size_t align,size_t size)noexcept{
					using namespace overhead_n;
					void*tmp=base_operators::aligned_alloc(correct_align(align),correct_size(size,align));
					if(tmp){
						set_overhead(tmp,size);
						return correct_pointer(tmp,align);
					}
					else
						return nullptr;
				}
				static void aligned_free(void*ptr,size_t align)noexcept{
					using namespace overhead_n;
					base_operators::aligned_free(recorrect_pointer(ptr,align),correct_align(align));
				}
				static void* aligned_realloc(void*ptr,size_t size,size_t align)noexcept{
					using namespace overhead_n;
					void*tmp=base_operators::aligned_realloc(recorrect_pointer(ptr,align),correct_size(size,align),correct_align(align));
					if(tmp){
						set_overhead(tmp,size);
						return correct_pointer(tmp,align);
					}
					else
						return nullptr;
				}
				static size_t aligned_blocksize(const void*ptr,size_t align)noexcept{
					using namespace overhead_n;
					return get_overhead(recorrect_pointer(ptr,align));
				}
			};
		#endif

		typedef
		#if defined(ELC_USING_OVERHEADED_ALLOC)
			overheaded_operators_provider_t<default_operators_provider_t>
		#else
			default_operators_provider_t
		#endif
		alloc_operators_provider;
		/*!
		aligned_alloc 内存分配函数，需提供对齐需求
		return空指针被允许
		size被保证不为0
		*/
		[[nodiscard]]inline byte*aligned_alloc(size_t align,size_t size)noexcept{
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			void* aret;//返回值放这里
			pop_msvc_warning();

			aret=alloc_operators_provider::aligned_alloc(align,size);

			#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
				if(aret){
					count_info::alloc_count++;
					count_info::alloc_size+=size;
					count_info::update_memory_using(size);
				}
			#endif

			return(byte*)aret;
		}
		/*!
		realloc 重新规划分配的大小
		return空指针被允许，但ptr值必须保持有效以保证gc后再次realloc有效
		new_size被保证不为0
		align维持不变
		但只允许在扩大数据块时可选的移动数据块
		*/
		[[nodiscard]]inline byte*realloc(byte*ptr,size_t nsize,size_t align)noexcept{
			#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
				const auto osize=get_size_of_alloc(ptr,align);
			#endif
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			void* aret;//返回值放这里
			pop_msvc_warning();

			aret=alloc_operators_provider::aligned_realloc(ptr,nsize,align);

			#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
				if(aret){
					count_info::alloc_size+=nsize-osize;
					count_info::update_memory_using(nsize-osize);
				}
			#endif

			return(byte*)aret;
		}
		/*!
		free 释放所分配的内存
		传入需获取大小的数据块起始点与对齐
		*/
		inline void free(byte*p,size_t align)noexcept{
			#if defined(ELC_TEST_COUNT_MEMORY_ALLOC)
				const auto size=get_size_of_alloc(p,align);
				count_info::free_count++;
				count_info::free_size+=size;
				count_info::update_memory_using(-ptrdiff_t(size));
			#endif

			alloc_operators_provider::aligned_free(p,align);
		}
		/*!
		get_size_of_alloc 获取数据块的大小
		传入需获取大小的数据块起始点与对齐
		*/
		[[nodiscard]]inline size_t get_size_of_alloc(const byte*p,[[maybe_unused]]size_t align)noexcept{
			return alloc_operators_provider::aligned_blocksize(p,align);
		}

		#include "../../_share/_undefs.hpp"
	}
	//
	#include "_tools/undef_decl_system_type.hpp"
#endif

//file_end

