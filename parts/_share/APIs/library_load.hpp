//library_load.hpp
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
#if !defined(ELC_APIS_library_load)
	#define ELC_APIS_library_load
	#include "_tools/decl_system_type.hpp"
	#include "../../header_file/files/elc/string"
	#if SYSTEM_TYPE == linux
		#include <dlfcn.h>
	#elif SYSTEM_TYPE == windows
		#define NOMINMAX
		#include <Windows.h>
		#include <Libloaderapi.h>
		#include <Errhandlingapi.h>//GetLastError
	#else
		#error ERROR_MSG_UNABLE_OS
	#endif
	namespace elc::APIs::library_load{
		#include "../_defs.hpp"
		using namespace elc::defs;

		typedef
		#if SYSTEM_TYPE == linux
			void*
		#elif SYSTEM_TYPE == windows
			HMODULE
		#endif
		library_handle;

		[[nodiscard]]inline library_handle base_load_library(const char*file_name)noexcept{
			//可返回bool意义为空的值表示失败
			return
			#if SYSTEM_TYPE == linux
				dlopen(file_name,RTLD_LAZY)
			#elif SYSTEM_TYPE == windows
				LoadLibraryA((LPCSTR)file_name)
			#endif
			;
		}
		[[nodiscard]]inline string base_get_load_error()noexcept{
			return
			#if SYSTEM_TYPE == linux
				to_char_t_str(dlerror());
			#elif SYSTEM_TYPE == windows
				es"ERROR CODE:"_constexpr_str+to_string((DWORD)GetLastError());//傻逼微软设计的jb端口，再您妈的见
			#endif
			;
		}
		inline void base_free_library(library_handle handle)noexcept{
			#if SYSTEM_TYPE == linux
				dlclose(handle)
			#elif SYSTEM_TYPE == windows
				FreeLibrary(handle)
			#endif
			;
		}
		[[nodiscard]]inline void* base_get_symbol(library_handle handle,const char*symbol_name)noexcept{
			//可返回bool意义为空的值表示失败
			return
			#if SYSTEM_TYPE == linux
				dlsym(handle,symbol_name)
			#elif SYSTEM_TYPE == windows
				(void*)GetProcAddress(handle,(LPCSTR)symbol_name)
			#endif
			;
		}

		[[nodiscard]]inline library_handle load_library(string file_name)noexcept{
			//可返回bool意义为空的值表示失败
			return base_load_library(to_char_str(file_name).c_str());
		}
		[[nodiscard]]inline string get_load_error()noexcept{
			return base_get_load_error();
		}
		inline void free_library(library_handle handle)noexcept{
			return base_free_library(handle);
		}
		[[nodiscard]]inline void* get_symbol(library_handle handle,string symbol_name)noexcept{
			//可返回bool意义为空的值表示失败
			return base_get_symbol(handle,to_char_str(symbol_name).c_str());
		}
		#include "../_undefs.hpp"
	}
	#include "_tools/undef_decl_system_type.hpp"
#endif

//file_end

