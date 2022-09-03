//paths.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if !defined(ELC_PATHS)
	#define ELC_PATHS
	#include "../../header_file/files/elc/base_defs"
	#include "../../header_file/files/elc/string"
	#include "APIs/str_code_convert.hpp"
	//
	#include "APIs/_tools/decl_system_type.hpp"
	//
	#if SYSTEM_TYPE == windows
		#include "shlobj_core.h"
	#elif SYSTEM_TYPE == linux
		#include <unistd.h>
	#else
		#include <cstdio>
	#endif
	//
	#include "_defs.hpp"
	namespace elc::paths{
		using namespace elc::defs;
		namespace defs{
			#if SYSTEM_TYPE == windows
				#define DYNAMIC_LIBRARY_SUFFIX ".dll"
			#else
				#define DYNAMIC_LIBRARY_SUFFIX ".so"
			#endif

			[[nodiscard]]inline bool file_exists(const string&name)noexcept{
				#if SYSTEM_TYPE == windows
					return GetFileAttributesA(to_char_str(name).c_str())!=INVALID_FILE_ATTRIBUTES;
				#elif SYSTEM_TYPE == linux
					return access(to_char_str(name).c_str(),F_OK)==0;
				#else
					auto fp = ::std::fopen(to_char_str(name).c_str(), "rb");
					if(fp){
						::std::fclose(fp);
						return true;
					}
					return false;
				#endif
			}
			[[nodiscard]]inline string get_program_data_path()noexcept{
				#if SYSTEM_TYPE == windows
					char path[MAX_PATH];
					if(!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)))
						return string{};
					return to_char_t_str(path);
				#elif SYSTEM_TYPE == linux
					return es"/usr/local/share"_constexpr_str;
				#else
					return string{};
				#endif
			}
			[[nodiscard]]inline string get_system_drive()noexcept{
				#if SYSTEM_TYPE == windows
					char path[MAX_PATH];
					if(!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_WINDOWS, NULL, 0, path)))
						return string{};
					return to_char_t_str(path)+es"\\.."_constexpr_str;
				#elif SYSTEM_TYPE == linux
					return es"/"_constexpr_str;
				#else
					return string{};
				#endif
			}
			[[nodiscard]]inline string get_elc_runtime_lib_path()noexcept{
				#if SYSTEM_TYPE == windows
					//windows: if can read file ProgramData/elc/flag , use ProgramData/elc
					//else try to read SystemDrive/elc/flag , use ProgramData/elc
					//else return "./elc"
					{
						string program_data_path=get_program_data_path();
						if(program_data_path.size() && file_exists(program_data_path + es"/elc/flag"_constexpr_str))
							return program_data_path+es"/elc"_constexpr_str;
					}
					{
						string system_drive=get_system_drive();
						if(system_drive.size() && file_exists(system_drive + es"/elc/flag"_constexpr_str))
							return system_drive+es"/elc"_constexpr_str;
					}
				#elif SYSTEM_TYPE == linux
					//linux: if can read file /usr/lib/elc/flag , use /usr/lib/elc
					//else return "./elc"
					string flag_path=es"/usr/lib/elc/flag"_constexpr_str;
					if(file_exists(flag_path))
						return es"/usr/lib/elc"_constexpr_str;
				#endif
				return es"./elc"_constexpr_str;
			}
		}
		inline constexpr auto package_symbol_loader_file_name = es"package_symbol_loader" DYNAMIC_LIBRARY_SUFFIX ""_constexpr_str;

		#if defined(_MSC_VER)
			#pragma warning(push)
			#pragma warning(disable:26426)//动态初始化警告diss
		#endif
		distinctive inline string elc_runtime_lib_path=defs::get_elc_runtime_lib_path();
		distinctive inline string package_symbol_loader_path = elc_runtime_lib_path+es"/"_constexpr_str+package_symbol_loader_file_name;
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif

		#undef DYNAMIC_LIBRARY_SUFFIX
	}
	#include "_undefs.hpp"
	//
	#include "APIs/_tools/undef_decl_system_type.hpp"
#endif

//file_end

