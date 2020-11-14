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
	#include "../default_data_type.hpp"
	#include "../../header_file/files/string"
	#define ELC_APIS_str_code_convert
	namespace elc::APIs::str_code_convert{
		using namespace ::elc::defs;//get def
		//UF
		inline str_code_convert_ret_t<char> to_char_str(const char_t* a)
		inline str_code_convert_ret_t<char_t> to_char_t_str(const char* a)

		inline string_t<char> to_char_str(const string_t<char_t> a){
			string_t<char>ret;
			::std::mbstate_t stat{};
			if(::std::setlocale(LC_ALL,"en_US.utf8")==nullptr)
				die_with(::elc::locale::str::setlocale_error);
			char r[MB_LEN_MAX];
			size_t s;
			for(auto c:a){
				s=::std::c32rtomb(r,c,&stat);
				if(s < 0)//-1
					die_with(::elc::locale::str::code_convert_error);
				ret+=range_t<char*>{(char*)r,s};
			}
			::std::setlocale(LC_ALL,"C");
			return ret;
		}
		inline string_t<char_t> to_char_t_str(const string_t<char> a){
			string_t<char_t>ret;
			::std::mbstate_t stat{};
			if(::std::setlocale(LC_ALL,"en_US.utf8")==nullptr)
				die_with(::elc::locale::str::setlocale_error);
			char_t c;
			size_t s;
			auto i=a.cbegin(),e=a.cend();
			while(i!=e){
				s=::std::mbrtoc32(&c,i,MB_LEN_MAX,&stat);
				if(s < 0)//-3 或 -1
					die_with(::elc::locale::str::code_convert_error);
				i+=s;
				ret+=c;
			}
			::std::setlocale(LC_ALL,"C");
			return ret;
		}
	}
	namespace elc::defs{
		using namespace elc::APIs::str_code_convert;
	}
#endif

//file_end

