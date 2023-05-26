//function_name.hpp
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
#if !defined(ELC_APIS_function_name)
	#define ELC_APIS_function_name
	#include "../../header_file/files/elc/base_defs"
	#include "_tools/decl_system_type.hpp"
	#if SYSTEM_TYPE == linux
		#include <dlfcn.h>
	#elif SYSTEM_TYPE == windows
		#define NOMINMAX
		#include <Windows.h>
		#include <dbghelp.h>
		#pragma comment(lib,"dbghelp.lib")
	#else
		#error ERROR_MSG_UNABLE_OS
	#endif
	#undef other
	namespace elc::APIs::function_name{
		#include "../_defs.hpp"

		using namespace ::elc::defs;//get def

		#if SYSTEM_TYPE == linux
			struct function_name_t:demangle_name_t{
				using base_t=demangle_name_t;
				using base_t::base_t;
			};
		#elif SYSTEM_TYPE == windows
			//windows需要额外复制一份
			push_and_disable_msvc_warning(26408);//malloc/free警告diss
			struct function_name_t:string_view_t<char>{
				using base_t=string_view_t<char>;
				string_view_t<char> copy_name(string_view_t<char>name)noexcept{
					auto tmp=(char*)::std::malloc(name.size()+1);
					if(tmp) {
						memcpy(tmp, name.data(), name.size());
						tmp[name.size()] = 0;
						return string_view_t<char>{tmp, name.size()};
					}
					else
						return base_t{nullptr,0};
				}
				string_view_t<char> demangle_name(string_view_t<char>name)noexcept{
					char tmp[1024];
					if(name.starts_with("@ILT+")){
						const auto lit_end_pos=name.find('(');
						if(lit_end_pos==base_t::npos)
							return copy_name(name);
						name=name.substr(lit_end_pos+1);
						name=name.substr(size_t{0},name.size()-1);
						memcpy(tmp,name.data(),name.size());
						tmp[name.size()]=0;
						name={tmp,name.size()};
					}
					const auto len=UnDecorateSymbolName(name.data(),tmp,1024,UNDNAME_COMPLETE);
					if(len)
						return copy_name(string_view_t<char>{tmp,len});
					else
						return copy_name(name);
				}
				function_name_t(const char*str,ULONG len)noexcept:base_t(demangle_name(base_t{str,len})){}
				function_name_t(const function_name_t&other)noexcept:base_t(copy_name(other)){}
				function_name_t(function_name_t&&other)noexcept:base_t(nullptr,0){
					swap(other,*this);
				}
				~function_name_t()noexcept{
					::std::free((void*)base_t::data());
				}
				function_name_t&operator=(const function_name_t&other)noexcept{
					function_name_t tmp(other);
					swap(tmp,*this);
					return *this;
				}
				function_name_t&operator=(function_name_t&&other)noexcept{
					swap(other,*this);
					return *this;
				}
			};
			pop_msvc_warning();
			//一个额外的全局对象用于SymInitialize和SymCleanup
			struct symbol_init_t{
				bool _inited;
				symbol_init_t()noexcept{
					SymSetOptions(SYMOPT_DEFERRED_LOADS);
					_inited=SymInitialize(GetCurrentProcess(), nullptr, true);
				}
				~symbol_init_t()noexcept{
					if(_inited)
						SymCleanup(GetCurrentProcess());
				}
			};
			inline extern symbol_init_t symbol_init{};
		#endif
		[[nodiscard]]inline function_name_t get_function_name(void*ptr)noexcept{
			#if SYSTEM_TYPE==windows
				//SymFromAddr (dbghelp.h)
				struct symbol_info_t:SYMBOL_INFO{
					char name_addtional_space[MAX_SYM_NAME];
				};
				symbol_info_t symbol;
				symbol.SizeOfStruct=sizeof(SYMBOL_INFO);
				symbol.MaxNameLen=MAX_SYM_NAME;
				DWORD64 displacement;
				if(SymFromAddr(GetCurrentProcess(),(DWORD64)ptr,&displacement,&symbol))
					return function_name_t(symbol.Name,symbol.NameLen);
				else
					return function_name_t("unknown",7);
			#elif SYSTEM_TYPE==linux
				//dladdr (dlfcn.h)
				Dl_info info;
				if(dladdr(ptr,&info))
					return function_name_t(info.dli_sname);
				else
					return function_name_t("unknown");
			#endif
		}

		#include "../_undefs.hpp"
	}
	//
	#include "_tools/undef_decl_system_type.hpp"
	namespace elc::defs{
		using namespace elc::APIs::function_name;
	}
#endif

//file_end

