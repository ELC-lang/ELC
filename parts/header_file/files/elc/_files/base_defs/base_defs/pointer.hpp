//pointer.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

//指针转bool的重载接口，允许类对此进行自定义
//elc内部的指针转bool应当视情况使用默认配置或此重载接口
template<typename T>
[[nodiscard]]constexpr auto pointer_to_bool(T*a)noexcept{
	if constexpr(was_not_an_ill_form(the_pointer_to_bool(a)))
		return the_pointer_to_bool(a);
	else
		return null_ptr!=a;
}
//指针比较的重载接口，允许类对此进行自定义
//elc内部的指针比较应当视情况使用默认配置或此重载接口
template<typename T>
[[nodiscard]]constexpr auto pointer_equal(T*a,T*b)noexcept{
	if constexpr(was_not_an_ill_form(the_pointer_equal(a,b)))
		return the_pointer_equal(a,b);
	else
		return a==b;
}
//判断类型是否为指针
template<class T>
inline constexpr bool is_pointer= ::std::is_pointer_v<T>;
//未知类型指针
typedef void*pointer;
//未知类型指针，不可写
typedef const void*const_pointer;

//获取两个指针的偏移
[[nodiscard]]constexpr ptrdiff_t get_off_set(note::from_t<const_pointer>a,note::to_t<const_pointer>b)noexcept{
	return cast_to_data(b())-cast_to_data(a());//b-a == c
}
//应用偏移
template<class T>
[[nodiscard]]constexpr auto apply_off_set(T*a,ptrdiff_t c)noexcept{
	return cast_to_data(a)+c;//a+c == b
}
//反向应用偏移
template<class T>
[[nodiscard]]constexpr auto unapply_off_set(T*b,ptrdiff_t c)noexcept{
	return cast_to_data(b)-c;//b-c == a
}

//file_end

