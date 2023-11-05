//float_info.hpp
/*
未完成的elc解释器_share文件
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
/*
参考信息：
float:
	Sign bit: 1 bit
	Exponent width: 8 bits
	Significand precision: 24 bits (23 explicitly stored)
double:
	Sign bit: 1 bit
	Exponent: 11 bits
	Significand precision: 53 bits (52 explicitly stored)
IEEE-754 binary128:
	Sign bit: 1 bit
	Exponent width: 15 bits
	Significand precision: 113 bits (112 explicitly stored)
注意一下long double在msvc上是double，得预处理判断一下
*/
namespace float_infos{
	struct float16_float_info{
		//精确数部分的掩码
		static constexpr auto precision_mask=uint16_t(0x07FFu);
		//精确数的无符号整数类型
		typedef decltype(precision_mask) precision_type;

		//浮点数的无符号整数数据类型
		typedef uint16_t data_type;

		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		static constexpr auto precision_base=uint16_t(0x0800u);
		//自浮点数获取精确数的基的位数（如float是23）
		static constexpr auto precision_base_bit=11u;
		//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_diff=15;
		//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
		static constexpr auto exponent_min=-14;
		//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_max=15;

		//浮点数的指数部分的无符号整数类型
		typedef uint8_t exponent_unsigned_type;
		//浮点数的指数部分的经过偏移后的有符号整数类型
		typedef int8_t exponent_signed_type;
		//浮点数的指数部分的掩码
		static constexpr auto exponent_mask=uint16_t(0x7FFFu);

		//快速开方魔数
		static constexpr auto quick_invsqrt_magic_number=uint16_t(0xB350u);//59a7 : 0.00187549, 59a8 : 0.0016386, 59a9 : 0.00181447
	};
	struct float32_float_info{
		//精确数部分的掩码
		static constexpr auto precision_mask=uint32_t(0x007FFFFFu);
		//精确数的无符号整数类型
		typedef decltype(precision_mask) precision_type;

		//浮点数的无符号整数数据类型
		typedef uint32_t data_type;

		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		static constexpr auto precision_base=uint32_t(0x800000u);
		//自浮点数获取精确数的基的位数（如float是23）
		static constexpr auto precision_base_bit=23u;
		//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_diff=127;
		//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
		static constexpr auto exponent_min=-126;
		//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_max=127;

		//浮点数的指数部分的无符号整数类型
		typedef uint8_t exponent_unsigned_type;
		//浮点数的指数部分的经过偏移后的有符号整数类型
		typedef int8_t exponent_signed_type;
		//浮点数的指数部分的掩码
		static constexpr auto exponent_mask=uint32_t(0x7FFFFFFFu);

		//快速开方魔数
		static constexpr auto quick_invsqrt_magic_number=uint32_t(0xBE6EB50Cu);
	};
	struct float64_float_info{
		//精确数部分的掩码
		static constexpr auto precision_mask=uint64_t(0x000FFFFFFFFFFFFFu);
		//精确数的无符号整数类型
		typedef decltype(precision_mask) precision_type;

		//浮点数的无符号整数数据类型
		typedef uint64_t data_type;

		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		static constexpr auto precision_base=uint64_t(0x10000000000000u);
		//自浮点数获取精确数的基的位数（如float是23）
		static constexpr auto precision_base_bit=52u;
		//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_diff=1023;
		//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
		static constexpr auto exponent_min=-1022;
		//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_max=1023;

		//浮点数的指数部分的无符号整数类型
		typedef uint16_t exponent_unsigned_type;
		//浮点数的指数部分的经过偏移后的有符号整数类型
		typedef int16_t exponent_signed_type;
		//浮点数的指数部分的掩码
		static constexpr auto exponent_mask=uint64_t(0x7FFFFFFFFFFFFFFFu);

		//快速开方魔数
		static constexpr auto quick_invsqrt_magic_number=uint64_t(0xBFCDD6A18F6A6F52u);
	};
	#if defined(ELC_BASE_ENV_HAS_INT128)
	struct float128_float_info{
		//精确数部分的掩码
		static constexpr auto precision_mask=0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF_u128;
		//精确数的无符号整数类型
		typedef decltype(precision_mask) precision_type;

		//浮点数的无符号整数数据类型
		typedef uint128_t data_type;

		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		static constexpr auto precision_base=0x10000000000000000000000000000_u128;
		//自浮点数获取精确数的基的位数（如float是23）
		static constexpr auto precision_base_bit=112u;
		//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_diff=16383;
		//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
		static constexpr auto exponent_min=-16382;
		//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_max=16383;

		//浮点数的指数部分的无符号整数类型
		typedef uint16_t exponent_unsigned_type;
		//浮点数的指数部分的经过偏移后的有符号整数类型
		typedef int16_t exponent_signed_type;
		//浮点数的指数部分的掩码
		static constexpr auto exponent_mask=0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_u128;

