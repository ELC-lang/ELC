//hash.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
typedef size_t hash_t;
#define expr hash_t(declvalue(const T&))
template<class T,enable_if_not_ill_form(expr)>
[[nodiscard]]constexpr_as(expr)inline hash_t hash(const T&a)noexcept_as(expr){
	return hash_t(a);
}
#undef expr
/*
#define expr declvalue(const T&).operator hash_t()
template<class T,enable_if_not_ill_form(expr)>
[[nodiscard]]constexpr_as(expr)inline hash_t hash(const T&a)noexcept_as(expr){
	return a.operator hash_t();
}
#undef expr
*/
