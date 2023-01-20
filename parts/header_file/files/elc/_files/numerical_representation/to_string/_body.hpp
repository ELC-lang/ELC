//_body.hpp
//at namespace elc::defs::numerical_representation_n
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace to_string_n{
	struct convert_state_t{
		//转换状态
		//用于记录转换过程中的状态
		/// 是否成功
		bool success=1;
		/// 有限小数？
		bool is_finite=1;
		/// 纯整数？
		bool is_integer=1;
	};
	template<numerical_representation representation_t>
	class string_convert_impl{
		//以任意的给定进制类型转换为字符串
		const representation_t& _repres;
	public:
		constexpr string_convert_impl(const representation_t&repres)noexcept:_repres(repres){}
	private:
		[[nodiscard]]string to_string_base(ubigfloat num)const noexcept{
			string aret;
			//判断是否是有限小数
			num.simplify();
			const bool is_finite=_repres.is_finite(num);
			//若无限小数，以分数形式输出
			if(!is_finite)[[unlikely]]{
				auto&numpart=get_numerator_as_ref(num);
				auto&denopart=get_denominator_as_ref(num);
				auto fractional_separator=_repres.get_fractional_separator();
				aret=to_string(move(numpart))+fractional_separator+to_string(move(denopart));
			}
			//有限小数
			else[[likely]]{
				ptrdiff_t exp=0;
				auto& numerator=get_numerator_as_ref(num);
				auto& denominator=get_denominator_as_ref(num);
				const auto radix=_repres.get_radix();
				//化简为numerator*radix^exp
				{
					auto result=divmod(numerator,radix);
					while(result.quot&&!(result.mod)){
						numerator=move(result.quot);
						++exp;
						result=divmod(numerator,radix);
					}
					result=divmod(denominator,radix);
					while(result.quot&&!(result.mod)){
						denominator=move(result.quot);
						--exp;
						result=divmod(denominator,radix);
					}
					numerator*=_repres.get_denominator_complement(denominator,exp);
					//现在denominator是1
					#if defined(_DEBUG)
						if(denominator!=1u)
							__debugbreak();
					#endif
				}
				//先获取numerator和exp的字符串表示
				aret=to_string(move(numerator));
				auto expstr=to_string(exp);
				/*两种情况：
					若exp是负数，在exp的绝对值大于aret.size+expstr.size()+1时使用科学计数法有意义
					若exp正数，在exp大于expstr.size()+1时使用科学计数法有意义
				*/
				if(exp<0){
					if(size_t(-exp)>aret.size()+expstr.size()+1){
						aret+=_repres.get_exponent_separator();
						aret+=expstr;
					}
					else{
						//否则，插入小数点
						//首先计算需要的前置0的个数
						const ptrdiff_t need_zero=(-exp)-aret.size()+1;
						if(need_zero>0)
							aret.push_front(_repres.get_char(0),need_zero);
						//插入小数点
						const auto dot_pos=aret.size()+exp;
						aret.insert(dot_pos,_repres.get_fractional_sign());
					}
				}
				elseif(exp>0){
					if(size_t(exp)>expstr.size()+1){
						aret+=_repres.get_exponent_separator();
						aret+=expstr;
					}
					else//否则，补0
						aret.push_back(_repres.get_char(0),exp);
				}
			}
			return aret;
		}
		[[nodiscard]]string to_string_unsigneded(ubigfloat num)const noexcept{
			//首先，特殊值检查
			if(isNaN(num))
				return _repres.get_nan();
			if(isinf(num))
				return _repres.get_inf();
			return to_string_base(move(num));
		}
	public:
		[[nodiscard]]string to_string(const bigfloat&num)const noexcept{
			if(is_negative(num))
				return _repres.get_negative_sign()+to_string_unsigneded(abs(num));
			else
				return to_string_unsigneded(abs(num));
		}
		[[nodiscard]]string to_string(bigfloat&&num)const noexcept{
			if(is_negative(num))
				return _repres.get_negative_sign()+to_string_unsigneded(abs(move(num)));
			else
				return to_string_unsigneded(abs(move(num)));
		}
		[[nodiscard]]string to_string(const ubigfloat&num)const noexcept{
			return to_string_unsigneded(num);
		}
		[[nodiscard]]string to_string(ubigfloat&&num)const noexcept{
			return to_string_unsigneded(move(num));
		}
		template<integer_type T>//适用于任何c++整数类型以及elc的bigint和ubigint
		[[nodiscard]]string to_string(T num)const noexcept{
			if constexpr(type_info<T> == type_info<bool>)
				return to_string(union_cast<unsigned_specific_size_t<sizeof(bool)>>(num));
			elseif constexpr(is_signed<T>){
				if(is_negative(num))
					return _repres.get_negative_sign()+to_string(abs(num));
				else
					return to_string(abs(num));
			}
			else{
				string aret;
				const auto radix=_repres.get_radix();
				push_and_disable_msvc_warning(4244);
				do{//do-while是为了保证至少有一位"0"
					auto res=divmod(num,radix);
					const auto index=to_size_t(move(res.mod));
					const auto ch=_repres.get_char(index);
					#if defined(_DEBUG)
						if(!_repres.is_valid_char(ch))
							__debugbreak();
					#endif
					aret.push_front(ch);
					num=move(res.quot);
				}while(num);
				pop_msvc_warning();
				return aret;
			}
		}
	private:
		template<class T> requires ::std::is_floating_point_v<T>
		[[nodiscard]]string to_string_unsigneded(T num)const noexcept{
			return to_string_base(move(num));
		}
	public:
		template<class T> requires ::std::is_floating_point_v<T>
		[[nodiscard]]string to_string(T num)const noexcept{
			//首先，特殊值检查
			if(isNaN(num)){
				if constexpr(::std::numeric_limits<T>::has_signaling_NaN){
					constexpr auto signaling_NaN = ::std::numeric_limits<T>::signaling_NaN();
					if(full_equal_in_byte(signaling_NaN,num)){
						return _repres.get_signaling_nan();
					}
					auto negative_signaling_NaN = copy_as_negative(signaling_NaN);
					if(full_equal_in_byte(negative_signaling_NaN,num)){
						return string()+_repres.get_negative_sign()+_repres.get_signaling_nan();
					}
				}
				if constexpr(::std::numeric_limits<T>::has_quiet_NaN){
					constexpr auto quiet_NaN = ::std::numeric_limits<T>::quiet_NaN();
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
			if constexpr(::std::numeric_limits<T>::has_infinity){
				constexpr auto infinity = ::std::numeric_limits<T>::infinity();
				if(full_equal_in_byte(infinity,num)){
					return _repres.get_inf();
				}
				auto negative_infinity = copy_as_negative(infinity);
				if(full_equal_in_byte(negative_infinity,num)){
					return string()+_repres.get_negative_sign()+_repres.get_inf();
				}
			}
			if(is_negative(num))
				return _repres.get_negative_sign()+to_string_unsigneded(-num);
			else
				return to_string_unsigneded(num);
		}
	private:
		//to_string定义完了，开始定义from_string_get
		template<class T> requires(type_info<T> == type_info<ubigfloat>)
		[[nodiscard]]ubigfloat from_string_get_base(const string&str,convert_state_t&state)const noexcept{
			state={};
			ubigint base;
			const auto radix=_repres.get_radix();
			const auto end=str.end();
			auto i=str.begin();
			for(;i!=end;++i){
				char_t ch=*i;
				if(ch==_repres.get_fractional_sign()){//小数：123.456
					state.is_integer=false;
					++i;
					ubigint radix_power=1u;
					for(;i!=end;++i){
						radix_power*=radix;
						base*=radix;
						ch=*i;
						if(ch==_repres.get_exponent_separator()){//科学计数法：123.456e789
							++i;
							auto expstr=string{i,end};
							auto exp=from_string_get<ptrdiff_t>(expstr,state);
							if(!state.success)
								return {};
							ubigfloat aret{move(base)};
							aret/=radix_power;
							while(exp<0){
								aret/=radix;
								++exp;
							}
							while(exp>0){
								aret*=radix;
								--exp;
							}
							return aret;
						}
						if(_repres.is_valid_char(ch))
							base+=_repres.get_index(ch);
						else{//error
							state.success=false;
							return {};
						}
					}
					ubigfloat aret{move(base)};
					aret/=radix_power;
					return aret;
				}
				elseif(ch==_repres.get_fractional_separator()){//分数：123/456
					state.is_integer=false;
					state.is_finite=false;
					++i;
					ubigint denopart;
					for(;i!=end;++i){
						denopart*=radix;
						ch=*i;
						if(_repres.is_valid_char(ch))
							denopart+=_repres.get_index(ch);
						else{//error
							state.success=false;
							return {};
						}
					}
					ubigfloat aret{move(base)};
					aret/=denopart;
					return aret;
				}
				elseif(ch==_repres.get_exponent_separator()){//科学计数法：123e456
					++i;
					auto expstr=string(i,end);
					auto exp=from_string_get<ptrdiff_t>(expstr,state);
					if(!state.success)
						return {};
					ubigfloat aret{move(base)};
					while(exp<0){
						aret/=radix;
						++exp;
					}
					while(exp>0){
						aret*=radix;
						--exp;
					}
					return aret;
				}
				base*=radix;
				if(_repres.is_valid_char(ch))
					base+=_repres.get_index(ch);
				else{//error
					state.success=false;
					return {};
				}
			}
			return base;//这里直接他妈的返回了一个整数
		}
		//ubigfloat
		template<class T> requires(type_info<T> == type_info<ubigfloat>)
		[[nodiscard]]ubigfloat from_string_get_unsigneded(const string&str,convert_state_t&state)const noexcept{
			//首先判断特殊值
			if(str==_repres.get_nan()){
				state={};
				return ::std::numeric_limits<double>::quiet_NaN();
			}
			elseif(str==_repres.get_inf()){
				state={};
				return ::std::numeric_limits<double>::infinity();
			}
			else
				return from_string_get_base<ubigfloat>(str,state);
		}
	public:
		template<class T> requires(type_info<T> == type_info<ubigfloat>)
		[[nodiscard]]ubigfloat from_string_get(const string&str,convert_state_t&state)const noexcept{
			return from_string_get_unsigneded<ubigfloat>(str,state);
		}
		//floats
	private:
		template<class T> requires ::std::is_floating_point_v<T>
		[[nodiscard]]T from_string_get_unsigneded(string str,convert_state_t&state)const noexcept{
			//首先判断特殊值
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
				if constexpr(::std::numeric_limits<T>::has_signaling_NaN)
					if(str==_repres.get_signaling_nan()){
						state={};
						return ::std::numeric_limits<T>::signaling_NaN();
					}
				if constexpr(::std::numeric_limits<T>::has_quiet_NaN)
					if(str==_repres.get_quiet_nan()){
						state={};
						return ::std::numeric_limits<T>::quiet_NaN();
					}
				if(str.starts_with(_repres.get_nan())){
					str.remove_front(_repres.get_nan().size());
					if(str && str.back()==_repres.get_unknown_data_start_sign()){
						state={};
						str.remove_front();
						str.remove_back();
						data_block<T> block;
						size_t		  write_index = 0;

						const auto radix = _repres.get_radix();
						const auto unknown_data_split_sign = _repres.get_unknown_data_split_sign();
						const bool needs_split_sign = radix < number_of_possible_values_per<char>;
						while(state.success){
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
						return state.success?data_cast<T>(block):T{};
					}
					else{
						state={};
						return ::std::numeric_limits<T>::quiet_NaN();
					}
				}
			}
			if constexpr(::std::numeric_limits<T>::has_infinity)
				if(str==_repres.get_inf()){
					state={};
					return ::std::numeric_limits<T>::infinity();
				}
			//由于基本类型的浮点数转换实现有损，所以这里直接转换为ubigfloat再转换为T
			return from_string_get_base<ubigfloat>(move(str),state).convert_to<T>();
		}
	public:
		template<class T> requires(::std::is_floating_point_v<T> && ::std::is_unsigned_v<T>)
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
				push_and_disable_msvc_warning(4267);
				T aret={};
				const auto radix=_repres.get_radix();
				const auto end=str.end();
				for(auto i=str.begin();i!=end;++i){
					aret*=radix;
					const char_t ch=*i;
					if(_repres.is_valid_char(ch))
						aret+=_repres.get_index(ch);
					else{//error
						state.success=false;
						return {};
					}
				}
				return aret;
				pop_msvc_warning();
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
			if(_repres.is_valid_char(ch)){
				state={};
				return(T)_repres.get_index(ch);
			}
			else{
				state.success=false;
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
			return copy_as_negative(from_string_get_unsigneded<UT>(move(str),state),is_negative);
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
		[[nodiscard]]string operator()(const T&val,const Repres&repres)const noexcept{
			const string_convert_impl<Repres> impl{repres};
			return impl.to_string(val);
		}
		template<arithmetic_type T>
		[[nodiscard]]string operator()(const T&val)const noexcept{
			return (*this)(val,decimal);
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
