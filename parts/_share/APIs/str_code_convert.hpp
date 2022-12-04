//str_code_convert.hpp
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
#if !defined(ELC_APIS_str_code_convert)
	#define ELC_APIS_str_code_convert
	#include <cuchar>
	#include <clocale>
	#include "../default_data_type.hpp"
	#include "../../header_file/files/elc/string"
	namespace elc::APIs::str_code_convert{
		#include "../_defs.hpp"

		using namespace ::elc::defs;//get def

		[[nodiscard]]inline string_t<char> to_char_str(string_view_t<char_t> a)noexcept{
			string_t<char>aret;
			::std::mbstate_t stat{};
			push_and_disable_msvc_warning(26494)//未初始化警告diss
			char r[MB_LEN_MAX];
			ptrdiff_t s;
			pop_msvc_warning();
			for(auto c:a){
				s=::std::c32rtomb(r,c,&stat);
				if(s < 0)//-1
					die_with(locale::str::code_convert_error);
				aret+=string_view_t<char>{(char*)r,note::size((size_t)s)};
			}
			return aret;
		}
		[[nodiscard]]inline string_t<char_t> to_char_t_str(string_view_t<char> a)noexcept{
			string_t<char_t>aret;
			::std::mbstate_t stat{};
			push_and_disable_msvc_warning(26494);//未初始化警告diss
			char_t c;
			ptrdiff_t s;
			pop_msvc_warning();
			auto i=a.cbegin(),e=a.cend();
			while(i!=e){
				s=::std::mbrtoc32(&c,i,MB_LEN_MAX,&stat);
				if(s < 0)//-3 或 -1
					die_with(locale::str::code_convert_error);
				i+=s;
				aret+=c;
			}
			return aret;
		}

		#include "../_undefs.hpp"
	}
	namespace elc::defs{
		using namespace elc::APIs::str_code_convert;
	}
#endif

//file_end

