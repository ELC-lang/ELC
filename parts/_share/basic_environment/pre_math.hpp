//pre_math.hpp
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
	#include "_defs.hpp"

	namespace basic_environment{
		/// 每个bit（不是字节）的可能性
		/// 为什么c艹委员会不定义这个？
		constexpr size_t BIT_POSSIBILITY=1<<1;

		typedef bool bit_type;
		[[nodiscard]]constexpr force_inline bool is_rounding_bit(bit_type v)noexcept{
			if constexpr(BIT_POSSIBILITY==2)
				return v;
			else{
				push_and_disable_msvc_warning(4804);//？
				return v>=BIT_POSSIBILITY/2;
				pop_msvc_warning();
			}
		}

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

		#include "float_info.hpp"
	}
	using basic_environment::BIT_POSSIBILITY;

	#include "_undefs.hpp"
}

//file_end

