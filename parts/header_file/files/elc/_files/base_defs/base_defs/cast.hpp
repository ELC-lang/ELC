//cast.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:26491)//down_cast警告diss
	#pragma warning(disable:26466)//down_cast警告diss
	#pragma warning(disable:26492)//const_cast警告diss
#endif
template<typename T>
inline constexpr const T add_const(T a)noexcept{return a;}
template<typename T>
inline constexpr const T&add_const(T&a)noexcept{return a;}
template<typename T>
inline constexpr T remove_const(const T a)noexcept{return a;}
template<typename T>
inline constexpr const T*add_const(T*a)noexcept{return a;}
template<typename T>
inline constexpr T*remove_const(const T*a)noexcept{return const_cast<T*>(a);}
template<typename T,typename U>
inline constexpr T down_cast(U a)noexcept{return static_cast<T>(a);}
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

//file_end

