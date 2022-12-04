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

		distinctive inline struct streams_handles_t{
			handle_type output_stream;
			handle_type input_stream;
			handle_type error_stream;
		}streams_handles;

		force_inline handle_type init_output_stream()noexcept{
			#if SYSTEM_TYPE == windows
				streams_handles.output_stream = GetStdHandle(STD_OUTPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				streams_handles.output_stream = STDOUT_FILENO;
			#endif
			return streams_handles.output_stream;
		}
		force_inline handle_type init_input_stream()noexcept{
			#if SYSTEM_TYPE == windows
				streams_handles.input_stream = GetStdHandle(STD_INPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				streams_handles.input_stream = STDIN_FILENO;
			#endif
			return streams_handles.input_stream;
		}
		force_inline handle_type init_error_stream()noexcept{
			#if SYSTEM_TYPE == windows
				streams_handles.error_stream = GetStdHandle(STD_ERROR_HANDLE);
			#elif SYSTEM_TYPE == linux
				streams_handles.error_stream = STDERR_FILENO;
			#endif
			return streams_handles.error_stream;
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

		inline void write_text_to_stream(handle_type stream,const char_t*buffer,size_t size)noexcept{
			//convert utf-32 text to utf-8 in linux(utf-16 in windows) and write it to stream
			#if SYSTEM_TYPE == linux
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				ptrdiff_t s;
				char8_t r[char_set::utf32_to_utf8_code_size];
				pop_msvc_warning();
				for(size_t i=0;i<size;i++){
					//convert it to utf-8
					s=char_set::utf32_to_utf8(buffer[i],r);
					if(s == size_t(-1))
						die_with(locale::str::code_convert_error);
					//write it to stream
					basic_write_impl(stream,r,s*sizeof(char8_t));
				}
			#elif SYSTEM_TYPE == windows
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				char16_t r[char_set::utf32_to_utf16_code_size];
				pop_msvc_warning();
				for(size_t i=0;i<size;i++){
					//convert it to utf-16
					const auto s=char_set::utf32_to_utf16(buffer[i],r);
					if(!s)
						die_with(locale::str::code_convert_error);
					//write it to stream with WriteConsoleW
					DWORD write_count;
					WriteConsoleW(stream,r,(DWORD)s.processed_output().size(),&write_count,nullptr);
				}
			#endif
		}
		inline size_t read_text_from_stream(handle_type stream,char_t*buffer,size_t size)noexcept{
			//GetConsoleMode if LINE_INPUT_MODE
			#if SYSTEM_TYPE == windows
				DWORD ConsoleModeBackup;
				GetConsoleMode(stream, &ConsoleModeBackup);
				//disable line input mode
				SetConsoleMode(stream, ConsoleModeBackup & ~ENABLE_LINE_INPUT);
			#endif
			//read utf-8 text from stream in linux(utf-16 in windows) and convert it to utf-32
			size_t read_size=0;
			#if SYSTEM_TYPE == linux
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				size_t s;
				size_t v=0;
				char8_t r[char_set::utf32_to_utf8_code_size+1];
				pop_msvc_warning();
				size_t i=0;
				for(;i<size;i++){
					//read it from stream
					s=basic_read_impl(stream,r+v,sizeof(char8_t)*(char_set::utf32_to_utf8_code_size-v));
					if(s == 0)
						break;
					r[s+v]=0;
					//convert it to utf-32
					auto z=char_set::utf8_to_utf32(buffer+i,r,1);
					if(!z)
						die_with(locale::str::code_convert_error);
					s=z.processed_output().size();
					v=char_set::utf32_to_utf8_code_size-s;
					copy_assign[v](r,r+s);
				}
				read_size = i;
				//put back chars if v != 0
				if(v != 0)
					basic_seek_impl(stream,-(ptrdiff_t)v*sizeof(char8_t),cur);
			#elif SYSTEM_TYPE == windows
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				size_t s;
				size_t v=0;
				char16_t r[char_set::utf32_to_utf16_code_size+1];
				pop_msvc_warning();
				size_t i=0;
				for(;i<size;i++){
					//read it from stream with ReadConsoleW
					{
						DWORD read_count;
						ReadConsoleW(stream,r+v,(DWORD)(char_set::utf32_to_utf16_code_size-v),&read_count,nullptr);
						s=read_count;
					}
					if(s == 0)
						break;
					r[s+v]=0;
					//convert it to utf-32
					const auto z=char_set::utf16_to_utf32(buffer+i,r,1);
					if(!z)
						die_with(locale::str::code_convert_error);
					s=z.processed_output().size();
					v=char_set::utf32_to_utf16_code_size-s;
					copy_assign[v](r,r+s);
				}
				read_size = i;
				//put back chars if v != 0
				if(v != 0)
					basic_seek_impl(stream,-(ptrdiff_t)v*sizeof(char16_t),cur);
			#endif
			#if SYSTEM_TYPE == windows
				SetConsoleMode(stream, ConsoleModeBackup);
			#endif
			return read_size;
		}

		#include "../_undefs.hpp"
	}
#endif

//file_end

