//arithmetic_type_info_prover_decl.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
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
namespace math{
	/// 算术类型帮助类型
	/// 任何后续定义的新算术类型都可以重载这些类型来实现数学库的泛型支持
	template<typename T>
	struct arithmetic_type_info_prover;
	
	/*! 无符号位的对应类型 */
	template<typename T>
	using to_unsigned_t = typename arithmetic_type_info_prover<remove_cvref<T>>::unsigned_type;
	/*! 有符号位的对应类型 */
	template<typename T>
	using to_signed_t = typename arithmetic_type_info_prover<remove_cvref<T>>::signed_type;
	/*! 对应的浮点数类型 */
	template<typename T>
	using float_type_of = typename arithmetic_type_info_prover<remove_cvref<T>>::float_type;
	/*! 对应的整数类型 */
	template<typename T>
	using integer_type_of = typename arithmetic_type_info_prover<remove_cvref<T>>::integer_type;

	/// 概念名称空间
	/// 基于标准库的概念，但是使用帮助类型来便于后续定义中的新算数类型重载
	inline namespace concepts{
		/// 算术类型概念
		template<typename T>
		concept arithmetic_type=arithmetic_type_info_prover<remove_cvref<T>>::is_arithmetic_type;
		template<typename T>
		concept is_arithmetic_type=arithmetic_type<T>;

		/// 基础类型概念
		template<typename T>
		concept basic_type=arithmetic_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_basic_type;
		template<typename T>
		concept is_basic_type=basic_type<T>;

		/// 大数类型概念
		template<typename T>
		concept big_type=arithmetic_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_big_type;
		template<typename T>
		concept is_big_type=big_type<T>;

		/// 浮点类型概念
		template<typename T>
		concept float_type=arithmetic_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_float_type;
		template<typename T>
		concept is_float_type=float_type<T>;

		//浮点数类型：指数记录法
		template<typename T>
		concept exponent_float_type=is_float_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_exponent_float_type;
		template<typename T>
		concept is_exponent_float_type=exponent_float_type<T>;

		//浮点数类型：分数记录法
		template<typename T>
		concept fraction_float_type=is_float_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_fraction_float_type;
		template<typename T>
		concept is_fraction_float_type=fraction_float_type<T>;

		/// 基础浮点数类型概念
		template<typename T>
		concept basic_float_type=float_type<T> && basic_type<T>;
		template<typename T>
		concept is_basic_float_type=basic_float_type<T>;

		/// 整数类型概念
		template<typename T>
		concept integer_type=arithmetic_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_integer_type;
		template<typename T>
		concept is_integer_type=integer_type<T>;

		/// 基础整数类型概念
		template<typename T>
		concept basic_integer_type=integer_type<T> && basic_type<T>;
		template<typename T>
		concept is_basic_integer_type=basic_integer_type<T>;

		/// 有符号类型概念
		template<typename T>
		concept signed_type=arithmetic_type<T> && arithmetic_type_info_prover<remove_cvref<T>>::is_signed;
		template<typename T>
		concept is_signed=signed_type<T>;

		/// 无符号类型概念
		template<typename T>
		concept unsigned_type=arithmetic_type<T> && !is_signed<T>;
		template<typename T>
		concept is_unsigned=unsigned_type<T>;

		/// 有符号整数类型概念
		template<typename T>
		concept signed_integer_type=integer_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_integer_type=signed_integer_type<T>;

		/// 无符号整数类型概念
		template<typename T>
		concept unsigned_integer_type=integer_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_integer_type=unsigned_integer_type<T>;

		/// 无符号浮点数类型概念
		template<typename T>
		concept unsigned_float_type=float_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_float_type=unsigned_float_type<T>;

		/// 有符号浮点数类型概念
		template<typename T>
		concept signed_float_type=float_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_float_type=signed_float_type<T>;

		/// 有符号基础整数类型概念
		template<typename T>
		concept signed_basic_integer_type=basic_integer_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_basic_integer_type=signed_basic_integer_type<T>;

		/// 无符号基础整数类型概念
		template<typename T>
		concept unsigned_basic_integer_type=basic_integer_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_basic_integer_type=unsigned_basic_integer_type<T>;

		/// 有符号基础浮点数类型概念
		template<typename T>
		concept signed_basic_float_type=basic_float_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_basic_float_type=signed_basic_float_type<T>;

		/// 无符号基础浮点数类型概念
		template<typename T>
		concept unsigned_basic_float_type=basic_float_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_basic_float_type=unsigned_basic_float_type<T>;

		/// 大整数类型概念
		template<typename T>
		concept big_integer_type=integer_type<T> && big_type<T>;
		template<typename T>
		concept is_big_integer_type=big_integer_type<T>;

		/// 大浮点数类型概念
		template<typename T>
		concept big_float_type=float_type<T> && big_type<T>;
		template<typename T>
		concept is_big_float_type=big_float_type<T>;

		/// 无符号大整数类型概念
		template<typename T>
		concept unsigned_big_integer_type=big_integer_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_big_integer_type=unsigned_big_integer_type<T>;

		/// 有符号大整数类型概念
		template<typename T>
		concept signed_big_integer_type=big_integer_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_big_integer_type=signed_big_integer_type<T>;

		/// 无符号大浮点数类型概念
		template<typename T>
		concept unsigned_big_float_type=big_float_type<T> && is_unsigned<T>;
		template<typename T>
		concept is_unsigned_big_float_type=unsigned_big_float_type<T>;

		/// 有符号大浮点数类型概念
		template<typename T>
		concept signed_big_float_type=big_float_type<T> && is_signed<T>;
		template<typename T>
		concept is_signed_big_float_type=signed_big_float_type<T>;

		/// 有NaN的类型概念
		template<typename T>
		concept has_NaN=arithmetic_type_info_prover<remove_cvref<T>>::has_NaN;

		/// 有inf的类型概念
		template<typename T>
		concept has_inf=arithmetic_type_info_prover<remove_cvref<T>>::has_inf;

		/// 有min的类型概念
		template<typename T>
		concept has_min=arithmetic_type_info_prover<remove_cvref<T>>::has_min;

		/// 有max的类型概念
		template<typename T>
		concept has_max=arithmetic_type_info_prover<remove_cvref<T>>::has_max;

		/// 有min和max的类型概念
		template<typename T>
		concept has_min_max=has_min<T> && has_max<T>;

		/// 有ε的类型概念
		template<typename T>
		concept has_ε=arithmetic_type_info_prover<remove_cvref<T>>::has_ε;
	}
}
using math::to_unsigned_t;
using math::to_signed_t;
using math::float_type_of;
using math::integer_type_of;
using namespace math::concepts;

//file_end

