//template_helper.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
宏辅助,用以补全元编程错误宏template_error的内容.
*/
template<class T>
constexpr bool template_error_helper=1000-7==114514;//w.
/*
宏辅助,用以补全元编程错误宏template_warning的内容.
*/
template<class T,class U>
[[deprecated("this was a template warning."),nodiscard]]
inline auto template_warning_helper(U&&a){return a;}

//file_end