		//快速开方魔数
		static constexpr auto quick_invsqrt_magic_number=0xBFFCDD90BCFBC61B561FEEE8A556CED2_u128;
	};
	#endif
	struct bfloat16_float_info{
		//精确数部分的掩码
		static constexpr auto precision_mask=uint16_t(0x00FFu);
		//精确数的无符号整数类型
		typedef decltype(precision_mask) precision_type;

		//浮点数的无符号整数数据类型
		typedef uint16_t data_type;

		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		static constexpr auto precision_base=uint16_t(0x0100u);
		//自浮点数获取精确数的基的位数（如float是23）
		static constexpr auto precision_base_bit=8u;
		//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_diff=127;
		//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
		static constexpr auto exponent_min=-126;
		//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
		static constexpr auto exponent_max=127;

		//浮点数的指数部分的无符号整数类型
		typedef uint8_t exponent_unsigned_type;
		//浮点数的指数部分的经过偏移后的有符号整数类型
		typedef int8_t exponent_signed_type;
		//浮点数的指数部分的掩码
		static constexpr auto exponent_mask=uint16_t(0x7FFFu);

		//快速开方魔数
		static constexpr auto quick_invsqrt_magic_number=uint16_t(0xBE6Au);//5f34 : 0.0035338, 5f35 : 0.00163863, 5f36 : 0.00357427
	};

	template<basic_float_type T>
	static constexpr auto get_float_info()noexcept{
		#if defined(__STDCPP_FLOAT16_T__)
			if constexpr(type_info<T> == type_info<::std::float16_t>)
				return float16_float_info{};
		#endif
		#if defined(__STDCPP_FLOAT32_T__)
			if constexpr(type_info<T> == type_info<::std::float32_t>)
				return float32_float_info{};
		#endif
		#if defined(__STDCPP_FLOAT64_T__)
			if constexpr(type_info<T> == type_info<::std::float64_t>)
				return float64_float_info{};
		#endif
		#if defined(__STDCPP_FLOAT128_T__)
			#if defined(ELC_BASE_ENV_HAS_INT128)
				if constexpr(type_info<T> == type_info<::std::float128_t>)
					return float128_float_info{};
			#else
				//"float128_t is not supported"
			#endif
		#endif
		#if defined(__STDCPP_BFLOAT16_T__)
			if constexpr(type_info<T> == type_info<::std::bfloat16_t>)
				return bfloat16_float_info{};
		#endif
		if constexpr(type_info<T> == type_info<float>)
			return float32_float_info{};
		elseif constexpr(type_info<T> == type_info<double>)
			return float64_float_info{};
		elseif constexpr(type_info<T> == type_info<long double>){
			#if defined(_MSC_VER)//msvc上long double就是double
				return float64_float_info{};
			#elif defined(ELC_BASE_ENV_HAS_INT128) && defined(__aarch64__)//aarch64上long double是float128
				return float128_float_info{};
			#else
				//"long double is not supported"
			#endif
		}
	}
	template<basic_float_type T>
	using float_info=decltype(get_float_info<T>());

	//精确数部分的掩码
	template<basic_float_type T>
	constexpr auto precision_mask=float_info<T>::precision_mask;
	//精确数的无符号整数类型
	template<basic_float_type T>
	using precision_type=typename float_info<T>::precision_type;

	//浮点数的无符号整数数据类型
	template<basic_float_type T>
	using data_type=typename float_info<T>::data_type;

	//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
	template<basic_float_type T>
	constexpr auto precision_base=float_info<T>::precision_base;

	//自浮点数获取精确数的基的位数（如float是23）
	template<basic_float_type T>
	constexpr auto precision_base_bit=float_info<T>::precision_base_bit;

	//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
	template<basic_float_type T>
	constexpr auto exponent_diff=float_info<T>::exponent_diff;
	//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
	template<basic_float_type T>
	constexpr auto exponent_min=float_info<T>::exponent_min;
	//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
	template<basic_float_type T>
	constexpr auto exponent_max=float_info<T>::exponent_max;

	//浮点数的指数部分的无符号整数类型
	template<basic_float_type T>
	using exponent_unsigned_type=typename float_info<T>::exponent_unsigned_type;
	//浮点数的指数部分的经过偏移后的有符号整数类型
	template<basic_float_type T>
	using exponent_type=typename float_info<T>::exponent_signed_type;
	//浮点数的指数部分的掩码
	template<basic_float_type T>
	constexpr auto exponent_mask=float_info<T>::exponent_mask;

	//快速开方魔数
	template<basic_float_type T>
	constexpr auto quick_invsqrt_magic_number=float_info<T>::quick_invsqrt_magic_number;
}

//file_end

