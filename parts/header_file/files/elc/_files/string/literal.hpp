//literal.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace string_n{
	template<constexpr_str_n::constexpr_str_t_literal_helper str_helper>
	inline constexpr string operator""_elc_string()noexcept{
		return string(constexpr_str_n::operator""_constexpr_str<str_helper>());
	}
}
using string_n::operator""_elc_string;

//file_end

