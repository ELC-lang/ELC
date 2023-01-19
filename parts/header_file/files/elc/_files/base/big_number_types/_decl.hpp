//_decl.hpp
//at namespace elc::defs::big_number_types
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

class bigint;
class ubigint;
class bigfloat;
class ubigfloat;

//注入math::arithmetic_type_info_helper
BREAK_NAMESPACE;

namespace base::math{
	using namespace big_number_types;
	template<>struct arithmetic_type_info_helper<bigint>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = false;
		//bool：是否是有符号类型
		static constexpr bool is_signed = true;
		//bool：是否有NaN
		static constexpr bool has_NaN = false;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigint;
		using signed_type = bigint;
	};
	
	template<>struct arithmetic_type_info_helper<ubigint>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = false;
		//bool：是否是有符号类型
		static constexpr bool is_signed = false;
		//bool：是否有NaN
		static constexpr bool has_NaN = false;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigint;
		using signed_type = bigint;
	};

	template<>struct arithmetic_type_info_helper<bigfloat>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = true;
		//bool：是否是有符号类型
		static constexpr bool is_signed = true;
		//bool：是否有NaN
		static constexpr bool has_NaN = true;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigfloat;
		using signed_type = bigfloat;
	};

	template<>struct arithmetic_type_info_helper<ubigfloat>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = true;
		//bool：是否是有符号类型
		static constexpr bool is_signed = false;
		//bool：是否有NaN
		static constexpr bool has_NaN = true;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigfloat;
		using signed_type = bigfloat;
	};
}

INTER_NAMESPACE(big_number_types);

//file_end

