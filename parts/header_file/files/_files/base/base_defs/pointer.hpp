//pointer.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
[[nodiscard]]constexpr logical_bool pointer_to_bool(T*a)noexcept{
	return null_ptr!=a;
}
template<typename T>
[[nodiscard]]constexpr logical_bool pointer_equal(T*a,T*b)noexcept{
	return a==b;
}
template<class T>
inline constexpr bool is_pointer= ::std::is_pointer_v<T>;
typedef void*pointer;
typedef const void*const_pointer;

//file_end

