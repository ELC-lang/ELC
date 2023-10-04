//_body.hpp
//at namespace elc::defs::numerical_representation_n
/*
未完成的elc解释器numerical_representation文件
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
namespace to_string_n{
	//用于记录转换过程中的状态
	class convert_state_t{
		//是否出错
		bool _error=false;
		//是否出现了非法字符
		bool _has_invalid_char=false;
		//含有分数？
		bool _has_fractional=false;
		//含有小数？
		bool _has_decimal=false;
		//含有指数？
		bool _has_exponent=false;
	public:
		constexpr convert_state_t()noexcept=default;
		constexpr convert_state_t(const convert_state_t&)=default;
		constexpr convert_state_t(convert_state_t&&)=default;
		constexpr convert_state_t&operator=(const convert_state_t&)=default;
		constexpr convert_state_t&operator=(convert_state_t&&)=default;
		~convert_state_t()noexcept=default;
		void reset()noexcept{
			*this=convert_state_t{};
		}
		[[nodiscard]]constexpr bool error()const noexcept{return _error;}
		[[nodiscard]]constexpr bool success()const noexcept{return !error();}
		[[nodiscard]]constexpr bool has_invalid_char()const noexcept{return _has_invalid_char;}
		[[nodiscard]]constexpr bool has_fractional()const noexcept{return _has_fractional;}
		[[nodiscard]]constexpr bool has_decimal()const noexcept{return _has_decimal;}
		[[nodiscard]]constexpr bool has_exponent()const noexcept{return _has_exponent;}
		constexpr void set_error()noexcept{_error=true;}
		constexpr void set_has_invalid_char()noexcept{_has_invalid_char=true;}
		constexpr void set_has_fractional()noexcept{_has_fractional=true;}
		constexpr void set_has_decimal()noexcept{_has_decimal=true;}
		constexpr void set_has_exponent()noexcept{_has_exponent=true;}
	};
	template<numerical_representation representation_t>
	class string_convert_impl{
		//以任意的给定进制类型转换为字符串
		const representation_t& _repres;
	public:
		constexpr string_convert_impl(const representation_t&repres)noexcept:_repres(repres){}
	private:
		//基础部分
		[[nodiscard]]constexpr string build_expression_str(string base,ptrdiff_t exp)const noexcept{
			if(exp<0){
				auto exp_str=to_string(exp);
				//首先计算需要的前置0的个数
				const auto need_zero=size_t(max.for_type<ptrdiff_t>(0,-exp+1-base.size()));
				if(need_zero>exp_str.size()){//选取最短表达
					base+=_repres.get_exponent_separator();
					base+=exp_str;
				}
				else{
					if(need_zero)
						base.push_front(_repres.get_char(0),need_zero);
					//插入小数点
					const auto dot_pos=base.size()+exp;
					base.insert(dot_pos,_repres.get_fractional_sign());
				}
			}
			elseif(exp>0)
				base=build_expression_str(base,size_t(exp));
			return base;
		}
		[[nodiscard]]constexpr string build_expression_str(string base,size_t exp)const noexcept{
			if(exp){
				auto exp_str=to_string(exp);
				if(exp>exp_str.size()+1){
					base+=_repres.get_exponent_separator();
					base+=exp_str;
				}
				else//否则，补0
					base.push_back(_repres.get_char(0),exp);
			}
			return base;
		}
		//整数部分
	public:
		template<integer_type T>//适用于任何c++整数类型以及elc的bigint和ubigint
		[[nodiscard]]string to_string(T num)const noexcept{
			if constexpr(type_info<T> == type_info<bool>)
				return to_string(union_cast<unsigned_specific_size_t<sizeof(bool)>>(move(num)));
			elseif constexpr(is_signed<T>){
				if(is_negative(num))
					return _repres.get_negative_sign()+to_string(abs(move(num)));
				else
					return to_string(abs(move(num)));
			}
			else
				return to_string_unsigneded(move(num));
		}
	private:
		template<unsigned_integer_type T>//适用于任何c++无符号整数类型以及elc的ubigint
		[[nodiscard]]string to_string_unsigneded(T num)const noexcept{
			return to_string_base<T>(move(num));
		}
		template<unsigned_integer_type T>//适用于任何c++无符号整数类型以及elc的ubigint
		[[nodiscard]]string to_string_with_exp(T num)const noexcept{
			ptrdiff_t exp;
			auto aret=to_string_and_set_exp(move(num),exp);
			return build_expression_str(move(aret),exp);
		}
		template<unsigned_integer_type T>//适用于任何c++无符号整数类型以及elc的ubigint
		[[nodiscard]]string to_string_and_set_exp(T num,ptrdiff_t&exp)const noexcept{
			return to_string_and_add_exp(move(num),exp=0);
		}
		template<unsigned_integer_type T>//适用于任何c++无符号整数类型以及elc的ubigint
		[[nodiscard]]string to_string_and_add_exp(T num,ptrdiff_t&exp)const noexcept{
			const auto radix=_repres.get_radix();
			while(num){
				auto result=divmod(num,radix);
				if(result.mod)
					break;
				num=T(move(result.quot));
				++exp;
			}
			return to_string_base(move(num));
		}
		template<unsigned_integer_type T>//适用于任何c++无符号整数类型以及elc的ubigint
		[[nodiscard]]string to_string_base(T num)const noexcept{
			string aret;
			const auto radix=_repres.get_radix();
			if constexpr(is_basic_type<T>){
				//基本类型有最大值，可以预分配足够的空间来提高效率
				constexpr auto info_threshold_base = pow(BIT_POSSIBILITY, bitnum_of(T));
				const auto info_threshold = to_size_t(ceil(log(info_threshold_base, radix)));
				aret.pre_alloc_before_begin(info_threshold);
			}
			if constexpr(is_big_type<T>){
				//大整数类型可以通过分治法来提高效率
				constexpr auto partition_method_threshold=max(type_info<size_t>);
				if(num > partition_method_threshold){
					T base{radix};
					size_t len=1;//余数部分要补的前导0个数
					//计算分割点
					while(base.memory_usage()*3 < num.memory_usage()){
						len *= 2;
						base *= base;
					}
					//算出分割后的高位和低位
					auto [high,low] = divmod(num, base);
					return to_string(move(high)) + to_string(move(low)).pad_left(_repres.get_char(0), len);
				}
				else
					return to_string(num.convert_to<size_t>());
			}
			else{
				push_and_disable_msvc_warning(4244);
				do{//do-while是为了保证至少有一位"0"
					auto res=divmod(move(num),radix);
					const auto index=to_size_t(move(res.mod));
					const auto ch=_repres.get_char(index);
					aret.push_front(ch);
					num=move(res.quot);
				}while(num);
				pop_msvc_warning();
				return aret;
			}
		}
		//浮点数部分
		template<float_type T>
		[[nodiscard]]string to_string_NaN_handle(T num)const noexcept{
			if constexpr(is_basic_type<T>){
				if constexpr(arithmetic_type_info_prover<T>::has_signaling_NaN){
					constexpr auto signaling_NaN = arithmetic_type_info_prover<T>::signaling_NaN();
					if(full_equal_in_byte(signaling_NaN,num)){
						return _repres.get_signaling_nan();
					}
					auto negative_signaling_NaN = copy_as_negative(signaling_NaN);
					if(full_equal_in_byte(negative_signaling_NaN,num)){
						return string()+_repres.get_negative_sign()+_repres.get_signaling_nan();
					}
				}
				if constexpr(arithmetic_type_info_prover<T>::has_quiet_NaN){
					constexpr auto quiet_NaN = arithmetic_type_info_prover<T>::quiet_NaN();
					if(full_equal_in_byte(quiet_NaN,num)){
						return _repres.get_quiet_nan();
					}
					auto negative_quiet_NaN = copy_as_negative(quiet_NaN);
					if(full_equal_in_byte(negative_quiet_NaN,num)){
						return string()+_repres.get_negative_sign()+_repres.get_quiet_nan();
					}
				}
				string aret=_repres.get_nan();
				aret+=_repres.get_unknown_data_start_sign();
				const auto radix = _repres.get_radix();
				const auto unknown_data_split_sign = _repres.get_unknown_data_split_sign();
				const bool needs_split_sign = radix < number_of_possible_values_per<char>;
				data_view<T> view{&num};
				for(const byte c: view){
					aret += to_string((unsigned char)c);
					if(needs_split_sign)
						aret += unknown_data_split_sign;
				}
				if(needs_split_sign)
					aret.back()=_repres.get_unknown_data_end_sign();
				else
					aret+=_repres.get_unknown_data_end_sign();
				return aret;
			}
			else
				return _repres.get_nan();
		}
		template<float_type T>
		[[nodiscard]]string to_string_Inf_handle(T num)const noexcept{
			if constexpr(is_basic_type<T>){
				constexpr auto infinity = arithmetic_type_info_prover<T>::Inf();
				if(full_equal_in_byte(infinity,num))
					return _repres.get_inf();
				else
					return string()+_repres.get_negative_sign()+_repres.get_inf();
			}
			else{
				if(is_negative(num))
					return _repres.get_negative_sign()+_repres.get_inf();
				else
					return _repres.get_inf();
			}
		}
	public:
		template<float_type T>
		[[nodiscard]]string to_string(T num)const noexcept{
			//首先，特殊值检查
			if(isNaN(num))
				return to_string_NaN_handle(move(num));
			elseif(isInf(num))
				return to_string_Inf_handle(move(num));
			//然后分符号处理
			if(is_negative(num))
				return _repres.get_negative_sign()+to_string_unsigneded(abs(move(num)));
			else
				return to_string_unsigneded(abs(move(num)));
		}
		template<fraction_float_type T>//适用于任何分数法记录的浮点类型
		[[nodiscard]]string to_string_fractional(T num)const noexcept{
			//首先，特殊值检查
			if(isNaN(num))
				return to_string_NaN_handle(move(num));
			elseif(isInf(num))
				return to_string_Inf_handle(move(num));
			//然后分符号处理
			if(is_negative(num))
				return _repres.get_negative_sign()+to_string_fractional_base(abs(move(num)));
			else
				return to_string_fractional_base(abs(move(num)));
		}
	private:
		template<float_type T>
		[[nodiscard]]string to_string_unsigneded(T num)const noexcept{
			return to_string_base(move(num));
		}
		template<float_type T,unsigned_integer_type num_t>
		[[nodiscard]]inline void apply_info_threshold(num_t&num,ptrdiff_t&exp,const T&check_num)const noexcept{
			if constexpr(is_basic_type<T>){//若T是基础类型
				const auto radix=_repres.get_radix();
				//根据basic_environment::float_infos::precision_base<T>*2*radix计算阈值
				constexpr auto info_threshold_base=basic_environment::float_infos::precision_base<T>*2;
				//获取info_threshold_num
				const auto info_threshold_num=info_threshold_base*radix;
				{
					//这里我们需要一次生成radix^x大小的数来让num快速减小，以加速后续的循环
					//但不能通过上文中的info_threshold_base计算，因为适当的舍入是必要的，所以需要一个更为宽松但又合适的阈值
					//因此我们需要一个新的阈值，根据信息论第一定理，我们可以假设新的阈值基础为unsigned_specific_size_t<sizeof(T)>所容纳的信息量
					//其比info_threshold_base更为宽松，但又合适于限制num的大小
					typedef unsigned_specific_size_t<sizeof(T)> new_info_threshold_base;
					//计算新的阈值
					const auto info_threshold_of_radix_num=to_size_t(ceil(log(max(type_info<new_info_threshold_base>),radix)));
					const auto num_of_radix=integer_log(num,radix);
					const ptrdiff_t diff=num_of_radix-info_threshold_of_radix_num;
					if(diff>0){
						num/=pow(num_t{radix},size_t(diff));
						exp+=diff;
					}
				}
				//舍入相关
				const auto rounding_threshold=radix/2;
				auto rounding=exlambda(bool skip_zeros,auto backup_updater,auto exp_updater)noexcept{
					auto result=divmod(move(num),radix);
					if(skip_zeros)//处理多余的0
						if(!result.mod){
							backup_updater(assign(num,move(result.quot)),++exp);
							exp_updater(exp);
							result=divmod(move(num),radix);
						}
					if(result.mod>=rounding_threshold)//舍入
						++result.quot;
					num=move(result.quot);
					exp_updater(++exp);
				};
				//更新exp并舍入num直到num小于info_threshold_num
				while(num>info_threshold_num)
					rounding(false,do_nothing,do_nothing);
				//额外的舍入检查为了更精确的处理
				if(num>info_threshold_base){
					auto num_backup=num;auto exp_backup=exp;
					auto backup_updater=exlambda(const auto&new_num,const auto&new_exp)noexcept{
						num_backup=new_num;exp_backup=new_exp;
					};
					auto exp_pows=pow((ubigint)radix,exp);
					auto exp_updater=exlambda(const auto&)noexcept{exp_pows*=radix;};
					floop{
						//首先进行舍入
						rounding(true,backup_updater,exp_updater);
						//然后判断是否需要继续
						auto tmp_bigfloat=num*exp_pows;
						if(tmp_bigfloat.convert_to<T>()==check_num)
							backup_updater(num,exp);
						else
							break;
					}
					num=move(num_backup);exp=move(exp_backup);
				}
			}
		}
		template<float_type T,unsigned_integer_type num_base_t>
		[[nodiscard]]string string_builder(T num,num_base_t number,ptrdiff_t&exp)const noexcept{
			//根据类型需求对number进行截断
			apply_info_threshold<T>(number,exp,num);
			//先获取number的字符串
			auto tmp=to_string_and_add_exp(move(number),exp);
			//然后就着exp输出
			return build_expression_str(move(tmp),exp);
		}
		template<exponent_float_type T>//适用于任何指数法记录的浮点类型
		[[nodiscard]]string to_string_base(T num)const noexcept{
			//首先，猜测分数
			if(auto[numerator,denominator,divide_success]=to_divide(num);divide_success){
				if(denominator!=1u){//分母不为1：正规分数
					ptrdiff_t exp=0;
					const auto complete=_repres.get_denominator_complement(denominator,exp);
					//现在denominator是1？若不是则说明这是一个无限小数
					//若无限小数，以分数形式输出
					if(denominator!=1u){
						//先恢复denominator
						denominator*=complete;
						//呃，这里我们需要稍微处理一下numerator，考虑到其类型是T，而不是什么整数类型
						//若numerator小于size_t的最大值，那么我们就可以直接转换为size_t
						//否则，我们就需要转换为ubigint
						string aret;
						if(numerator<=max(type_info<size_t>))
							aret=to_string_with_exp(to_size_t(numerator));
						else
							aret=to_string_with_exp((ubigint)numerator);
						return aret+_repres.get_fractional_separator()+to_string_unsigneded(denominator);
					}
					//否则我们下落到以小数形式输出
				}
				//整数部分可以在这里处理，但是若进入ubigint处理会出现不必要的计算
				elseif(numerator<=max(type_info<size_t>))
					return to_string_with_exp(to_size_t(numerator));
				//所以在下面再处理特大整数的情况
			}
			//然后分情况处理
			const auto[precision,exponent]=get_precision_and_exponent(num);//声明个info和exp先
			ptrdiff_t exp=exponent;
			//首先判断进制是否是BIT_POSSIBILITY的整数倍
			if(_repres.is_bit_friendly_radix()){
				const auto radix=_repres.get_radix();
				const auto radix_diff=radix>>1;//用于补数
				//这边情况是比较好处理的，因为exp的base是BIT_POSSIBILITY，所以该数肯定是有限小数
				//所以我们需要做的就是，呃
				//首先新建一个base是radix的exp值，并将现有的exp降低为0
				//对于exp部分，塞入number
				ubigint number=precision;//避免溢出
				if(exp<0)
					number*=pow((ubigint)radix_diff,abs(exp));
				elseif(exp>0){
					auto result=divmod(number,radix_diff);
					ptrdiff_t new_exp=0;
					while(exp&&!result.mod){
						number=move(result.quot);
						--exp;
						++new_exp;
						result=divmod(number,radix_diff);
					}
					number<<=exp;
					exp=new_exp;
				}
				return string_builder(num,move(number),exp);
			}
			else{
				//可惜没有额外的优化
				//这里很好笑，因为输出进制不是bit的整数倍，这意味着你无法用有限小数表示这个数
				if(exp<0){
					//将num转换为分数，然后输出
					//需要化简分数吗？呃，虚空说答案是不
					const auto numerator=precision;
					const auto denominator=ubigint(1u)<<abs(exponent);
					return to_string_with_exp(numerator)+_repres.get_fractional_separator()+to_string_unsigneded(denominator);
				}
				else{
					ubigint number=precision;
					number<<=exp;
					return to_string_with_exp(number);
				}
			}
		}
		template<fraction_float_type T>//适用于任何分数法记录的浮点类型
		[[nodiscard]]string to_string_base(T num)const noexcept{
			simplify(num);//化简
			ptrdiff_t exp=0;
			auto& numerator=get_numerator_as_ref(num);
			if(!numerator)
				return _repres.get_char(0);
			auto& denominator=get_denominator_as_ref(num);
			if(denominator==1u)
				return to_string_with_exp(move(numerator));
			const auto radix=_repres.get_radix();
			{
				auto denominator_backup=denominator;//备份denominator用于在发现此为无限小数时继续输出（省去还原计算）
				auto clear_backup=exlambda()noexcept{denominator_backup=decltype(denominator_backup){};};//即时清空denominator_backup以节省内存
				//化简为numerator*radix^exp
				{
					auto result=divmod(denominator,radix);
					while(result.quot&&!result.mod){
						denominator=move(result.quot);
						--exp;
						result=divmod(denominator,radix);
					}
				}
				auto complete=_repres.get_denominator_complement(denominator,exp);
				//现在denominator是1？若不是则说明这是一个无限小数
				//若无限小数，以分数形式输出
				if(denominator!=1){
					auto fractional_separator=_repres.get_fractional_separator();
					denominator=move(denominator_backup);
					clear_backup();//考虑到右值赋值的实现很可能是swap，这个调用是有意义的
					return to_string(move(numerator))+fractional_separator+to_string(move(denominator));
				}
				clear_backup();
				numerator*=complete;
			}
			return string_builder(num,move(numerator),exp);
		}
		template<fraction_float_type T>//适用于任何分数法记录的浮点类型
		[[nodiscard]]string to_string_fractional_base(T num)const noexcept{
			auto& numerator=get_numerator_as_ref(num);
			if(!numerator)return _repres.get_char(0);
			auto& denominator=get_denominator_as_ref(num);
			array_t<remove_cvref<decltype(numerator)>> numerator_array;
			string result;
			{
				const auto info=divmod(numerator,denominator);
				result=to_string(info.quot);
				numerator=move(info.mod);
			}
			result+=_repres.get_fractional_sign();
			auto functional_part_begin=result.size();
			const auto radix=_repres.get_radix();
			while(numerator){
				numerator*=radix;
				const auto info=divmod(numerator,denominator);
				numerator=move(info.mod);
				if(auto index=numerator_array.find(numerator);index!=nullptr){
					result.insert(index-numerator_array.begin()+functional_part_begin,_repres.get_repetition_sign_start());
					result+=_repres.get_repetition_sign_end();
					break;
				}
				result+=_repres.get_char(to_size_t(info.quot));
				numerator_array.push_back(numerator);
			}
			return result;
		}
	private:
		//to_string定义完了，开始定义from_string_get
		template<class T> requires(type_info<T> == type_info<ubigfloat>)
		[[nodiscard]]ubigfloat from_string_get_base(string str,convert_state_t&state)const noexcept{
			{
				const auto exponent_pos = str.find_last_of(_repres.get_exponent_separator());
				if(exponent_pos != string::npos){
					state.set_has_exponent();
					auto expstr = str.substr(exponent_pos+1);
					str = str.substr(0,exponent_pos);
					auto exp = from_string_get<bigint>(expstr,state);
					if(not state.success())
						return {};
					auto base = from_string_get_base<T>(str,state);
					if(not state.success())
						return {};
					return base*pow(_repres.get_radix(),exp);
				}
			}
			{
				const auto fractional_pos = str.find_first_of(_repres.get_fractional_separator());//分数
				if(fractional_pos != string::npos){
					state.set_has_fractional();
					auto numeratorstr = str.substr(0,fractional_pos);
					str = str.substr(fractional_pos+1);
					const auto& denominatorstr = str;
					auto numerator = from_string_get_base<T>(numeratorstr,state);
					if(not state.success())
						return {};
					auto denominator = from_string_get_base<T>(denominatorstr,state);
					if(not state.success())
						return {};
					return numerator/denominator;
				}
			}
			{
				const auto fractional_pos = str.find_first_of(_repres.get_fractional_sign());//小数
				if(fractional_pos != string::npos){
					state.set_has_decimal();
					auto integerstr = str.substr(0,fractional_pos);
					str = str.substr(fractional_pos+1);
					const auto& fractionalstr = str;
					auto integer = from_string_get<ubigint>(integerstr,state);
					if(not state.success())
						return {};
					auto fractional = from_string_get<ubigint>(fractionalstr,state);
					if(not state.success())
						return {};
					auto saclen = pow(ubigint{_repres.get_radix()},fractionalstr.size());
					auto numerator = integer*saclen+fractional;
					auto denominator = saclen;
					return ubigfloat::build_from_numerator_and_denominator(numerator, denominator);
				}
			}
			//纯纯的整数
			return from_string_get<ubigint>(str,state);
		}
		//ubigfloat
		template<class T> requires(type_info<T> == type_info<ubigfloat>)
		[[nodiscard]]ubigfloat from_string_get_unsigneded(const string&str,convert_state_t&state)const noexcept{
			//首先判断特殊值
			if(str==_repres.get_nan())
				return arithmetic_type_info_prover<double>::quiet_NaN();
			elseif(str==_repres.get_inf())
				return arithmetic_type_info_prover<double>::infinity();
			else
				return from_string_get_base<ubigfloat>(str,state);
		}
		//floats
	private:
		template<basic_float_type T>
		[[nodiscard]]T from_string_get_unsigneded(string str,convert_state_t&state)const noexcept{
			//首先判断特殊值
			if constexpr(has_NaN<T>){
				if constexpr(arithmetic_type_info_prover<T>::has_signaling_NaN)
					if(str==_repres.get_signaling_nan())
						return arithmetic_type_info_prover<T>::signaling_NaN();
				if constexpr(arithmetic_type_info_prover<T>::has_quiet_NaN)
					if(str==_repres.get_quiet_nan())
						return arithmetic_type_info_prover<T>::quiet_NaN();
				if(str.starts_with(_repres.get_nan())){
					str.remove_front(_repres.get_nan().size());
					if(str && str.back()==_repres.get_unknown_data_end_sign()){
						str.remove_front();
						str.remove_back();
						data_block<T> block;
						size_t		  write_index = 0;

						const auto radix = _repres.get_radix();
						const auto unknown_data_split_sign = _repres.get_unknown_data_split_sign();
						const bool needs_split_sign = radix < number_of_possible_values_per<char>;
						while(state.success()){
							if(needs_split_sign){
								const size_t dot_pos = str.find(unknown_data_split_sign);
								auto byte_str		 = str.substr(0, dot_pos);
								block[write_index++] = (byte)from_string_get_unsigneded<unsigned char>(byte_str,state);
								if(dot_pos == string::npos)
									break;
								str					 = str.substr(dot_pos+1);
							}
							else
								block[write_index++] = (byte)from_string_get_unsigneded<unsigned char>(str.pop_front(),state);
						}
						return state.success()?data_cast<T>(block):T{};
					}
					else
						return arithmetic_type_info_prover<T>::quiet_NaN();
				}
			}
			if constexpr(has_inf<T>)
				if(str==_repres.get_inf())
					return arithmetic_type_info_prover<T>::Inf();
			//由于基本类型的浮点数转换实现有损，所以这里直接转换为ubigfloat再转换为T：安全、大范围、稳健（尽管速度慢）
			return from_string_get_base<ubigfloat>(move(str),state).convert_to<T>();
		}
	public:
		template<class T> requires(float_type<T> && unsigned_type<T>)
		[[nodiscard]]T from_string_get(const string&str,convert_state_t&state)const noexcept{
			return from_string_get_unsigneded<T>(str,state);
		}
		//integers
	private:
		template<unsigned_integer_type T>
		[[nodiscard]]T from_string_get_unsigneded(const string&str,convert_state_t&state)const noexcept{
			if constexpr(type_info<T> == type_info<bool>)
				return union_cast<bool>(from_string_get<unsigned_specific_size_t<sizeof(bool)>>(str,state));
			else{
				const auto radix=_repres.get_radix();
				if constexpr(is_big_type<T>){
					//大整数类型可以通过分治法来提高效率
					const auto partition_method_threshold=trunc(log(max(type_info<size_t>),radix));
					if(str.size()>partition_method_threshold){
						T base{radix};
						size_t len=1;
						//计算分割点
						while(len*3 < str.size()){
							len *= 2;
							base *= base;
						}
						const auto split_pos=str.size()-len;
						string high_str=str.substr(0,split_pos);
						string low_str=str.substr(split_pos);
						T high=from_string_get_unsigneded<T>(high_str,state);
						if(!state.success())
							return {};
						T low=from_string_get_unsigneded<T>(low_str,state);
						if(!state.success())
							return {};
						return move(high)*move(base)+move(low);
					}
					else
						return from_string_get_unsigneded<size_t>(str,state);
				}
				else{
					push_and_disable_msvc_warning(4267);
					T aret={};
					const auto end=str.end();
					for(auto i=str.begin();i!=end;++i){
						aret*=radix;
						const char_t ch=*i;
						if(_repres.is_valid_char(ch))
							aret+=_repres.get_index(ch);
						else{//error
							state.set_error();
							state.set_has_invalid_char();
							return {};
						}
					}
					return aret;
					pop_msvc_warning();
				}
			}
		}
	public:
		template<unsigned_integer_type T>
		[[nodiscard]]T from_string_get(const string&str,convert_state_t&state)const noexcept{
			return from_string_get_unsigneded<T>(str,state);
		}
	private:
		//特殊：从单个字符中获取数值
		template<unsigned_type T>
		[[nodiscard]]T from_string_get_unsigneded(const char_t&ch,convert_state_t&state)const noexcept{
			if(_repres.is_valid_char(ch))
				return(T)_repres.get_index(ch);
			else{
				state.set_error();
				state.set_has_invalid_char();
				return {};
			}
		}
	public:
		template<unsigned_type T>
		[[nodiscard]]T from_string_get(const char_t&ch,convert_state_t&state)const noexcept{
			return from_string_get_unsigneded<T>(ch,state);
		}
		template<signed_type T>
		[[nodiscard]]T from_string_get(const char_t&ch,convert_state_t&state)const noexcept{
			typedef to_unsigned_t<T> UT;
			return(T)from_string_get_unsigneded<UT>(ch,state);
		}
		template<signed_type T>
		[[nodiscard]]T from_string_get(string str,convert_state_t&state)const noexcept{
			//处理可能的正负符号开头
			bool is_negative=false;
			if(str[0]==_repres.get_negative_sign()){
				is_negative=true;
				str.remove_front();
			}
			elseif(str[0]==_repres.get_positive_sign())
				str.remove_front();
			typedef to_unsigned_t<T> UT;
			auto unum=from_string_get_unsigneded<UT>(move(str),state);
			return is_negative?copy_as_negative(move(unum)):move(unum);
		}
		template<arithmetic_type T>
		[[nodiscard]]T from_string_get(const string&str)const noexcept{
			convert_state_t state;
			return from_string_get<T>(str,state);
		}
	};
	/// @brief to_string
	constexpr struct to_string_t{
		template<arithmetic_type T,numerical_representation Repres>
		[[nodiscard]]string operator()(T&&val,const Repres&repres)const noexcept{
			const string_convert_impl<Repres> impl{repres};
			return impl.to_string(forward<T>(val));
		}
		template<arithmetic_type T>
		[[nodiscard]]string operator()(T&&val)const noexcept{
			return (*this)(forward<T>(val),decimal);
		}
		template<size_t N,numerical_representation Repres>
		[[nodiscard]]string operator()(const bitset<N>&val,const Repres&repres)const noexcept{
			return (*this)(ubigint(val),repres).pad_left(N,repres.get_char(0));
		}
		/**
		 * 这个函数用于以小数形式转换分数法记录的浮点类型，并在循环小数部分的循环节上加上方括号
		 * e.g. to_string.fractional(1/3_bigfloat) -> "0.[3]"
		 * elc的stream并不会支持将这个函数设置为默认的输出方式，原因如下：
		 * 	1. 以小数形式输出一个无限循环小数尽管在某些情况下对人类阅读友好，但是大多数情况下我们不需要顾及这一点
		 * 	2. 这个函数在运行时维护一个不确定大小且可能很大的数组并在生成每一位时遍历它，拥有较高的时间和空间复杂度
		 * 	3. 这个函数在大多数情况下没有分数形式输出的结果字数少
		 * 	4. 处理大数的时候很容易炸掉（比如48456231736923095375908590957286289431354707638085/15424097609075667653528921705562303688163012378624）
		 * 在你需要更为可读的调试或者别的什么特殊需求的时候，大可显式地调用这个函数
		**/
		template<fraction_float_type T,numerical_representation Repres>
		[[nodiscard]]string fractional(T&&val,const Repres&repres)const noexcept{
			const string_convert_impl<Repres> impl{repres};
			return impl.to_string_fractional(forward<T>(val));
		}
		template<fraction_float_type T>//适用于任何分数法记录的浮点类型
		[[nodiscard]]string fractional(T&&val)const noexcept{
			return fractional(forward<T>(val),decimal);
		}
	}to_string{};
	/// @brief from_string_get
	template<arithmetic_type T>
	struct from_string_get_t{
		typedef convert_state_t state_t;
		template<numerical_representation Repres>
		[[nodiscard]]T operator()(const string&str,const Repres&repres,convert_state_t&state)const noexcept{
			const string_convert_impl<Repres> impl{repres};
			return impl.from_string_get<T>(str,state);
		}
		template<numerical_representation Repres>
		[[nodiscard]]T operator()(const string&str,const Repres&repres)const noexcept{
			convert_state_t state;
			return (*this)(str,repres,state);
		}
		[[nodiscard]]T operator()(const string&str,convert_state_t&state)const noexcept{
			return (*this)(str,decimal,state);
		}
		[[nodiscard]]T operator()(const string&str)const noexcept{
			return (*this)(str,decimal);
		}
		//单个字符
		template<numerical_representation Repres>
		[[nodiscard]]T operator()(const char_t&ch,const Repres&repres,convert_state_t&state)const noexcept{
			const string_convert_impl<Repres> impl{repres};
			return impl.from_string_get<T>(ch,state);
		}
		template<numerical_representation Repres>
		[[nodiscard]]T operator()(const char_t&ch,const Repres&repres)const noexcept{
			convert_state_t state;
			return (*this)(ch,repres,state);
		}
		[[nodiscard]]T operator()(const char_t&ch,convert_state_t&state)const noexcept{
			return (*this)(ch,decimal,state);
		}
		[[nodiscard]]T operator()(const char_t&ch)const noexcept{
			return (*this)(ch,decimal);
		}
	};
	template<arithmetic_type T>
	constexpr from_string_get_t<T>from_string_get{};
}
using to_string_n::to_string;
using to_string_n::from_string_get;

//file_end

