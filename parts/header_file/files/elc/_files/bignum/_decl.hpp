//_decl.hpp
//at namespace elc::defs
/*
未完成的elc解释器bignum文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
namespace bignum_n{
	class bigint;
	class ubigint;
	class bigfloat;
	class ubigfloat;
}
using namespace bignum_n;//干净的符号导出！

//注入math::arithmetic_type_info_prover
namespace base::math{
	using namespace bignum_n;
	template<>struct arithmetic_type_info_prover<bigint>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = false;
		//浮点数类型：是指数记录法还是分数记录法
		static constexpr bool is_exponent_float_type=false;
		static constexpr bool is_fraction_float_type=false;
		//bool：是否是整数类型
		static constexpr bool is_integer_type = true;
		//bool：是否是有符号类型
		static constexpr bool is_signed = true;
		//bool：是否有NaN
		static constexpr bool has_NaN = false;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//bool：是否有min
		static constexpr bool has_min = false;
		//bool：是否有max
		static constexpr bool has_max = false;
		//bool：是否有默认极限值
		static constexpr bool has_ε = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigint;
		using signed_type = bigint;
		using float_type = bigfloat;
		using integer_type = bigint;
	};

	template<>struct arithmetic_type_info_prover<ubigint>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = false;
		//浮点数类型：是指数记录法还是分数记录法
		static constexpr bool is_exponent_float_type=false;
		static constexpr bool is_fraction_float_type=false;
		//bool：是否是整数类型
		static constexpr bool is_integer_type = true;
		//bool：是否是有符号类型
		static constexpr bool is_signed = false;
		//bool：是否有NaN
		static constexpr bool has_NaN = false;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//bool：是否有min
		static constexpr bool has_min = true;
		//min
		static ubigint min()noexcept;
		//bool：是否有max
		static constexpr bool has_max = false;
		//bool：是否有默认极限值
		static constexpr bool has_ε = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigint;
		using signed_type = bigint;
		using float_type = ubigfloat;
		using integer_type = ubigint;
	};

	template<>struct arithmetic_type_info_prover<bigfloat>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = true;
		//浮点数类型：是指数记录法还是分数记录法
		static constexpr bool is_exponent_float_type=false;
		static constexpr bool is_fraction_float_type=true;
		//bool：是否是整数类型
		static constexpr bool is_integer_type = false;
		//bool：是否是有符号类型
		static constexpr bool is_signed = true;
		//bool：是否有NaN
		static constexpr bool has_NaN = true;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//bool：是否有min
		static constexpr bool has_min = false;
		//bool：是否有max
		static constexpr bool has_max = false;
		//bool：是否有默认极限值
		static constexpr bool has_ε = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigfloat;
		using signed_type = bigfloat;
		using float_type = bigfloat;
		using integer_type = bigint;
	};

	template<>struct arithmetic_type_info_prover<ubigfloat>{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type = true;
		//bool：是否是基础类型
		static constexpr bool is_basic_type = false;
		//bool：是否是大数类型
		static constexpr bool is_big_type = true;
		//bool：是否是浮点类型
		static constexpr bool is_float_type = true;
		//浮点数类型：是指数记录法还是分数记录法
		static constexpr bool is_exponent_float_type=false;
		static constexpr bool is_fraction_float_type=true;
		//bool：是否是整数类型
		static constexpr bool is_integer_type = false;
		//bool：是否是有符号类型
		static constexpr bool is_signed = false;
		//bool：是否有NaN
		static constexpr bool has_NaN = true;
		//bool：是否有inf
		static constexpr bool has_inf = false;
		//bool：是否有min
		static constexpr bool has_min = true;
		//min
		static ubigfloat min()noexcept;
		//bool：是否有max
		static constexpr bool has_max = false;
		//bool：是否有默认极限值
		static constexpr bool has_ε = false;
		//对应的无符号和有符号类型
		using unsigned_type = ubigfloat;
		using signed_type = bigfloat;
		using float_type = ubigfloat;
		using integer_type = ubigint;
	};
}

//file_end

