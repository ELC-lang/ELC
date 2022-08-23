//other.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class empty_type{};

template<typename T,typename U>
inline auto&assign(T&&a,U&&b)noexcept_as(a=b){//为绕过条件内赋值时の警告而使用
	a=b;
	return a;
}

template<typename T>
inline void discard(T&&){}//fucking nodiscard.
template<typename T>
inline void just_a_use(T&&){}

template<typename T>
distinctive inline T const_default_value_of{};//?

enable_adl(the_destroy);
template<typename T>
static void destroy(T*a)noexcept{
	if constexpr(was_not_an_ill_form(the_destroy(a)))
		the_destroy(a);
	else
		template_error("Please overload the function the_destroy in the namespace where this type is defined.");
}

//file_end

