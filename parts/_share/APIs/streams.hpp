//streams.hpp
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
#if !defined(ELC_APIS_streams)
	#define ELC_APIS_streams
	#include <cuchar>
	#include <clocale>
	#include <cstdio>
	#include "_tools/decl_system_type.hpp"
	#include "../default_data_type.hpp"
	#include "../../header_file/files/elc/string"
	#if SYSTEM_TYPE == windows
		#define NOMINMAX
		#include <Windows.h>
	#endif
	namespace elc::APIs::streams{
		#include "../_defs.hpp"

		using namespace ::elc::defs;//get def

		force_inline void init_output_stream()noexcept{
			#if SYSTEM_TYPE == windows
				// Set console code page to UTF-8 so console known how to interpret string data
				SetConsoleOutputCP(CP_UTF8);
				// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
				setvbuf(stdout, nullptr, _IOFBF, 1000);
			#endif
		}
		force_inline void init_input_stream()noexcept{
			#if SYSTEM_TYPE == windows
				// Set console code page to UTF-8 so console known how to interpret string data
				SetConsoleCP(CP_UTF8);
				// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
				setvbuf(stdin, nullptr, _IOFBF, 1000);
			#endif
		}
		force_inline void init_error_stream()noexcept{
			#if SYSTEM_TYPE == windows
				// Set console code page to UTF-8 so console known how to interpret string data
				SetConsoleOutputCP(CP_UTF8);
				// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
				setvbuf(stderr, nullptr, _IOFBF, 1000);
			#endif
		}

		inline size_t read_text_from_stream(FILE*stream,char_t*buffer,size_t size)noexcept{
			//read utf-8 text from stream and convert it to utf-32
			::std::mbstate_t stat{};
			string_t old_lc=::std::setlocale(LC_CTYPE,nullptr);
			if(::std::setlocale(LC_CTYPE,"en_US.utf8")==nullptr)
				die_with(locale::str::setlocale_error);
			ptrdiff_t s=0;
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			char r[MB_LEN_MAX];
			pop_msvc_warning();
			for(size_t i=0;i<size;i++){
				//read a char
				s=::std::fread(r+s,1,MB_LEN_MAX-s,stream);
				if(s==0)
					return i;
				//convert it to utf-32
				s=::std::mbrtoc32(&buffer[i],r,MB_LEN_MAX,&stat);
				if(s < 0)//-3 或 -1
					die_with(locale::str::code_convert_error);
				//move the other char that not handled in 'r'
				copy_assign[size_t(s)](r,r+size_t(s));
			}
			::std::setlocale(LC_CTYPE,old_lc.c_str());
			return size;
		}
		inline void write_text_to_stream(FILE*stream,const char_t*buffer,size_t size)noexcept{
			//convert utf-32 text to utf-8 and write it to stream
			::std::mbstate_t stat{};
			string_t old_lc=::std::setlocale(LC_CTYPE,nullptr);
			if(::std::setlocale(LC_CTYPE,"en_US.utf8")==nullptr)
				die_with(locale::str::setlocale_error);
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			ptrdiff_t s;
			char r[MB_LEN_MAX];
			pop_msvc_warning();
			for(size_t i=0;i<size;i++){
				//convert it to utf-8
				s=::std::c32rtomb(r,buffer[i],&stat);
				if(s < 0)//-1
					die_with(locale::str::code_convert_error);
				//write it to stream
				if(::std::fwrite(r,1,size_t(s),stream)!=size_t(s))
					return;
			}
			::std::setlocale(LC_CTYPE,old_lc.c_str());
		}

		#include "../_undefs.hpp"
	}
#endif

//file_end

