//type_info.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct base_type_info_t{};
template<class T>
struct type_info_t:base_type_info_t{
	typedef T type;
	template<class T_>
	static constexpr bool base_on=::std::is_convertible_v<remove_cvref<T>*,remove_cvref<T_>*>;
	template<class T_>
	static constexpr bool not_base_on=!base_on<T_>;

	template<class attribute_name>
	static constexpr bool has_attribute_helper(){
		return ::std::is_base_of_v<attribute_name,remove_cvref<T>>;//防止因子类属性误判，不可用base_on
	}
	template<template<class>class attribute_name>
	static constexpr bool has_attribute_helper(){
		return has_attribute_helper<attribute_name<remove_cvref<T>>>();
	}
	/*
	//没有重载变量模板一说
	template<typename attribute_name>
	static constexpr bool has_attribute=has_attribute_helper<attribute_name>();
	template<typename attribute_name>
	static constexpr bool not_has_attribute=!has_attribute(attribute_name);
	*/
	template<class attribute_name>
	static constexpr bool not_has_has_attribute_helper(){
		return!has_attribute_helper<attribute_name>();
	}
	template<template<class>class attribute_name>
	static constexpr bool not_has_has_attribute_helper(){
		return!has_attribute_helper<attribute_name>();
	}
	// defed at defs.
	// #define has_attribute(...) has_attribute_helper<__VA_ARGS__>()
	// #define not_has_attribute(...) not_has_has_attribute_helper<__VA_ARGS__>()

	template<class T_>
	static constexpr bool can_convert_to=::std::is_convertible_v<T,T_>;
	template<class T_>
	static constexpr bool can_t_convert_to=!can_convert_to<T_>;
	template<class T_>
	static constexpr bool can_nothrow_convert_to=::std::is_nothrow_convertible_v<T,T_>;
	template<class T_>
	static constexpr bool can_t_nothrow_convert_to=!can_nothrow_convert_to<T_>;
};

template<class T>
constexpr type_info_t<T>type_info{};

[[nodiscard]]constexpr bool operator==(const base_type_info_t&a,const base_type_info_t&b){return &a==&b;}
[[nodiscard]]constexpr bool operator!=(const base_type_info_t&a,const base_type_info_t&b){return!(a==b);}

//file_end

