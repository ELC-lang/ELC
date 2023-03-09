//streams.hpp
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
#if !defined(ELC_APIS_streams)
	#define ELC_APIS_streams
	#include <cuchar>
	#include <clocale>
	#include <cstdio>
	#include "../default_data_type.hpp"
	#include "../char_set/_body.hpp"
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

		push_and_disable_msvc_warning(26475);//强制转换警告diss
		inline constexpr auto zero_base_handle = base_handle_type(0);
		pop_msvc_warning();

		inline bool is_terminal_stream(base_handle_type handle)noexcept{
			#if SYSTEM_TYPE == windows
				return GetFileType(handle)==FILE_TYPE_CHAR;
			#elif SYSTEM_TYPE == linux
				return isatty(handle);
			#endif
		}

		struct handle_type{
			base_handle_type _handle;
			constexpr handle_type(base_handle_type handle)noexcept:_handle(handle){}
			constexpr handle_type()noexcept:handle_type(zero_base_handle){}
			constexpr handle_type(const handle_type&)=default;
			constexpr handle_type(handle_type&&)=default;
			constexpr handle_type&operator=(const handle_type&)=default;
			constexpr handle_type&operator=(handle_type&&)=default;
			constexpr operator base_handle_type()const noexcept{return _handle;}
			inline bool is_terminal()const noexcept{return is_terminal_stream(_handle);}
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
		base_io_char_type;

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

		//判断handle是否eof
		inline bool is_eof(handle_type stream)noexcept{
			#if SYSTEM_TYPE == windows
				push_and_disable_msvc_warning(6031);//返回值忽略警告diss
				ReadFile(stream, nullptr, 0, nullptr, nullptr);
				pop_msvc_warning();
				return GetLastError() == ERROR_HANDLE_EOF;
			#elif SYSTEM_TYPE == linux
				return feof(stream);
			#endif
		}
		//判断handle是否等待输入并没有待处理的输入
		inline bool is_wait_input(handle_type stream)noexcept{
			#if SYSTEM_TYPE == windows
				if(stream.is_terminal())
					return WaitForSingleObject(stream, 0) == WAIT_TIMEOUT;
				else
					return false;
			#elif SYSTEM_TYPE == linux
				return false;
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
		inline size_t basic_read_for_terminal_impl(handle_type handle, base_io_char_type* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD read_count;
				push_and_disable_msvc_warning(6031);//返回值忽略警告diss
				ReadConsoleW(handle, buffer,(DWORD)size, &read_count, nullptr);
				pop_msvc_warning();
				return read_count;
			#elif SYSTEM_TYPE == linux
				return read(handle, buffer, size*sizeof(base_io_char_type))/sizeof(base_io_char_type);
			#endif
		}
		//basic_write_for_terminal_impl
		inline size_t basic_write_for_terminal_impl(handle_type handle, const base_io_char_type* buffer, size_t size)noexcept{
			#if SYSTEM_TYPE == windows
				DWORD write_count;
				WriteConsoleW(handle, buffer,(DWORD)size, &write_count, nullptr);
				return write_count;
			#elif SYSTEM_TYPE == linux
				return write(handle, buffer, size*sizeof(base_io_char_type))/sizeof(base_io_char_type);
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
		//before_terminal_read
		force_inline auto before_terminal_read(handle_type stream)noexcept{
			//GetConsoleMode if LINE_INPUT_MODE
			#if SYSTEM_TYPE == windows
				DWORD ConsoleModeBackup;
				GetConsoleMode(stream, &ConsoleModeBackup);
				//disable line input mode
				SetConsoleMode(stream, ConsoleModeBackup & ~ENABLE_LINE_INPUT);
				return ConsoleModeBackup;
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
				return oldt;
			#endif
		}
		//after_terminal_read
		force_inline void after_terminal_read(handle_type stream,auto something)noexcept{
			#if SYSTEM_TYPE == windows
				SetConsoleMode(stream, something);
			#elif SYSTEM_TYPE == linux
				tcsetattr(stream, TCSANOW, &something);
			#endif
		}
		//before_terminal_write
		force_inline auto before_terminal_write(handle_type stream)noexcept{
			//GetConsoleMode if LINE_INPUT_MODE
			#if SYSTEM_TYPE == windows
				DWORD ConsoleModeBackup;
				GetConsoleMode(stream, &ConsoleModeBackup);
				//enable ENABLE_PROCESSED_OUTPUT and ENABLE_WRAP_AT_EOL_OUTPUT ENABLE_VIRTUAL_TERMINAL_PROCESSING
				SetConsoleMode(stream, ConsoleModeBackup | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
				return ConsoleModeBackup;
			#elif SYSTEM_TYPE == linux
				//nothing to do
				return 0;
			#endif
		}
		//after_terminal_write
		force_inline void after_terminal_write(handle_type stream,auto something)noexcept{
			#if SYSTEM_TYPE == windows
				SetConsoleMode(stream, something);
			#elif SYSTEM_TYPE == linux
				//nothing to do
			#endif
		}

		#include "../_undefs.hpp"
	}
	//
	#include "_tools/undef_decl_system_type.hpp"
#endif

//file_end

