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
	#include "../default_data_type.hpp"
	#include "./char_set.hpp"
	#include "../../header_file/files/elc/string"
	#include "../../header_file/files/elc/base_stream"
	#include "_tools/decl_system_type.hpp"
	#if SYSTEM_TYPE == linux
		#include <unistd.h>
		#include <termios.h>
	#elif SYSTEM_TYPE == windows
		#define NOMINMAX
		#include <Windows.h>
		#include <ConsoleApi.h>
	#else
		#error ERROR_MSG_UNABLE_OS
	#endif
	namespace elc::APIs::streams{
		#include "../_defs.hpp"

		using namespace ::elc::defs;//get def

		typedef
		#if SYSTEM_TYPE == windows
			HANDLE
		#elif SYSTEM_TYPE == linux
			int
		#endif
		handle_type;

		//定义基础类型
		//用配套的系统api可以从handle_type指代的控制台中读写的基础类型，越大越好
		typedef
		#if SYSTEM_TYPE == windows
			wchar_t//windows下的wchar_t是utf-16，可以使用WriteConsoleW和ReadConsoleW从控制台的handle_type中读写
		#elif SYSTEM_TYPE == linux
			char//linux下的char是utf-8，可以使用write和read从控制台的handle_type中读写
		#endif
		base_input_char_type;

		constexpr size_t code_convert_buf_size=
		#if SYSTEM_TYPE == windows
			char_set::utf32_to_utf16_code_size
		#elif SYSTEM_TYPE == linux
			char_set::utf32_to_utf8_code_size
		#endif
		;

		//编码转换
		//code_convert_impl
		inline auto code_convert_to_input_char_t_impl(base_input_char_type* out, const char32_t* in, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				return char_set::utf32_to_utf16((char16_t*)out, in, size);
			#elif SYSTEM_TYPE == linux
				return char_set::utf32_to_utf8(out, in, size);
			#endif
		}
		inline auto code_convert_to_char_t_impl(char32_t* out, const base_input_char_type* in, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				return char_set::utf16_to_utf32(out, (const char16_t*)in, size);
			#elif SYSTEM_TYPE == linux
				return char_set::utf8_to_utf32(out, in, size);
			#endif
		}
		//单个字符
		inline auto code_convert_to_input_char_t_impl(char32_t in, base_input_char_type* out)noexcept{
			#if SYSTEM_TYPE == windows
				return char_set::utf32_to_utf16(in, (char16_t*)out);
			#elif SYSTEM_TYPE == linux
				return char_set::utf32_to_utf8(in, out);
			#endif
		}
		inline void code_convert_to_char_t_impl(base_input_char_type in, char32_t* out)noexcept=delete;

		//初始化流
		force_inline handle_type init_output_stream()noexcept{
			return
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_OUTPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDOUT_FILENO;
			#endif
		}
		force_inline handle_type init_input_stream()noexcept{
			return
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_INPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDIN_FILENO;
			#endif
		}
		force_inline handle_type init_error_stream()noexcept{
			return
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_ERROR_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDERR_FILENO;
			#endif
		}

		//basic_read_impl
		inline size_t basic_read_impl(handle_type handle, void* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD read_count;
				push_and_disable_msvc_warning(6031);//返回值忽略警告diss
				ReadFile(handle, buffer,(DWORD)size, &read_count, nullptr);
				pop_msvc_warning();
				return read_count;
			#elif SYSTEM_TYPE == linux
				return read(handle, buffer, size);
			#endif
		}
		//basic_write_impl
		inline size_t basic_write_impl(handle_type handle, const void* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD write_count;
				WriteFile(handle, buffer,(DWORD)size, &write_count, nullptr);
				return write_count;
			#elif SYSTEM_TYPE == linux
				return write(handle, buffer, size);
			#endif
		}
		//由于windows的WriteFile不能正确处理终端io，特化终端io所需的impl of read/write
		//basic_read_for_terminal_impl
		inline size_t basic_read_for_terminal_impl(handle_type handle, base_input_char_type* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD read_count;
				push_and_disable_msvc_warning(6031);//返回值忽略警告diss
				ReadConsoleW(handle, buffer,(DWORD)size, &read_count, nullptr);
				pop_msvc_warning();
				return read_count;
			#elif SYSTEM_TYPE == linux
				return read(handle, buffer, size*sizeof(base_input_char_type))/sizeof(base_input_char_type);
			#endif
		}
		//basic_write_for_terminal_impl
		inline size_t basic_write_for_terminal_impl(handle_type handle, const base_input_char_type* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD write_count;
				WriteConsoleW(handle, buffer,(DWORD)size, &write_count, nullptr);
				return write_count;
			#elif SYSTEM_TYPE == linux
				return write(handle, buffer, size*sizeof(base_input_char_type))/sizeof(base_input_char_type);
			#endif
		}
		//basic_flush_impl
		inline void basic_flush_impl(handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				FlushFileBuffers(handle);
			#elif SYSTEM_TYPE == linux
				fsync(handle);
			#endif
		}
		//basic_sync_impl
		inline void basic_sync_impl(handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				FlushFileBuffers(handle);
			#elif SYSTEM_TYPE == linux
				fsync(handle);
			#endif
		}
		//basic_close_impl
		inline void basic_close_impl(handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				CloseHandle(handle);
			#elif SYSTEM_TYPE == linux
				close(handle);
			#endif
		}
		//basic_tell_impl
		inline int_t basic_tell_impl(handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				return SetFilePointer(handle, 0, nullptr, FILE_CURRENT);
			#elif SYSTEM_TYPE == linux
				return lseek(handle, 0, SEEK_CUR);
			#endif
		}
		//basic_seek_impl
		inline int_t basic_seek_impl(handle_type handle,int_t offset,stream_n::seek_type type)noexcept{
			auto under_type =
			#if SYSTEM_TYPE == windows
				FILE_BEGIN
			#elif SYSTEM_TYPE == linux
				SEEK_SET
			#endif
			;
			using namespace stream_n;
			switch(type){//{beg,end,cur};
				case beg:
					#if SYSTEM_TYPE == windows
						under_type = FILE_BEGIN;
					#elif SYSTEM_TYPE == linux
						under_type = SEEK_SET;
					#endif
					break;
				case end:
					#if SYSTEM_TYPE == windows
						under_type = FILE_END;
					#elif SYSTEM_TYPE == linux
						under_type = SEEK_END;
					#endif
					break;
				case cur:
					#if SYSTEM_TYPE == windows
						under_type = FILE_CURRENT;
					#elif SYSTEM_TYPE == linux
						under_type = SEEK_CUR;
					#endif
					break;
			}
			#if SYSTEM_TYPE == windows
				SetFilePointer(handle,(LONG)offset, nullptr, under_type);
				return basic_tell_impl(handle);
			#elif SYSTEM_TYPE == linux
				return lseek(handle, offset, under_type);
			#endif
		}

		inline void write_text_to_terminal_stream(handle_type stream,const char_t*buffer,size_t size)noexcept{
			//convert utf-32 text to base_input_char_type and write it to stream
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			ptrdiff_t s;
			base_input_char_type r[code_convert_buf_size];
			pop_msvc_warning();
			for(size_t i=0;i<size;i++){
				//convert it to base_input_char_type
				s=code_convert_to_input_char_t_impl(buffer[i],r).processed_output().size();
				if(s == range_n::npos)
					die_with(locale::str::code_convert_error);
				//write it to stream
				basic_write_for_terminal_impl(stream,r,s*sizeof(char8_t));
			}
		}
		inline size_t read_text_from_terminal_stream(handle_type stream,char_t*buffer,size_t size,handle_type echo_stream)noexcept{
			//GetConsoleMode if LINE_INPUT_MODE
			#if SYSTEM_TYPE == windows
				DWORD ConsoleModeBackup;
				GetConsoleMode(stream, &ConsoleModeBackup);
				//disable line input mode
				SetConsoleMode(stream, ConsoleModeBackup & ~ENABLE_LINE_INPUT);
			#elif SYSTEM_TYPE == linux
				//no echo and disable line input mode
				termios oldt;
				tcgetattr(stream, &oldt);
				{
					auto flgbak = oldt.c_lflag;
					oldt.c_lflag &= ~(ECHO | ICANON);
					tcsetattr(stream, TCSANOW, &oldt);
					oldt.c_lflag = flgbak;
				}
			#endif
			//read base_input_char_type and convert it to utf-32
			size_t read_size=0;
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			size_t s;
			size_t v=0;
			base_input_char_type r[code_convert_buf_size+1];//+1是为了保证最后一个字符是0以方便编码转换函数判断
			pop_msvc_warning();
			size_t i=0;
			for(;i<size;i++){
				//read it from stream
				s=basic_read_for_terminal_impl(stream,r+v,sizeof(char8_t)*(code_convert_buf_size-v));
				if(s == 0)
					break;
				r[s+v]=0;
				//convert it to utf-32
				const auto z=code_convert_to_char_t_impl(buffer+i,r,1);
				if(!z)
					die_with(locale::str::code_convert_error);
				s=z.processed_output().size();
				if(echo_stream)
					basic_write_for_terminal_impl(echo_stream,r,s);
				v=code_convert_buf_size-s;
				copy_assign[v](r,r+s);
			}
			read_size = i;
			//put back chars if v != 0
			if(v != 0)
				basic_seek_impl(stream,-(ptrdiff_t)v*sizeof(char8_t),cur);
			#if SYSTEM_TYPE == windows
				SetConsoleMode(stream, ConsoleModeBackup);
			#elif SYSTEM_TYPE == linux
				tcsetattr(stream, TCSANOW, &oldt);
			#endif
			return read_size;
		}

		#include "../_undefs.hpp"
	}
#endif

//file_end

