//_char.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs::locale{
	inline const ::std::locale default_locale{"en_US.utf8"};
	namespace char_n{
		template <class charT> bool isspace (charT c){ return isspace (c,default_locale); }
		template <class charT> bool isprint (charT c){ return isprint (c,default_locale); }
		template <class charT> bool iscntrl (charT c){ return iscntrl (c,default_locale); }
		template <class charT> bool isupper (charT c){ return isupper (c,default_locale); }
		template <class charT> bool islower (charT c){ return islower (c,default_locale); }
		template <class charT> bool isalpha (charT c){ return isalpha (c,default_locale); }
		template <class charT> bool isdigit (charT c){ return isdigit (c,default_locale); }
		template <class charT> bool ispunct (charT c){ return ispunct (c,default_locale); }
		template <class charT> bool isxdigit(charT c){ return isxdigit(c,default_locale); }
		template <class charT> bool isalnum (charT c){ return isalnum (c,default_locale); }
		template <class charT> bool isgraph (charT c){ return isgraph (c,default_locale); }
		template <class charT> charT toupper(charT c){ return toupper (c,default_locale); }
		template <class charT> charT tolower(charT c){ return tolower (c,default_locale); }
	}
}

//file_end

