//arithmetic_type_info_prover.hpp
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
	template<typename T>
	struct arithmetic_type_info_prover{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type=::std::is_arithmetic_v<T> || is_elc_expansion_base_type<T>;
		//bool：是否是基础类型
		static constexpr bool is_basic_type=::std::is_arithmetic_v<T>;
		//bool：是否是大数类型
		static constexpr bool is_big_type=false;
		//bool：是否是浮点类型
		static constexpr bool is_float_type=::std::is_floating_point_v<T>;
		//浮点数类型：是指数记录法还是分数记录法
		static constexpr bool is_exponent_float_type=is_float_type;
		static constexpr bool is_fraction_float_type=false;
		//bool：是否是整数类型
		static constexpr bool is_integer_type=::std::is_integral_v<T>;
		//bool：是否是有符号类型
		static constexpr bool is_signed=::std::is_signed_v<T>;
		//bool：是否有NaN
	private:
		static constexpr bool has_quiet_NaN_helper()noexcept{
			if constexpr(is_arithmetic_type)
				return ::std::numeric_limits<T>::has_quiet_NaN;
			else
				return false;
		}
		static constexpr bool has_signaling_NaN_helper()noexcept{
			if constexpr(is_arithmetic_type)
				return ::std::numeric_limits<T>::has_signaling_NaN;
			else
				return false;
		}
	public:
		static constexpr bool has_quiet_NaN=has_quiet_NaN_helper();
		static constexpr bool has_signaling_NaN=has_signaling_NaN_helper();
		static constexpr bool has_NaN=has_quiet_NaN || has_signaling_NaN;

		static constexpr auto quiet_NaN()noexcept requires(has_quiet_NaN){
			return ::std::numeric_limits<T>::quiet_NaN();
		}
		static constexpr auto signaling_NaN()noexcept requires(has_signaling_NaN){
			return ::std::numeric_limits<T>::signaling_NaN();
		}
		static constexpr auto NaN()noexcept requires(has_NaN){
			if constexpr(has_quiet_NaN)
				return quiet_NaN();
			else
				return signaling_NaN();
		}
		//bool：是否有inf
	private:
		static constexpr bool has_inf_helper()noexcept{
			if constexpr(is_arithmetic_type)
				return ::std::numeric_limits<T>::has_infinity;
			else
				return false;
		}
	public:
		static constexpr bool has_inf=has_inf_helper();
		static constexpr auto Inf()noexcept requires(has_inf){
			return ::std::numeric_limits<T>::infinity();
		}
		static constexpr auto negative_Inf()noexcept requires(has_inf){
			return -Inf();
		}
	public:
		//bool：是否有最小值
		static constexpr bool has_min=is_arithmetic_type;
		//bool：是否有最大值
		static constexpr bool has_max=is_arithmetic_type;
	public:
		static constexpr auto min()noexcept requires(has_min){
			if constexpr(is_float_type)
				return ::std::numeric_limits<T>::lowest();
			else
				return ::std::numeric_limits<T>::min();
		}
		static constexpr auto max()noexcept requires(has_max){
			return ::std::numeric_limits<T>::max();
		}
	public:
		//bool：是否有默认极限值
		static constexpr bool has_ε=is_arithmetic_type;
	public:
		static constexpr auto ε()noexcept requires(has_ε){
			return ::std::numeric_limits<T>::epsilon();
		}
	public:
		//对应的无符号和有符号类型
		using unsigned_type=decltype(λ{
			if constexpr(type_info<T> == type_info<bool>)
				return T();
			elseif constexpr(is_float_type)//基础浮点类型没有对应的无符号
				return T();
			elseif constexpr(is_integer_type)//考虑到charX_t，所有整数类型都应该过一遍make_unsigned_t
				return::std::make_unsigned_t<T>();
		}());
		using signed_type=decltype(λ{
			if constexpr(type_info<T> == type_info<bool>)
				return T();
			elseif constexpr(is_float_type)
				return T();
			elseif constexpr(is_integer_type)//同上
				return::std::make_signed_t<T>();
		}());
		//对应的浮点数类型
		using float_type=decltype(λ{
			if constexpr(is_float_type)
				return T();
			elseif constexpr(is_integer_type)
				return .0;
		}());
		//对应的整数类型
		using integer_type=decltype(λ{
			if constexpr(is_integer_type)
				return T();
			elseif constexpr(is_float_type)
				return 0;
		}());
	};
}
using math::arithmetic_type_info_prover;

//file_end

