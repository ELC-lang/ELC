//friend_concepts.hpp
//at namespace elc::defs::big_number_types
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//重新定义算数类型相关概念以添加对big_number_types的支持
template<typename T>
constexpr bool is_big_number_type=(
	type_info<remove_cvref<T>> == type_info<ubigint> ||
	type_info<remove_cvref<T>> == type_info<bigint> ||
	type_info<remove_cvref<T>> == type_info<ubigfloat> ||
	type_info<remove_cvref<T>> == type_info<bigfloat>
);
template<typename T>
concept big_number_type=is_big_number_type<T>;

template<typename T>
constexpr bool is_integer_type= ::std::is_integral_v<T> || (
	type_info<remove_cvref<T>> == type_info<ubigint> ||
	type_info<remove_cvref<T>> == type_info<bigint>
);
template<typename T>
concept integer_type=is_integer_type<T>;

template<typename T>
constexpr bool is_unsigned= ::std::is_unsigned_v<T> || (
	type_info<remove_cvref<T>> == type_info<ubigint> ||
	type_info<remove_cvref<T>> == type_info<ubigfloat>
);
template<typename T>
concept unsigned_type=is_unsigned<T>;

template<typename T>
constexpr bool is_signed= ::std::is_signed_v<T> || (
	type_info<remove_cvref<T>> == type_info<bigint> ||
	type_info<remove_cvref<T>> == type_info<bigfloat>
);
template<typename T>
concept signed_type=is_signed<T>;

template<typename T>
constexpr bool is_floating_point= ::std::is_floating_point_v<T> || (
	type_info<remove_cvref<T>> == type_info<ubigfloat> ||
	type_info<remove_cvref<T>> == type_info<bigfloat>
);
template<typename T>
concept floating_point_type=is_floating_point<T>;

template<typename T>
constexpr bool is_arithmetic= ::std::is_arithmetic_v<remove_cvref<T>> || is_big_number_type<T>;
template<typename T>
concept arithmetic_type=is_arithmetic<T>;

template<typename T>
constexpr bool is_unsigned_integer= ::std::is_unsigned_v<T> || (
	type_info<remove_cvref<T>> == type_info<ubigint>
);
template<typename T>
concept unsigned_integer_type=is_unsigned_integer<T>;

//file_end

