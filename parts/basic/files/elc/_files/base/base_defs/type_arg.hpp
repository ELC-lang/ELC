//type_arg.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
struct type_arg_t{typedef T type;};
template<class T>
constexpr type_arg_t<T> type_arg{};
