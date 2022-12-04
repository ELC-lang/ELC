//die.hpp
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
#if !defined(ELC_APIS_die)
	#define ELC_APIS_die
	#include <cuchar>
	#include <clocale>
	#include "../default_data_type.hpp"
	#include "./char_set.hpp"
	#include "_tools/decl_system_type.hpp"
	#if SYSTEM_TYPE == windows
		#define NOMINMAX
		#include <Windows.h>
		#include <debugapi.h>
	#endif
	/// @brief 终止报错相关基础函数
	namespace elc::APIs::die{
		#include "../../_share/_defs.hpp"

		using namespace elc::defs;

		/// @brief 终止整个程序并可能的触发断点或输出错误信息
		[[noreturn]]inline void die()noexcept{
			#if SYSTEM_TYPE == windows
				if(IsDebuggerPresent())
					__debugbreak();
			#endif
			#if defined(_WINMAIN_)
				::MessageBoxW(NULL,L"elc died.",NULL,MB_ICONERROR);
			#else
				::std::fputs("elc died.\n",stderr);
				::std::fflush(stderr);
			#endif
			::std::abort();
		}
		/// @brief 同 die() ，但是有详细的错误信息
		/// @param err_msg 错误信息
		[[noreturn]]inline void die_with(string_view err_msg)noexcept{
			#if SYSTEM_TYPE == windows
				if(IsDebuggerPresent())
					__debugbreak();
			#endif
			if(err_msg){
				#if SYSTEM_TYPE == windows
					push_and_disable_msvc_warning(26494);//未初始化警告diss
					wchar_t err_msg_in_wchar[2048];
					pop_msvc_warning();
					const auto size=char_set::utf32_to_utf16((char16_t*)err_msg_in_wchar,err_msg).processed_output().size();
					err_msg_in_wchar[size]=L'\0';
					#if defined(_WINMAIN_)
						::MessageBoxW(NULL,err_msg_in_wchar,NULL,MB_ICONERROR);
					#else
						auto std_err=GetStdHandle(STD_ERROR_HANDLE);
						WriteConsoleW(std_err,L"elc died because:\n",17,NULL,NULL);
						WriteConsoleW(std_err,err_msg_in_wchar,(DWORD)size,NULL,NULL);
						WriteConsoleW(std_err,L"\n",1,NULL,NULL);
						FlushFileBuffers(std_err);
					#endif
				#else
					::std::mbstate_t stat{};
					push_and_disable_msvc_warning(26494);//未初始化警告diss
					char err_msg_in_char[2048];
					char* err_msg_write = err_msg_in_char;
					size_t s;
					pop_msvc_warning();
					for(auto c:err_msg){
						s = ::std::c32rtomb(err_msg_write, c, &stat);
						push_and_disable_msvc_warning(26475)//强转警告diss
						if(s == size_t(-1))
							die();
						pop_msvc_warning();
						err_msg_write += s;
					}
					*err_msg_write = '\0';
					::std::fputs("elc died because:\n",stderr);
					::std::fputs(err_msg_in_char,stderr);
					::std::fputc('\n',stderr);
					::std::fflush(stderr);
				#endif
			}
			else
				die();
			::std::abort();
		}

		#include "../../_share/_undefs.hpp"
	}
	//
	#include "_tools/undef_decl_system_type.hpp"

	namespace elc::defs{
		using elc::APIs::die::die;
		using elc::APIs::die::die_with;
	}
#endif

//file_end

