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
		if(c>='A')
			return c+10-'A';
		elseif(c>='a')
			return c+10-'a';
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
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<default_base,cs...>();
		}
	};
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler<default_base,'0','x',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<16,cs...>();
		}
	};
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler<default_base,'0','X',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<16,cs...>();
		}
	};
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler<default_base,'0','b',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<2,cs...>();
		}
	};
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler<default_base,'0','B',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<2,cs...>();
		}
	};
	template<unsigned default_base,char...cs>
	struct bigint_literal_evaler<default_base,'0',cs...>{
		static constexpr_as_auto ubigint eval()noexcept{
			return eval_bigint_literal<8,cs...>();
		}
	};
	template<char...cs>
	constexpr_as_auto ubigint operator""_ubigint()noexcept{
		return bigint_literal_evaler<10,cs...>::eval();
	}
	template<unsigned default_base,char...cs>
	struct signed_bigint_literal_evaler{
		static constexpr_as_auto bigint eval()noexcept{
			return bigint_literal_evaler<default_base,cs...>::eval();
		}
	};
	template<unsigned default_base,char...cs>
	struct signed_bigint_literal_evaler<default_base,'-',cs...>{
		static constexpr_as_auto bigint eval()noexcept{
			return -bigint_literal_evaler<default_base,cs...>::eval();
		}
	};
	template<char...cs>
	constexpr_as_auto bigint operator""_bigint()noexcept{
		return signed_bigint_literal_evaler<10,cs...>::eval();
	}
	//浮点数字面量
	template<unsigned base,char exp_char,bool pointed>
	constexpr_as_auto ubigfloat eval_bigfloat_literal(ubigint val=zero,bigint exp=0)noexcept{
		ubigfloat aret=val;
		if(is_negative(exp))
			do aret/=base; while(++exp);
		elseif(exp)
			do aret*=base; while(--exp);
		return aret;
	}
	template<unsigned base,char exp_char,bool pointed,char c,char...cs>
	constexpr_as_auto ubigfloat eval_bigfloat_literal(ubigint val=zero,bigint exp=0)noexcept{
		if constexpr(c=='\''){
			return eval_bigfloat_literal<base,exp_char,pointed,cs...>(val,exp);
		}
		elseif constexpr(c=='.'){
			static_assert(!pointed,"pointed twice");
			return eval_bigfloat_literal<base,exp_char,true,cs...>(val,exp);
		}
		elseif constexpr(c==exp_char || c==exp_char+('A'-'a')){
			exp+=signed_bigint_literal_evaler<base,cs...>::eval();
			return eval_bigfloat_literal<base,exp_char,pointed>(val,exp);
		}
		else{
			if constexpr(pointed)
				--exp;
			static_assert(base==16||base==10||base==8||base==2,"base must be 16,10,8 or 2");
			static_assert(c>='0'&&c<='9'||c>='a'&&c<='f'||c>='A'&&c<='F',"invalid char");
			static_assert(base>hexval(c),"invalid char");
			return eval_bigfloat_literal<base,exp_char,pointed,cs...>(val*base+hexval(c),exp);
		}
	}
	template<unsigned default_base,char default_exp_char,char...cs>
	struct bigfloat_literal_evaler{
		static constexpr_as_auto ubigfloat eval()noexcept{
			return eval_bigfloat_literal<default_base,default_exp_char,false,cs...>();
		}
	};
	template<unsigned default_base,char default_exp_char,char...cs>
	struct bigfloat_literal_evaler<default_base,default_exp_char,'0','x',cs...>{
		static constexpr_as_auto ubigfloat eval()noexcept{
			return eval_bigfloat_literal<16,'p',false,cs...>();
		}
	};
	template<unsigned default_base,char default_exp_char,char...cs>
	struct bigfloat_literal_evaler<default_base,default_exp_char,'0','X',cs...>{
		static constexpr_as_auto ubigfloat eval()noexcept{
			return eval_bigfloat_literal<16,'p',false,cs...>();
		}
	};
	template<char...cs>
	constexpr_as_auto ubigfloat operator""_ubigfloat()noexcept{
		return bigfloat_literal_evaler<10,'e',cs...>::eval();
	}
	template<unsigned default_base,char default_exp_char,char...cs>
	struct signed_bigfloat_literal_evaler{
		static constexpr_as_auto bigfloat eval()noexcept{
			return bigfloat_literal_evaler<default_base,default_exp_char,cs...>::eval();
		}
	};
	template<unsigned default_base,char default_exp_char,char...cs>
	struct signed_bigfloat_literal_evaler<default_base,default_exp_char,'-',cs...>{
		static constexpr_as_auto bigfloat eval()noexcept{
			return -bigfloat_literal_evaler<default_base,default_exp_char,cs...>::eval();
		}
	};
	template<char...cs>
	constexpr_as_auto bigfloat operator""_bigfloat()noexcept{
		return signed_bigfloat_literal_evaler<10,'e',cs...>::eval();
	}
}
using literal_n::operator""_ubigint;
using literal_n::operator""_bigint;
using literal_n::operator""_ubigfloat;
using literal_n::operator""_bigfloat;

//file_end

