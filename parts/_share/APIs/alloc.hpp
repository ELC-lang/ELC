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
	#include "../../header_file/files/elc/base_defs"
	namespace elc::APIs::alloc{
		#include "_tools/decl_system_type.hpp"
		#if SYSTEM_TYPE == windows
			#include <malloc.h>
			using namespace elc::defs;//remove_const
		#else
			#include "alloc/default_method/overhead.hpp"
		#endif
		[[nodiscard]]inline void*aligned_alloc(size_t align,size_t size)noexcept{
			//return空指针被允许
			//size被保证不为0
			#if SYSTEM_TYPE == windows
				return _aligned_malloc(size,align);
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
		[[nodiscard]]inline void*realloc(byte*ptr,size_t nsize,[[maybe_unused]]size_t align)noexcept{
			//return空指针被允许，但ptr值必须保持有效以保证gc后再次realloc有效
			//new_size被保证不为0
			//align维持不变
			//但只允许在扩大数据块时可选的移动数据块
			#if SYSTEM_TYPE == windows
				return _aligned_realloc(ptr,nsize,align);
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
		inline void free(void*p,[[maybe_unused]]size_t align)noexcept{
			//传入需释放的数据块起始点与大小（字节）与对齐
			#if SYSTEM_TYPE == windows
				_aligned_free(p);
			#else
				using namespace overhead_n;
				::std::free(recorrect_pointer(p,align));
			#endif
		}
		[[nodiscard]]inline size_t get_size_of_alloc(const byte*p,[[maybe_unused]]size_t align)noexcept{
			//传入需获取大小的数据块起始点与对齐
			#if SYSTEM_TYPE == windows
				return _aligned_msize(remove_const(p),align,0);
			#else
				using namespace overhead_n;
				return get_overhead(recorrect_pointer(p,align));
			#endif
		}
		#include "_tools/undef_decl_system_type.hpp"
	}
#endif

//file_end

