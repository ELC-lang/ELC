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
		base_handle_type;

		inline bool is_terminal_stream(base_handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				return GetFileType(handle)==FILE_TYPE_CHAR;
			#elif SYSTEM_TYPE == linux
				return isatty(handle);
			#endif
		}

		struct handle_type{
			base_handle_type _handle;
			bool _is_terminal;
			constexpr handle_type(base_handle_type handle)noexcept:_handle(handle),_is_terminal(handle&&is_terminal_stream(handle)){}
			constexpr handle_type()noexcept:handle_type(base_handle_type(0)){}
			constexpr handle_type(const handle_type&)=default;
			constexpr handle_type(handle_type&&)=default;
			constexpr handle_type&operator=(const handle_type&)=default;
			constexpr handle_type&operator=(handle_type&&)=default;
			constexpr operator base_handle_type()const noexcept{return _handle;}
			constexpr bool is_terminal()const noexcept{return _is_terminal;}
			explicit constexpr operator bool()const noexcept{return _handle;}
		};


		//定义基础类型
		//用配套的系统api可以从handle_type指代的文件流中读写的基础类型，越大越好
		typedef
		#if SYSTEM_TYPE == windows
			char16_t//windows下的wchar_t是utf-16，可以使用WriteConsoleW和ReadConsoleW从控制台的handle_type中读写
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

		//初始化流
		force_inline handle_type init_output_stream()noexcept{
			auto aret=
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_OUTPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDOUT_FILENO;
			#endif
			return aret;
		}
		force_inline handle_type init_input_stream()noexcept{
			auto aret=
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_INPUT_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDIN_FILENO;
			#endif
			return aret;
		}
		force_inline handle_type init_error_stream()noexcept{
			auto aret=
			#if SYSTEM_TYPE == windows
				GetStdHandle(STD_ERROR_HANDLE);
			#elif SYSTEM_TYPE == linux
				STDERR_FILENO;
			#endif
			return aret;
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
		template<class char_T>
		force_inline size_t basic_read_impl(handle_type handle, char_T* buffer, size_t size)noexcept{
			return basic_read_impl(handle, (void*)buffer, size*sizeof(char_T))/sizeof(char_T);
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
		template<class char_T>
		force_inline size_t basic_write_impl(handle_type handle, const char_T* buffer, size_t size)noexcept{
			return basic_write_impl(handle, (const void*)buffer, size*sizeof(char_T))/sizeof(char_T);
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

		//convert utf-32 text to char_T and write it to stream
		template<typename char_T,size_t code_convert_buf_size,size_t(*writter)(handle_type, const char_T*, size_t)noexcept>
		inline void base_write_text_with_code_convert_impl(handle_type stream,const char_t*buffer,size_t size)noexcept{
			if constexpr(type_info<char_T>!=type_info<char_t>){
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				ptrdiff_t s;
				char_T r[code_convert_buf_size];
				pop_msvc_warning();
				for(size_t i=0;i<size;i++){
					//convert it to char_T
					s=char_set::utf32_to_auto(buffer[i],r).processed_output().size();
					if(s == range_n::npos)
						die_with(locale::str::code_convert_error);
					//write it to stream
					writter(stream,r,s);
				}
			}else{
				//write it to stream
				writter(stream,buffer,size);
			}
		}

		inline void write_text_to_terminal_stream(handle_type stream,const char_t*buffer,size_t size)noexcept{
			if(!stream.is_terminal())
				base_write_text_with_code_convert_impl<char8_t,char_set::utf32_to_utf8_code_size,basic_write_impl>(stream,buffer,size);
			else
				base_write_text_with_code_convert_impl<base_input_char_type,code_convert_buf_size,basic_write_for_terminal_impl>(stream,buffer,size);
		}

		//read text from stream as char_T and convert it to utf-32
		template<typename char_T,size_t code_convert_buf_size,
				 size_t(*reader)(handle_type, char_T*, size_t)noexcept,
				 size_t(*writter)(handle_type, const char_T*, size_t)noexcept>
		inline size_t base_read_text_with_code_convert_impl(handle_type stream,char_t*buffer,size_t size,handle_type echo_stream)noexcept{
			size_t read_size=0;
			if constexpr(type_info<char_T>!=type_info<char_t>){
				push_and_disable_msvc_warning(26494);//未初始化警告diss
				size_t s;
				size_t v=0;
				char_T r[code_convert_buf_size+1];//+1是为了保证最后一个字符是0以方便编码转换函数判断
				pop_msvc_warning();
				size_t i=0;
				for(;i<size;i++){
					//read it from stream
					s=reader(stream,r+v,code_convert_buf_size-v);
					if(s == 0)
						break;
					r[s+v]=0;
					//convert it to utf-32
					const auto z=char_set::auto_to_utf32(buffer+i,r);
					if(!z)
						die_with(locale::str::code_convert_error);
					s=z.processed_output().size();
					if(echo_stream)
						writter(echo_stream,r,s);
					v=code_convert_buf_size-s;
					copy_assign[v](r,r+s);
				}
				read_size = i;
				//put back chars if v != 0
				if(v != 0)
					basic_seek_impl(stream,-(ptrdiff_t)v*sizeof(char_T),cur);
			}else{
				//read it from stream
				read_size=reader(stream,buffer,size);
				if(echo_stream)
					writter(echo_stream,buffer,read_size);
			}
			return read_size;
		}
		inline size_t read_text_from_terminal_stream(handle_type stream,char_t*buffer,size_t size,handle_type echo_stream)noexcept{
			if(!stream.is_terminal())
				return base_read_text_with_code_convert_impl<char8_t,char_set::utf32_to_utf8_code_size,
															 basic_read_impl,basic_write_impl>(stream,buffer,size,echo_stream);
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
			size_t read_size=base_read_text_with_code_convert_impl<base_input_char_type,code_convert_buf_size,
																   basic_read_for_terminal_impl,basic_write_for_terminal_impl>(stream,buffer,size,echo_stream);
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

