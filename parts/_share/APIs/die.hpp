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
	/// @brief 终止报错相关基础函数
	namespace elc::APIs::die{
		#include "../../_share/_defs.hpp"

		using namespace elc::defs;

		/// @brief 终止整个程序并可能的触发断点或输出错误信息
		[[noreturn]]inline void die()noexcept{
			#if defined(_MSC_VER)
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
		/// @brief 同 die ，但是有详细的错误信息
		/// @param err_msg 错误信息
		[[noreturn]]inline void die_with(const char_t*err_msg)noexcept{
			#if defined(_MSC_VER)
				__debugbreak();
			#endif
			push_and_disable_msvc_warning(
				26485//数组转型警告diss
				26475//强转警告diss
				26481//指针操作警告diss
				26429//nullness警告diss
			)
			if(err_msg){
				::std::mbstate_t stat{};
				if(::std::setlocale(LC_CTYPE,"en_US.utf8")==nullptr)
					die();
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				char err_msg_in_char[2048];
				char* err_msg_write = err_msg_in_char;
				size_t s;
				char_t c;
				pop_msvc_warning();
				while(assign(c,*err_msg++)){
					s = ::std::c32rtomb(err_msg_write, c, &stat);
					if(s == size_t(-1))
						die();
					err_msg_write += s;
				}
				*err_msg_write = '\0';
				#if defined(_WINMAIN_)
					wchar_t err_msg_in_wchar[2048];
					MultiByteToWideChar(CP_UTF8, 0, err_msg_in_char, -1, err_msg_in_wchar, 2048);
					::MessageBoxW(NULL,err_msg_in_wchar,NULL,MB_ICONERROR);
				#else
					::std::fputs("elc died because:\n",stderr);
					::std::fputs(err_msg_in_char,stderr);
					::std::fputc('\n',stderr);
					::std::fflush(stderr);
				#endif
			}
			::std::abort();
			pop_msvc_warning();
		}

		#include "../../_share/_undefs.hpp"
	}
	namespace elc::defs{
		using namespace elc::APIs::die;
	}
#endif

//file_end

