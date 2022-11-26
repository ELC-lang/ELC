//cast.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
push_and_disable_msvc_warning(
	26491//down_cast警告diss
	26466//down_cast警告diss
	26492//const_cast警告diss
);
/// 对值追加const
template<typename T>
force_inline constexpr const T add_const(T a)noexcept{return a;}
/// 对引用追加const
template<typename T>
force_inline constexpr const T&add_const(T&a)noexcept{return a;}
/// 对引用移除const
template<typename T>
force_inline constexpr T&remove_const(const T&a)noexcept{return const_cast<T&>(a);}
/// 对指针追加const
template<typename T>
force_inline constexpr const T*add_const(T*a)noexcept{return a;}
/// 对指针移除const
template<typename T>
force_inline constexpr T*remove_const(const T*a)noexcept{return const_cast<T*>(a);}
/// 对指针移除const
template<typename T>
force_inline constexpr T*launder_remove_const(const T*a)noexcept{return ::std::launder(remove_const(a));}
/// 向下转型至
template<typename T,typename U>
force_inline constexpr T down_cast(U a)noexcept{return static_cast<T>(a);}
pop_msvc_warning();

//file_end

