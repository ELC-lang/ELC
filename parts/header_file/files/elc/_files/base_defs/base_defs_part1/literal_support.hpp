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
				return -unsigned_integer_literal_evaler<unsigned_integer_T>::template eval_with_base<default_base,cs...>();
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
}

//file_end

