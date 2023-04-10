//_body.hpp
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
namespace elc::defs{
	#include "../_defs.hpp"

	namespace basic_environment{
		/// 每个bit（不是字节）的可能性
		/// 为什么c艹委员会不定义这个？
		constexpr size_t BIT_POSSIBILITY=2;

		#if defined(__SIZEOF_INT128__)
			#define ELC_BASE_ENV_HAS_INT128
			typedef __uint128_t uint128_t;
			typedef __int128_t int128_t;
		#endif
		#if defined(ELC_BASE_ENV_HAS_INT128)
			typedef uint128_t uint_fast128_t;
			typedef int128_t int_fast128_t;
			template<char...cs>
			constexpr uint128_t operator ""_u128()noexcept{
				return literal_support::unsigned_integer_literal_evaler<uint128_t>::eval<cs...>();
			}
			template<char...cs>
			constexpr int128_t operator ""_i128()noexcept{
				return literal_support::signed_integer_literal_evaler<int128_t,uint128_t>::eval<cs...>();
			}
		#endif
		//基础的uintmax_t
		typedef ::std::uintmax_t basic_uintmax_t;
		typedef ::std::intmax_t basic_intmax_t;
		//更新uintmax_t和intmax_t的代指
		typedef decltype(lambda{
			#if defined(ELC_BASE_ENV_HAS_INT128)
			if constexpr(sizeof(basic_uintmax_t) < sizeof(uint128_t))
				return uint128_t{};
			else
			#endif
				return basic_uintmax_t{};
		}()) uintmax_t;
		typedef decltype(lambda{
			#if defined(ELC_BASE_ENV_HAS_INT128)
			if constexpr(sizeof(basic_intmax_t) < sizeof(int128_t))
				return int128_t{};
			else
			#endif
				return basic_intmax_t{};
		}()) intmax_t;
		//众所周知，class不能作为数组下标，所以uintmax_index_t等类型的存在是有必要的
		typedef basic_uintmax_t uintmax_index_t;
		typedef basic_intmax_t intmax_index_t;
		template<class T>
		constexpr inline bool is_elc_expansion_base_type_helper()noexcept{
			#if defined(ELC_BASE_ENV_HAS_INT128)
			if constexpr(sizeof(basic_uintmax_t) < sizeof(uint128_t) && type_info<T> == type_info<uint128_t>)
				return true;
			if constexpr(sizeof(basic_intmax_t) < sizeof(int128_t) && type_info<T> == type_info<int128_t>)
				return true;
			#endif
			return false;
		}
		template<class T>
		concept is_elc_expansion_base_type=is_elc_expansion_base_type_helper<T>();

		/*! 给定大小的无符号整数类型 */
		template<size_t size>
		using unsigned_specific_size_t=decltype(lambda{
			#define TYPE_MAPPER(type) if constexpr(size == sizeof(type))return (type)0;else
			#include "./arithmetic_mapper/unsigned_mapper.hpp"
			#undef TYPE_MAPPER
			{}
		}());
		/*! 至少有给定大小的快速无符号整数类型 */
		template<size_t size>
		using unsigned_specific_size_fast_t=decltype(lambda{
			#define TYPE_MAPPER(type) if constexpr(size <= sizeof(type))return (type)0;else
			#include "./arithmetic_mapper/unsigned_fast_mapper.hpp"
			#undef TYPE_MAPPER
			{}
		}());
		//任意类型转算数类型
		inline constexpr class to_arithmetic_t{
			template<class T,class type>
			//由于can_convert_to的歧义性质，使用requires表达式辅助推断type是否为实际支持的类型
			//由于requires表达式的强制性质，使用can_convert_to辅助推断隐式转换是否可行
			static inline constexpr bool is_convertible = type_info<T>.can_convert_to<type> && was_not_an_ill_form(static_cast<type>(declvalue(T)));
			template<class T>
			static inline constexpr bool r_able_helper()noexcept{
				#define TYPE_MAPPER(type) if constexpr(is_convertible<T,type>)return true;else
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				return false;
			}
			template<class T>
			static force_inline constexpr auto to_arithmetic_base(T&&v)noexcept{
				#define TYPE_MAPPER(type) if constexpr(r_able_helper<T>()&&is_convertible<T,type>)return (type)v;else
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				{}
			}
			template<class T>
			static inline constexpr size_t get_arithmetic_muti_convertible_count()noexcept{
				typedef decltype(to_arithmetic_base(declvalue(T))) to_type;
				size_t muti_convertible_count=0;
				#define TYPE_MAPPER(type) \
				if constexpr(type_info<to_type>!=type_info<type> && is_convertible<T,type>)\
					muti_convertible_count++;
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				return muti_convertible_count;
			}
			template<class T>
			static inline constexpr bool convertible_unique_helper()noexcept{
				return get_arithmetic_muti_convertible_count<T>()==1;//the oly way we can save all lifes........
			}
			template<class T>
			static inline constexpr bool able_helper()noexcept{
				if constexpr(r_able_helper<T>())
					return convertible_unique_helper<T>();
				else
					return false;
			}
		public:
			template<class T>
			static inline constexpr bool able = able_helper<T>();
		private:
			//nothrow
			template<class T>
			static inline constexpr bool nothrow_helper()noexcept{
				typedef decltype(to_arithmetic_base(declvalue(T))) to_type;
				if constexpr(able<T>)
					return type_info<T>.can_nothrow_convert_to<to_type>;
				else
					return false;
			}
		public:
			template<class T>
			static inline constexpr bool nothrow = nothrow_helper<T>();
			//算数类型转任意类型
			template<class T> requires able<T>
			force_inline constexpr auto operator()(T&&v)const noexcept(nothrow<T>){
				return to_arithmetic_base(forward<T>(v));
			}
		}to_arithmetic{};
		//判断wchar_t是否和char_t相同
		inline constexpr bool wchar_t_same_as_char_t=sizeof(wchar_t)==sizeof(char_t);
		//判断wchar_t是否和char16_t相同
		inline constexpr bool wchar_t_same_as_char16_t=sizeof(wchar_t)==sizeof(char16_t);

