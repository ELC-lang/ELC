//defs.hpp
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

/*
客制化to_string用途类
*/
struct base_numerical_representation_t{};
/*
记录数字的表示方式以便客制化to_string
*/
class numerical_representation_t:public base_numerical_representation_t{
	size_t _radix=10;
	string _radix_table=es"0123456789"_constexpr_str;
	char_t _fractional_sign=ec('.');
	char_t _positive_sign=ec('+');
	char_t _negative_sign=ec('-');
	string _nan=es"NaN"_constexpr_str;
	string _signaling_nan=es"signaling_NaN"_constexpr_str;
	string _quiet_nan=es"quiet_NaN"_constexpr_str;
	string _inf=es"Inf"_constexpr_str;
	char_t _unknown_data_start_sign=ec('[');
	char_t _unknown_data_split_sign=ec(',');
	char_t _unknown_data_end_sign=ec(']');
public:
	numerical_representation_t()noexcept=default;
	numerical_representation_t(const numerical_representation_t&)noexcept=default;
	numerical_representation_t(numerical_representation_t&&)noexcept=default;
	numerical_representation_t(size_t radix,string radix_table,
							   char_t fractional_sign,
							   char_t positive_sign,char_t negative_sign,
							   string nan,string signaling_nan,string quiet_nan,
							   string inf,
							   char_t unknown_data_start_sign,char_t unknown_data_split_sign,char_t unknown_data_end_sign
							  )noexcept:
		_radix(radix),_radix_table(radix_table),
		_fractional_sign(fractional_sign),
		_positive_sign(positive_sign),_negative_sign(negative_sign),
		_nan(nan),_signaling_nan(signaling_nan),_quiet_nan(quiet_nan),
		_inf(inf),
		_unknown_data_start_sign(unknown_data_start_sign),_unknown_data_split_sign(unknown_data_split_sign),_unknown_data_end_sign(unknown_data_end_sign)
	{}
	numerical_representation_t(size_t radix,string radix_table)noexcept:
		_radix(radix),
		_radix_table(radix_table)
	{}
public:
	void set_radix(size_t radix)noexcept{
		_radix=radix;
	}
	void set_radix_table(string radix_table)noexcept{
		_radix_table=radix_table;
		_radix=radix_table.size();
	}
	void set_fractional_sign(char_t point)noexcept{
		_fractional_sign=point;
	}
	void set_positive_sign(char_t positive_sign)noexcept{
		_positive_sign=positive_sign;
	}
	void set_negative_sign(char_t negative_sign)noexcept{
		_negative_sign=negative_sign;
	}
	void set_nan(string nan)noexcept{
		_nan=nan;
	}
	void set_inf(string inf)noexcept{
		_inf=inf;
	}
	[[nodiscard]]constexpr size_t radix()const noexcept{
		return _radix;
	}
	[[nodiscard]]string radix_table()const noexcept{
		return _radix_table;
	}
	[[nodiscard]]constexpr char_t fractional_sign()const noexcept{
		return _fractional_sign;
	}
	[[nodiscard]]constexpr char_t positive_sign()const noexcept{
		return _positive_sign;
	}
	[[nodiscard]]constexpr char_t negative_sign()const noexcept{
		return _negative_sign;
	}
	[[nodiscard]]string nan()const noexcept{
		return _nan;
	}
	[[nodiscard]]string inf()const noexcept{
		return _inf;
	}
private:
	template<typename T>
	inline string to_string_num_base_mantissa(T num)const noexcept{
		string aret;
		//Information threshold相关声明
		//这限制了在当前radix下mantissa的最大长度，避免如radix=3而num=0.25时的无限循环
		constinit auto info_threshold_base = pow(BIT_POSSIBILITY,bitnumof<T>);
		auto		   info_threshold =	to_size_t(ceil(log(info_threshold_base,_radix)));
		//浮点数精度浮动，所以需要确定何时开始使用Info threshold
		bool is_mantissa_begined = false;
		while(num){
			//mantissa push部分
			num*=_radix;
			T first_char_index;
			num=::std::modf(num,&first_char_index);
			aret+=_radix_table[(size_t)first_char_index];
			//Information threshold制御部分
			if(to_size_t(first_char_index))
				is_mantissa_begined = true;
			if(is_mantissa_begined){
				info_threshold--;
				if(!info_threshold)
					break;
			}
		}
		return aret;
	}
	template<typename T>
	inline string to_string_num_base_integer(T num)const noexcept{
		string aret;
		do{//do while，在num为0时也有返值
			const auto first_char_index = mod(num, _radix);
			if constexpr(::std::is_floating_point_v<T>)
				num -= first_char_index;
			num /= (T)_radix;
			aret.push_front(_radix_table[(size_t)first_char_index]);
		} while(num);
		return aret;
	}
	template<typename T>
	inline bool to_string_special_value_check(T num,string&str,bool is_negative)const noexcept{
		if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
			if(::std::isnan(num)){
				if constexpr(::std::numeric_limits<T>::has_signaling_NaN) {
					constexpr auto signaling_NaN = ::std::numeric_limits<T>::signaling_NaN();
					if(full_equal_in_byte(signaling_NaN,num)){
						str=_signaling_nan;
						return true;
					}
					auto negative_signaling_NaN = copy_as_negative(signaling_NaN);
					if(full_equal_in_byte(negative_signaling_NaN,num)){
						str=_negative_sign+_signaling_nan;
						return true;
					}
				}
				if constexpr(::std::numeric_limits<T>::has_quiet_NaN) {
					constexpr auto quiet_NaN = ::std::numeric_limits<T>::quiet_NaN();
					if(full_equal_in_byte(quiet_NaN,num)){
						str=_quiet_nan;
						return true;
					}
					auto negative_quiet_NaN = copy_as_negative(quiet_NaN);
					if(full_equal_in_byte(negative_quiet_NaN,num)){
						str=_negative_sign+_quiet_nan;
						return true;
					}
				}
				str=_nan+_unknown_data_start_sign;
				data_view<T> view{&num};
				for(const byte c: view) {
					str += to_string_rough((unsigned char)c);
					str += _unknown_data_split_sign;
				}
				str.back() = _unknown_data_end_sign;
				return true;
			}
		}
		if constexpr(::std::numeric_limits<T>::has_infinity){
			if(::std::isinf(num)){
				if constexpr(!::std::is_unsigned_v<T>)
					if(is_negative) {
						str=_negative_sign+_inf;
						return true;
					}
				str=_inf;
				return true;
			}
		}
		return false;
	}
