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
	#if SYSTEM_TYPE == linux
		#include <dlfcn.h>
	#elif SYSTEM_TYPE == windows
		#include <Libloaderapi.h>
		#include <Errhandlingapi.h>//GetLastError
	#else
		#error ERROR_MSG_UNABLE_OS
	#endif
	namespace elc::APIs::library_load{
		typedef
		#if SYSTEM_TYPE == linux
			void*
		#elif SYSTEM_TYPE == windows
			HMODULE
		#endif
		library_handle;

		[[nodiscard]]library_handle base_load_library(const char*file_name){
			//可返回bool意义为空的值表示失败
			return
			#if SYSTEM_TYPE == linux
				dlopen(file_name,RTLD_LAZY)
			#elif SYSTEM_TYPE == windows
				LoadLibraryA((LPCSTR)file_name)
			#endif
			;
		}
		[[nodiscard]]const char* base_get_load_error(){
			return
			#if SYSTEM_TYPE == linux
				(char*)dlerror()
			#elif SYSTEM_TYPE == windows
				"unknown error."//"ERROR CODE:"+to_string((DWORD)GetLastError());//傻逼微软设计的jb端口，再您妈的见
			#endif
			;
		}
		void base_free_library(library_handle handle){
			#if SYSTEM_TYPE == linux
				dlclose(handle)
			#elif SYSTEM_TYPE == windows
				FreeLibrary(handle)
			#endif
			;
		}
		[[nodiscard]]void* base_get_symbol(library_handle handle,const char*symbol_name){
			//可返回bool意义为空的值表示失败
			#if SYSTEM_TYPE == linux
				dlsym(handle,symbol_name)
			#elif SYSTEM_TYPE == windows
				(void*)GetProcAddress(handle,(LPCSTR)symbol_name)
			#endif
			;
		}

		[[nodiscard]]library_handle load_library(char_t*file_name){
			//可返回bool意义为空的值表示失败
			return base_load_library((const char*)(to_char_str(file_name)+zero));
		}
		[[nodiscard]]string_t<char_t> get_load_error(){
			return to_char_t_str(base_get_load_error());
		}
		void free_library(library_handle handle){
			return base_free_library(handle);
		}
		[[nodiscard]]void* get_symbol(library_handle handle,char_t*symbol_name){
			//可返回bool意义为空的值表示失败
			return base_get_symbol(handle,(const char*)(to_char_str(symbol_name)+zero));
		}
	}
	#include "_tools/undef_decl_system_type.hpp"
#endif

//file_end