		#include "float_info.hpp"
		namespace float_infos{
			template<basic_float_type T>
			union float_data_union{
				T v;
				data_type<T> data;
			};
			template<basic_float_type T>
			constexpr data_type<T> get_float_data(T v)noexcept{
				float_data_union<T> tmp;
				tmp.v=v;
				return tmp.data;
			}
			template<basic_float_type T>
			constexpr T get_float_from_data(data_type<T> data)noexcept{
				float_data_union<T> tmp;
				tmp.data=data;
				return tmp.v;
			}
		}
		//自浮点数获取原生精确数部分，舍去指数和符号位
		template<basic_float_type T>
		force_inline constexpr auto get_native_precision(T v)noexcept{
			using namespace float_infos;
			auto tmp=get_float_data(v);
			tmp&=precision_mask<T>;
			return tmp;
		}
		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		template<basic_float_type T>
		force_inline constexpr auto get_precision_base(T=T{})noexcept{
			return float_infos::precision_base<T>;
		}
		template<basic_float_type T>
		using float_precision_base_t = decltype(get_precision_base<T>());
		//（基础的）自浮点数获取指数部分，舍去基数和符号位
		template<basic_float_type T>
		force_inline constexpr auto base_get_exponent(T v)noexcept{
			using namespace float_infos;
			auto tmp=get_float_data(v);
			tmp&=exponent_mask<T>;
			tmp>>=precision_base_bit<T>;
			return exponent_unsigned_type<T>(tmp);
		}
		//自浮点数获取原生指数部分，舍去基数和符号位
		template<basic_float_type T>
		force_inline constexpr auto get_native_exponent(T v)noexcept{
			const auto tmp=base_get_exponent(v);
			using namespace float_infos;
			if(tmp==0)return exponent_min<T>;
			else return exponent_type<T>(tmp)-exponent_diff<T>;
		}
		//自浮点数获取基数
		template<basic_float_type T>
		force_inline constexpr auto get_base_num(T v)noexcept{
			//特殊情况处理（exp=0时，base=0）
			const auto tmp=base_get_exponent(v);
			return tmp?get_precision_base(v):float_precision_base_t<T>{};
		}
		//自浮点数获取基数
		template<basic_float_type T>
		force_inline constexpr auto get_precision(T v)noexcept{
			return get_native_precision(v)+get_base_num(v);
		}
		//自浮点数获取指数
		template<basic_float_type T>
		force_inline constexpr auto get_exponent(T v)noexcept{
			using namespace float_infos;
			return ptrdiff_t(get_native_exponent(v))-precision_base_bit<T>;
		}
		//自浮点数获取基数和指数
		template<basic_float_type T>
		force_inline constexpr auto get_precision_and_exponent(T v)noexcept{
			struct precision_and_exponent_t{
				float_precision_base_t<T> precision;
				ptrdiff_t exponent;
			};
			const auto precision=get_precision(v);
			const auto exponent=get_exponent(v);
			return precision_and_exponent_t{precision,exponent};
		}
		//自基数和指数构造浮点数
		//num=base_num*2^exponent
		template<basic_float_type T>
		force_inline constexpr T make_float(float_precision_base_t<T> base_num,ptrdiff_t exponent)noexcept{
			using namespace float_infos;
			//首先将基数转换为precision_base（2^precision_base_bit）为分母的分数的分子
			//并在此过程中加减指数
			//需要注意的是，这里的基数是包含1的，所以转换目标是base_num>>precision_base_bit为1
			{
				const auto tmp=countl_zero(base_num);
				constexpr auto need_shift=bitnum_of(base_num)-precision_base_bit<T>-1;
				const ptrdiff_t shift=tmp-need_shift;
				if(shift>0){
					base_num<<=shift;
					exponent-=shift;
				}
				elseif(shift<0){
					base_num>>=-shift;
					exponent-=shift;
				}
			}
			//适当放缩后，需要偏移指数部分，原因如下：
			//原有逻辑是num=base_num*2^exponent，这是大分数的表达逻辑
			//但是浮点数的逻辑是num=(base_num/precision_base)*2^exponent
			//将precision_base合并到exponent中，即可得到浮点数的逻辑
			exponent+=precision_base_bit<T>;
			if(exponent < exponent_min<T>){//指数过小，需要舍去
				const auto diff=exponent_min<T> - exponent;
				base_num>>=diff;
				exponent+=diff;
				if(!base_num)return 0;//基数过小，无法表示
			}
			if(exponent>exponent_max<T>)return 0;//指数过大，无法表示
			//DEN情况判断
			const bool is_den=!(base_num>>precision_base_bit<T>);//若基数最高位为0，则exponent一定为exponent_min，不用判断
			if(is_den)
				return get_float_from_data<T>(base_num);
			else{//非DEN情况下，根据浮点数表示规则去掉基数多余的1
				const auto exp=exponent_unsigned_type<T>(exponent+exponent_diff<T>);
				base_num-=get_precision_base<T>();
				data_type<T> data=base_num;
				data|=data_type<T>(exp)<<precision_base_bit<T>;
				return get_float_from_data<T>(data);
			}
		}
		//若可以，将浮点数分解为两数之商
		template<basic_float_type T,size_t max_numerator = 1u<<7,size_t max_denominator = 1u<<17>
		force_inline constexpr auto to_divide(T v)noexcept{
			struct divide_t{
				T numerator;
				size_t denominator;
			private:
				bool _success;
			public:
				constexpr divide_t(T numerator,size_t denominator)noexcept:_success(true),numerator(numerator),denominator(denominator){}
				constexpr divide_t()noexcept:_success(false),numerator(0),denominator(0){}
				[[nodiscard]]constexpr bool success()const noexcept{
					return _success;
				}
				[[nodiscard]]constexpr explicit operator bool()const noexcept{
					return success();
				}
			};
			T integer_part;
			const auto fraction_part=std::modf(v,&integer_part);
			if(fraction_part==0)return divide_t{integer_part,1u};
			//开始测试分数
			size_t numerator=1;
			while(numerator<max_numerator){
				const auto denominator = to_size_t(numerator/fraction_part);
				if(denominator<=max_denominator)//忽略分母过大的情况
				if(is_close(T(numerator)/denominator,fraction_part))//近似相等即可
				if(gcd(numerator,denominator)==1)//不进行重复检查
				{
					//补正整数部分
					const auto numerator_with_inter = integer_part*denominator+numerator;
					//检查是否能够转换回
					const auto f_test = T(numerator_with_inter)/denominator;
					if(f_test==fraction_part)
						return divide_t{numerator_with_inter,denominator};
				}
				numerator++;
			}
			return divide_t{};
		}
	}
	using basic_environment::BIT_POSSIBILITY;

	using basic_environment::basic_uintmax_t;
	using basic_environment::basic_intmax_t;
	using basic_environment::uintmax_index_t;
	using basic_environment::intmax_index_t;
	using basic_environment::uintmax_t;
	using basic_environment::intmax_t;
	#if defined(ELC_BASE_ENV_HAS_INT128)
	using basic_environment::uint128_t;
	using basic_environment::int128_t;
	using basic_environment::operator""_u128;
	using basic_environment::operator""_i128;
	#endif
	using basic_environment::is_elc_expansion_base_type;

	using basic_environment::unsigned_specific_size_t;
	using basic_environment::unsigned_specific_size_fast_t;

	using basic_environment::to_arithmetic;

	using basic_environment::wchar_t_same_as_char_t;
	using basic_environment::wchar_t_same_as_char16_t;

	using basic_environment::get_exponent;
	using basic_environment::get_precision;
	using basic_environment::float_precision_base_t;
	using basic_environment::get_precision_and_exponent;
	using basic_environment::make_float;
	using basic_environment::to_divide;

	#include "../_undefs.hpp"
}

//file_end

