//alloc.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
APIs说明
elc依赖的基础函数.
调整这些,实现快速移植,快速优化与行为自定义.
*/
#if !defined(ELC_APIS_alloc)
	#define ELC_APIS_alloc
	#include <cstdlib>
	#if defined(_DEBUG)
		#include <stacktrace>//for operate_source_location
	#endif
	#if SYSTEM_TYPE == windows
		#include <malloc.h>
	#endif
	#include "../../header_file/files/elc/base_defs"
	#include "../../_share/_defs.hpp"
	namespace elc::APIs::alloc{
		#include "_tools/decl_system_type.hpp"
		#if SYSTEM_TYPE == windows
			using namespace elc::defs;//remove_const
		#else
			#include "alloc/default_method/overhead.hpp"//overhead
		#endif
		#include "alloc/debug_info/source_location_guard.hpp"//operate_source_location & source_location_guard
		/*
		aligned_alloc 内存分配函数，需提供对齐需求
		return空指针被允许
		size被保证不为0
		*/
		[[nodiscard]]inline void*aligned_alloc(size_t align,size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				#if defined(_DEBUG)
					return _aligned_malloc_dbg(size,align,operate_source_location.file(),operate_source_location.line());
				#else
					return _aligned_malloc(size,align);
				#endif
			#else
				using namespace overhead_n;
				void*tmp=::std::aligned_alloc(correct_align(align),correct_size(size,align));
				if(tmp){
					set_overhead(tmp,size);
					return correct_pointer(tmp,align);
				}
				else return nullptr;
			#endif
		}
		/*
		realloc 重新规划分配的大小
		return空指针被允许，但ptr值必须保持有效以保证gc后再次realloc有效
		new_size被保证不为0
		align维持不变
		但只允许在扩大数据块时可选的移动数据块
		*/
		[[nodiscard]]inline void*realloc(byte*ptr,size_t nsize,[[maybe_unused]]size_t align)noexcept{
			#if SYSTEM_TYPE == windows
				#if defined(_DEBUG)
					return _aligned_realloc_dbg(ptr,nsize,align,operate_source_location.file(),operate_source_location.line());
				#else
					return _aligned_realloc(ptr,nsize,align);
				#endif
			#else
				using namespace overhead_n;
				void*tmp=::std::realloc(recorrect_pointer(ptr,align),correct_size(nsize,align));
				if(tmp){
					set_overhead(tmp,nsize);
					return correct_pointer(tmp,align);
				}
				else return nullptr;
			#endif
		}
		/*
		free 释放所分配的内存
		传入需获取大小的数据块起始点与对齐
		*/
		inline void free(void*p,[[maybe_unused]]size_t align)noexcept{
			#if SYSTEM_TYPE == windows
				#if defined(_DEBUG)
					_aligned_free_dbg(p);
				#else
					_aligned_free(p);
				#endif
			#else
				using namespace overhead_n;
				::std::free(recorrect_pointer(p,align));
			#endif
		}
		/*
		get_size_of_alloc 获取数据块的大小
		传入需获取大小的数据块起始点与对齐
		*/
		[[nodiscard]]inline size_t get_size_of_alloc(const byte*p,[[maybe_unused]]size_t align)noexcept{
			#if SYSTEM_TYPE == windows
				#if defined(_DEBUG)
					return _aligned_msize_dbg(remove_const(p),align,0);
				#else
					return _aligned_msize(remove_const(p),align,0);
				#endif
			#else
				using namespace overhead_n;
				return get_overhead(recorrect_pointer(p,align));
			#endif
		}
		#include "_tools/undef_decl_system_type.hpp"
	}
	#include "../../_share/_undefs.hpp"
#endif

//file_end

