//literal_support.hpp
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
namespace literal_support{
	constexpr uint8_t hexval(char c)noexcept{
		if constexpr('a'>'A'){
			if(c>='a')
				return c+10-'a';
			elseif(c>='A')
				return c+10-'A';
		}
		else{
			if(c>='A')
				return c+10-'A';
			elseif(c>='a')
				return c+10-'a';
		}
		return c-'0';
	}
	template<class integer_T>
	class unsigned_integer_literal_evaler{
		template<unsigned base>
		static constexpr_as_auto integer_T eval_impl(integer_T val={})noexcept{
			return val;
		}
		template<unsigned base,char c,char...cs>
		static constexpr_as_auto integer_T eval_impl(integer_T val={})noexcept{
			if constexpr(c!='\''){
				static_assert(base==16||base==10||base==8||base==2,"base must be 16,10,8 or 2");
				static_assert(c>='0'&&c<='9'||c>='a'&&c<='f'||c>='A'&&c<='F',"invalid char");
				static_assert(base>hexval(c),"invalid char");
				return eval_impl<base,cs...>(val*base+hexval(c));
			}else
				return eval_impl<base,cs...>(val);
		}
		template<unsigned default_base,char...cs>
		struct eval_differ{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<default_base,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','x',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<16,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','X',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<16,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','b',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<2,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','B',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<2,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return eval_impl<8,cs...>();
			}
		};
	public:
		template<unsigned default_base,char...cs>
		static constexpr_as_auto integer_T eval_with_base()noexcept{
			return eval_differ<default_base,cs...>::eval();
		}
		template<char...cs>
		static constexpr_as_auto integer_T eval()noexcept{
			return eval_with_base<10,cs...>();
		}
	};
	template<class integer_T,class unsigned_integer_T>
	class signed_integer_literal_evaler{
		template<unsigned default_base,char...cs>
		struct eval_differ{
			static constexpr_as_auto integer_T eval()noexcept{
				return unsigned_integer_literal_evaler<unsigned_integer_T>::template eval_with_base<default_base,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'-',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return -static_cast<integer_T>(unsigned_integer_literal_evaler<unsigned_integer_T>::template eval_with_base<default_base,cs...>());
			}
		};
	public:
		template<unsigned default_base,char...cs>
		static constexpr_as_auto integer_T eval_with_base()noexcept{
			return eval_differ<default_base,cs...>::eval();
		}
		template<char...cs>
		static constexpr_as_auto integer_T eval()noexcept{
			return eval_with_base<10,cs...>();
		}
	};
	template<class float_T,class base_process_T,class exp_process_integer_T,class exp_process_unsigned_integer_T>
	class unsigned_float_literal_evaler{
		template<unsigned base,char exp_char,bool pointed>
		static constexpr_as_auto float_T eval_impl(base_process_T val={},exp_process_integer_T exp={})noexcept{
			float_T aret=val;
			aret*=pow(base,exp);
			return aret;
		}
		template<unsigned base,char exp_char,bool pointed,char c,char...cs>
		static constexpr_as_auto float_T eval_impl(base_process_T val={},exp_process_integer_T exp={})noexcept{
			if constexpr(c=='\''){
				return eval_impl<base,exp_char,pointed,cs...>(val,exp);
			}
			elseif constexpr(c=='.'){
				static_assert(!pointed,"pointed twice");
				return eval_impl<base,exp_char,true,cs...>(val,exp);
			}
			elseif constexpr(c==exp_char || c==exp_char+('A'-'a')){
				exp+=signed_integer_literal_evaler<exp_process_integer_T,exp_process_unsigned_integer_T>::template eval_with_base<base,cs...>();
				return eval_impl<base,exp_char,pointed>(val,exp);
			}
			else{
				if constexpr(pointed)
					--exp;
				static_assert(base==16||base==10||base==8||base==2,"base must be 16,10,8 or 2");
				static_assert(c>='0'&&c<='9'||c>='a'&&c<='f'||c>='A'&&c<='F',"invalid char");
				static_assert(base>hexval(c),"invalid char");
				return eval_impl<base,exp_char,pointed,cs...>(val*base+hexval(c),exp);
			}
		}
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ{
			static constexpr_as_auto float_T eval()noexcept{
				return eval_impl<default_base,default_exp_char,false,cs...>();
			}
		};
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ<default_base,default_exp_char,'0','x',cs...>{
			static constexpr_as_auto float_T eval()noexcept{
				return eval_impl<16,'p',false,cs...>();
			}
		};
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ<default_base,default_exp_char,'0','X',cs...>{
			static constexpr_as_auto float_T eval()noexcept{
				return eval_impl<16,'p',false,cs...>();
			}
		};
	public:
		template<unsigned default_base,char default_exp_char,char...cs>
		static constexpr_as_auto float_T eval_with_base()noexcept{
			return eval_differ<default_base,default_exp_char,cs...>::eval();
		}
		template<char...cs>
		static constexpr_as_auto float_T eval()noexcept{
			return eval_with_base<10,'e',cs...>();
		}
	};
	template<class float_T,class unsigned_float_T,class base_process_T,class exp_process_integer_T,class exp_process_unsigned_integer_T>
	class signed_float_literal_evaler{
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ{
			static constexpr_as_auto float_T eval()noexcept{
				return unsigned_float_literal_evaler<unsigned_float_T,base_process_T,exp_process_integer_T,exp_process_unsigned_integer_T>::template eval_with_base<default_base,default_exp_char,cs...>();
			}
		};
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ<default_base,default_exp_char,'-',cs...>{
			static constexpr_as_auto float_T eval()noexcept{
				return -static_cast<float_T>(unsigned_float_literal_evaler<unsigned_float_T,base_process_T,exp_process_integer_T,exp_process_unsigned_integer_T>::template eval_with_base<default_base,default_exp_char,cs...>());
			}
		};
	public:
		template<unsigned default_base,char default_exp_char,char...cs>
		static constexpr_as_auto float_T eval_with_base()noexcept{
			return eval_differ<default_base,default_exp_char,cs...>::eval();
		}
		template<char...cs>
		static constexpr_as_auto float_T eval()noexcept{
			return eval_with_base<10,'e',cs...>();
		}
	};
}

//file_end

