//pointer.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
[[nodiscard]]constexpr auto pointer_to_bool(T*a)noexcept{
	if constexpr(was_not_an_ill_form(the_pointer_to_bool(a)))
		return the_pointer_to_bool(a);
	else
		return null_ptr!=a;
}
template<typename T>
[[nodiscard]]constexpr auto pointer_equal(T*a,T*b)noexcept{
	if constexpr(was_not_an_ill_form(the_pointer_equal(a,b)))
		return the_pointer_equal(a,b);
	else
		return a==b;
}
template<class T>
inline constexpr bool is_pointer= ::std::is_pointer_v<T>;
typedef void*pointer;
typedef const void*const_pointer;
typedef decltype(declvalue(byte*)-declvalue(byte*)) ptr_diff_t;
[[nodiscard]]constexpr ptr_diff_t get_off_set(note::from_t<const_pointer>a,note::to_t<const_pointer>b)noexcept{
	return cast_to_data(b())-cast_to_data(a());//b-a == c
}
template<class T>
[[nodiscard]]constexpr auto apply_off_set(T*a,ptr_diff_t c)noexcept{
	return cast_to_data(a)+c;//a+c == b
}
template<class T>
[[nodiscard]]constexpr auto unapply_off_set(T*b,ptr_diff_t c)noexcept{
	return cast_to_data(b)-c;//b-c == a
}

//file_end

