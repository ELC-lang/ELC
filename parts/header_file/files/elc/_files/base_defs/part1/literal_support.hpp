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
	using ::std::move;

	template<class integer_T,unsigned base>
	class integer_iterator{
		integer_T& _val;
	public:
		constexpr integer_iterator(integer_T& val)noexcept:_val(val){}
		constexpr_as_auto void operator()(char c)noexcept{
			_val*=base;
			_val+=hexval(c);
		}
	};
	template<class integer_T,unsigned base>
	class fast_integer_iterator{
		size_t _var_block=0,_scale=1;
		integer_T& _val;
	public:
		constexpr fast_integer_iterator(integer_T& val)noexcept:_val(val){}
		constexpr_as_auto void operator()(char c)noexcept{
			const size_t new_scale=_scale*base;
			if(_scale>new_scale){//溢出，建立新块
				_val=move(_val)*_scale+_var_block;
				_var_block=hexval(c);
				_scale=base;
			}
			else{
				_var_block=_var_block*base+hexval(c);
				_scale=new_scale;
			}
		}
		constexpr_as_auto ~fast_integer_iterator()noexcept{
			_val=move(_val)*_scale+_var_block;
		}
	};

	template<class integer_T,unsigned base>
	using integer_iterator_t=conditional<is_big_type<integer_T>,fast_integer_iterator<integer_T,base>,integer_iterator<integer_T,base>>;

	template<unsigned base>
	constexpr void base_checker()noexcept{
		static_assert(base==16||base==10||base==8||base==2,"base must be 16,10,8 or 2");
	}
	template<unsigned base,char c>
	constexpr void char_checker()noexcept{
		static_assert(c>='0'&&c<='9'||c>='a'&&c<='f'||c>='A'&&c<='F',"invalid char");
		static_assert(base>hexval(c),"invalid char");
	}

	template<class integer_T>
	class unsigned_integer_literal_evaler{
		template<unsigned base>
		static constexpr_as_auto void eval_impl(integer_iterator_t<integer_T,base>&)noexcept{}
		template<unsigned base,char c,char...cs>
		static constexpr_as_auto void eval_impl(integer_iterator_t<integer_T,base>&iter)noexcept{
			if constexpr(c!='\''){
				char_checker<base,c>();
				iter(c);
			}
			eval_impl<base,cs...>(iter);
		}

		template<unsigned base,char...cs>
		static constexpr_as_auto integer_T do_eval()noexcept{
			base_checker<base>();
			integer_T val{};
			{
				integer_iterator_t<integer_T,base> iter(val);
				eval_impl<base,cs...>(iter);
			}
			return move(val);
		}

		template<unsigned default_base,char...cs>
		struct eval_differ{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<default_base,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','x',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<16,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','X',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<16,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','b',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<2,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0','B',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<2,cs...>();
			}
		};
		template<unsigned default_base,char...cs>
		struct eval_differ<default_base,'0',cs...>{
			static constexpr_as_auto integer_T eval()noexcept{
				return do_eval<8,cs...>();
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
		static constexpr_as_auto void eval_impl(integer_iterator_t<base_process_T,base>&,exp_process_integer_T&)noexcept{}
		template<unsigned base,char exp_char,bool pointed,char c,char...cs>
		static constexpr_as_auto void eval_impl(integer_iterator_t<base_process_T,base>&iter,exp_process_integer_T&exp)noexcept{
			if constexpr(c=='.'){
				static_assert(!pointed,"pointed twice");
				eval_impl<base,exp_char,true,cs...>(iter,exp);
			}
			elseif constexpr(c==exp_char || c==exp_char+('A'-'a')){
				exp+=signed_integer_literal_evaler<exp_process_integer_T,exp_process_unsigned_integer_T>::template eval_with_base<base,cs...>();
				eval_impl<base,exp_char,pointed>(iter,exp);
			}
			else{
				if constexpr(c!='\''){
					if constexpr(pointed)
						--exp;
					char_checker<base,c>();
					iter(c);
				}
				eval_impl<base,exp_char,pointed,cs...>(iter,exp);
			}
		}

		template<unsigned base,char exp_char,char...cs>
		static constexpr_as_auto float_T do_eval()noexcept{
			base_checker<base>();
			base_process_T val{};exp_process_integer_T exp{};
			{
				integer_iterator_t<base_process_T,base> iter(val);
				eval_impl<base,exp_char,false,cs...>(iter,exp);
			}
			float_T aret=move(val);
			aret*=pow(base,move(exp));
			return move(aret);
		}

		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ{
			static constexpr_as_auto float_T eval()noexcept{
				return do_eval<default_base,default_exp_char,cs...>();
			}
		};
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ<default_base,default_exp_char,'0','x',cs...>{
			static constexpr_as_auto float_T eval()noexcept{
				return do_eval<16,'p',cs...>();
			}
		};
		template<unsigned default_base,char default_exp_char,char...cs>
		struct eval_differ<default_base,default_exp_char,'0','X',cs...>{
			static constexpr_as_auto float_T eval()noexcept{
				return do_eval<16,'p',cs...>();
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