public:
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string_rough(T num)const noexcept{
		string aret;
		const bool is_negative=magic_number::is_negative(num);
		if(to_string_special_value_check(num,aret,is_negative))
			return aret;
		typedef decltype(lambda{
			if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
				return T();
			else
				return::std::make_unsigned_t<T>();
		}()) UT;
		//符号转无符号而不是num=-num避免INT_MAX这种情况下的溢出
		UT unum=UT(num);
		if constexpr(!::std::is_unsigned_v<T>)
			if(is_negative){
				aret=_negative_sign;
				unum=UT(-num);
			}
		UT num_fractional{};
		if constexpr(::std::is_floating_point_v<T>){
			num_fractional=::std::modf(unum,&unum);
			if(num_fractional && _radix==1)//1进制怎么可能有小数？
				return _nan;
		}
		aret+=to_string_num_base_integer(unum);
		if constexpr(::std::is_floating_point_v<T>)
			if(num_fractional)
				aret+=_fractional_sign+to_string_num_base_mantissa(num_fractional);
		return aret;
	}
private:
	template<typename T>
	inline T from_string_get_num_base_mantissa(string str)const noexcept{
		T aret{};
		size_t i=str.size();
		while(i){
			i--;
			const size_t index=_radix_table.find(str[i]);
			if(index==string::npos)
				return T();
			aret+=(T)index;
			aret/=_radix;
		}
		return aret;
	}
	template<typename T>
	inline T from_string_get_num_base_integer(string str) const noexcept {
		/*
		if constexpr(::std::is_floating_point_v<T>) {
			size_t order_of_magnitude = str.size();
			auto   num				  = from_string_get_num_base_mantissa<T>(str);
			auto final_num = num * pow(_radix, order_of_magnitude);
			return final_num;
		}
		else*/ {
			T aret{};
			for(size_t i = 0; i < str.size(); i++) {
				const size_t index = _radix_table.find(str[i]);
				if(index == string::npos)
					return T();
				aret *= (T)_radix;
				aret += (T)index;
			}
			return aret;
		}
	}
	template<typename T>
	inline bool from_string_special_value_check(string str,T& num,bool is_negative)const noexcept{
		if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN)
				if(str==_signaling_nan){
					num= copy_as_negative(::std::numeric_limits<T>::signaling_NaN(),is_negative);
					return true;
				}
			if constexpr(::std::numeric_limits<T>::has_quiet_NaN)
				if(str==_quiet_nan){
					num= copy_as_negative(::std::numeric_limits<T>::quiet_NaN(),is_negative);
					return true;
				}
			if(str.starts_with(_nan+_unknown_data_start_sign)){
				str.pop_front(_nan.size()+1);
				str.pop_back();
				data_block<T> block;
				size_t		  write_index = 0;
				floop{
					const size_t dot_pos = str.find(_unknown_data_split_sign);
					auto byte_str		 = str.substr(0, dot_pos);
					block[write_index++] = (byte)from_string_get<unsigned char>(byte_str);
					if(dot_pos == string::npos)
						break;
					str					 = str.substr(dot_pos+1);
				}
				num= data_cast<T>(block);
				return true;
			}
			elseif(str == _nan){
				if constexpr(::std::numeric_limits<T>::has_quiet_NaN)
					num= copy_as_negative(::std::numeric_limits<T>::quiet_NaN(),is_negative);
				else
					num= copy_as_negative(::std::numeric_limits<T>::signaling_NaN(),is_negative);
				return true;
			}
		}
		if constexpr(::std::numeric_limits<T>::has_infinity){
			if(str==_inf){
				if(is_negative)
					return copy_as_negative(::std::numeric_limits<T>::infinity());
				else
					return ::std::numeric_limits<T>::infinity();
				return true;
			}
		}
		return false;
	}
