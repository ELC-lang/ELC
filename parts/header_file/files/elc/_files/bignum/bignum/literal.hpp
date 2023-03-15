//literal.hpp
//at namespace elc::defs::bignum_n
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
namespace literal_n{
	//在此我们定义对于ubigint\bigint\ubigfloat\bigfloat的字面量支持。
	constexpr uint8_t hexval(char c)noexcept{
		if(c>'A')
			return c-'A'+10;
		elseif(c>'a')
			return c-'a'+10;
		else
			return c-'0';
	}
	template<unsigned base>
	constexpr_as_auto ubigint eval_bigint_literal(ubigint val=zero)noexcept{
		return val;
	}
	template<unsigned base,char c,char...cs>
	constexpr_as_auto ubigint eval_bigint_literal(ubigint val=zero)noexcept{
		if constexpr(c!='\''){
			static_assert(base==16||base==10||base==8||base==2,"base must be 16,10,8 or 2");
			static_assert(c>='0'&&c<='9'||c>='a'&&c<='f'||c>='A'&&c<='F',"invalid char");
			static_assert(base>hexval(c),"invalid char");
			return eval_bigint_literal<base,cs...>(val*base+hexval(c));
		}else
			return eval_bigint_literal<base,cs...>(val);
	}
	template<char...cs>
	struct bigint_literal_evaler{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<10,cs...>();
		}
	};
	template<char...cs>
	struct bigint_literal_evaler<'0','x',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<16,cs...>();
		}
	};
	template<char...cs>
	struct bigint_literal_evaler<'0','X',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<16,cs...>();
		}
	};
	template<char...cs>
	struct bigint_literal_evaler<'0','b',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<2,cs...>();
		}
	};
	template<char...cs>
	struct bigint_literal_evaler<'0','B',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<2,cs...>();
		}
	};
	template<char...cs>
	struct bigint_literal_evaler<'0',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<8,cs...>();
		}
	};
	template<char...cs>
	constexpr_as_auto ubigint operator""_ubigint()noexcept{
		return bigint_literal_evaler<cs...>::eval();
	}
	template<char...cs>
	struct signed_bigint_literal_evaler{
		static constexpr_as_auto bigint eval()noexcept{
			return bigint_literal_evaler<cs...>::eval();
		}
	};
	template<char...cs>
	struct signed_bigint_literal_evaler<'-',cs...>{
		static constexpr_as_auto bigint eval()noexcept{
			return -bigint_literal_evaler<cs...>::eval();
		}
	};
	template<char...cs>
	constexpr_as_auto bigint operator""_bigint()noexcept{
		return signed_bigint_literal_evaler<cs...>::eval();
	}
}
using literal_n::operator""_ubigint;
using literal_n::operator""_bigint;

//file_end

