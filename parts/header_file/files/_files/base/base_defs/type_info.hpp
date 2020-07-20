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
	static constexpr bool base_on=::std::is_convertible_v<T*,T_*>;
	template<class T_>
	static constexpr bool not_base_on=!base_on<T_>;
	
	template<template<class>class attribute_name>
	static constexpr bool has_attribute=base_on<attribute_name<T>>;
	template<template<class>class attribute_name>
	static constexpr bool not_has_attribute=!has_attribute<attribute_name>;
	template<class attribute_name>
	static constexpr bool has_attribute=::std::is_base_of_v<attribute_name,T>;//防止因子类属性误判，不可用base_on
	template<class attribute_name>
	static constexpr bool not_has_attribute=!has_attribute<attribute_name>;
	
	template<class T_>
	static constexpr bool can_convert_to=::std::is_convertible_v<T,T_>;
	template<class T_>
	static constexpr bool can_t_convert_to=!can_convert_to<T_>;
	template<class T_>
	static constexpr bool can_nothrow_convert_to=::std::is_convertible_v<T,T_>;
	template<class T_>
	static constexpr bool can_t_nothrow_convert_to=!can_convert_to<T_>;
};
template<class T>
constexpr type_info_t<T> type_info{};
constexpr bool operator==(base_type_info_t&a,base_type_info_t&b){return &a==&b;}
constexpr bool operator!=(base_type_info_t&a,base_type_info_t&b){return!(a==b);}