public:
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline T from_string_get(string str)const noexcept{
		T num{};
		suppress_msvc_warning(26496);
		bool is_negative=false;
		if constexpr(!::std::is_unsigned_v<T>)
			if(str[0]==_negative_sign){
				is_negative=true;
				str.pop_front();
			}
		if(str[0]==_positive_sign)
			str.pop_front();
		if(from_string_special_value_check(str,num,is_negative))
			return num;
		typedef decltype(lambda{
			if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
				return T();
			else
				return::std::make_unsigned_t<T>();
		}()) UT;
		UT unum{};
		if constexpr(::std::is_floating_point_v<T>) {
			const size_t dot_pos=str.find(_fractional_sign);
			if(dot_pos!=string::npos){
				if(_radix==1)//1进制怎么可能有小数点？
					return T();
				auto mantissa_str=str.substr(dot_pos+1);
				str=str.substr(0,dot_pos);
				unum=from_string_get_num_base_mantissa<UT>(mantissa_str);
			}
		}
		unum+=from_string_get_num_base_integer<UT>(str);
		return copy_as_negative<T>(unum,is_negative);
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string(T num)const noexcept{
		string aret;
		if(to_string_special_value_check(num,aret,is_negative(num)))
			return aret;
		aret=to_string_rough(num);
		if constexpr(::std::is_floating_point_v<T>){
			//需要纠正小数部分精度
			T tmp;
			if(::std::modf(num,&tmp)){//如果有小数部分
				const size_t dot_pos=aret.find(_fractional_sign);
				if(dot_pos == string::npos)
					return aret;//?

				//进位器
				auto rounding_up_char = lambda_with_catch(&) (string::arec_t char_arc)noexcept{
					const char_t up_char = move(char_arc);
					size_t up_pos  = _radix_table.find(up_char);
					up_pos++;
					if(up_pos == _radix)
						up_pos = 0;
					move(char_arc) = _radix_table[up_pos];
					return up_pos == 0;
				};
				auto rounding_up = lambda_with_catch(&) (string&str)noexcept{
					size_t i = str.size();
					while(i--){
						if(str[i] == _fractional_sign)
							i--;
						if(rounding_up_char(str[i]))
							continue;
						else
							break;
					}
				};
				{
					//二分法查找最合适的切割位点.
					size_t left_pos	 = dot_pos + 1;
					size_t right_pos = aret.size();
					string better_aret,better_aret_last;
					do {
						const size_t step_pos = (left_pos + right_pos) / 2;
						better_aret		= aret.substr(0, step_pos);
						//判断是否需要进位.(当被截断内容在radix_table的后半部分.)
						const char_t cut_char = aret[step_pos];
						const size_t cut_num	= _radix_table.find(cut_char);
						if(cut_num >= _radix / 2)
							rounding_up(better_aret);
						//判断当前切割位点有效性.
						if(from_string_get<T>(better_aret) == num) {
							if(better_aret.back() == _radix_table[0]) {
								const auto end_pos = better_aret.find_last_not_of(_radix_table[0]);
								better_aret.resize(end_pos + 1);
								if(better_aret.back() == _fractional_sign)
									better_aret.pop_back();
							}
							right_pos		 = better_aret.size();
							better_aret_last = better_aret;
						}
						elseif(left_pos != step_pos)
							left_pos = step_pos;
						else
							left_pos++;
					} while(left_pos < right_pos);
					if(better_aret_last){
						aret = better_aret_last;
						if(aret.ends_with(_fractional_sign))
							aret.pop_back();
					}
				}
			}
		}
		return aret;
	}
};
distinctive inline numerical_representation_t trinary{3,es"012"_constexpr_str};
distinctive inline numerical_representation_t binary{2,es"01"_constexpr_str};
distinctive inline numerical_representation_t octal{8,es"01234567"_constexpr_str};
distinctive inline numerical_representation_t decimal{10,es"0123456789"_constexpr_str};
distinctive inline numerical_representation_t hexadecimal{16,es"0123456789abcdef"_constexpr_str};
distinctive inline numerical_representation_t hexadecimal_upper{16,es"0123456789ABCDEF"_constexpr_str};

//字符串转换为数值
template<typename T> requires ::std::is_arithmetic_v<T>
inline T from_string_get(const string&str,const numerical_representation_t&radix=decimal)noexcept{
	return radix.from_string_get<T>(str);
}
//数值转换为字符串（小数部分粗略处理）
template<typename T> requires ::std::is_arithmetic_v<T>
inline string to_string_rough(T num,const numerical_representation_t&radix=decimal)noexcept{
	return radix.to_string_rough(num);
}
//数值转换为字符串
template<typename T> requires ::std::is_arithmetic_v<T>
inline string to_string(T num,const numerical_representation_t&radix=decimal)noexcept{
	return radix.to_string(num);
}

//file_end

