//defs.hpp
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

/*!
客制化to_string用途类
*/
struct base_numerical_representation_t{};
/*!
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
	//信息尾部分
	inline size_t get_info_tail_size_per_byte()const noexcept{
		constexpr auto info_threshold_base = pow(BIT_POSSIBILITY, bitnum_of(byte));
		const auto	   info_threshold	   = to_size_t(ceil(log(info_threshold_base, _radix)));
		return info_threshold;
	}
	template<typename T>
	inline size_t get_info_tail_size()const noexcept{
		return get_info_tail_size_per_byte()*sizeof(T)+_radix;
	}
	template<typename T>
	inline string get_info_tail(T x)const noexcept{
		auto info_tail_size_per_byte = get_info_tail_size_per_byte();
		string aret;
		data_view<const T> view{&x};
		for(const byte c: view){
			auto s= to_string_rough((unsigned char)c);
			aret+=string{info_tail_size_per_byte-s.size(),_radix_table[0]}+s;
		}
		return aret+_radix_table;
	}
	template<typename T>
	inline T get_from_info_tail(string str)const noexcept{
		const auto info_tail_size_per_byte = get_info_tail_size_per_byte();
		str.pop_back(_radix);
		T	aret{};
		data_view<T> view{&aret};
		for(byte&c: view){
			auto s=str.substr(0,info_tail_size_per_byte);
			c=(byte)from_string_get<unsigned char>(s);
			str=str.substr(info_tail_size_per_byte);
		}
		return aret;
	}
	template<typename T>
	inline string to_string_num_base(T num)const noexcept{
		//Information threshold相关声明
		//小数情况下，这限制了在当前radix下mantissa的最大长度，避免如radix=3而num=0.25时的无限循环
		//而不管整数还是小数的情况下，这都会被用作计算预分配空间大小
		constexpr auto info_threshold_base = pow(BIT_POSSIBILITY,bitnum_of(T));
		auto		   info_threshold =	to_size_t(ceil(log(info_threshold_base,_radix)));
		if constexpr(::std::is_floating_point_v<T>){
			string aret;
			suppress_msvc_warning(26494)//未初始化警告diss
			size_t order_of_magnitude;
			if(num>1)
				order_of_magnitude = to_size_t(floor(log(num,_radix)+1));
			else {
				order_of_magnitude = 0;
				aret=_radix_table[0];
				aret+=_fractional_sign;
			}
			num/=pow(_radix,order_of_magnitude);
			if(num>=1){
				aret=_radix_table[to_size_t(num)];
				num=0;
			}
			//浮点数精度浮动，所以需要确定何时开始使用Info threshold
			bool is_mantissa_begined = false;
			aret.pre_alloc_after_end(info_threshold+1);
			while(num){
				//mantissa push部分
				num*=_radix;
				T first_char_index;
				num=::std::modf(num,&first_char_index);
				aret+=_radix_table[to_size_t(first_char_index)];
				//Information threshold制御部分
				if(to_size_t(first_char_index))
					is_mantissa_begined = true;
				if(is_mantissa_begined){
					if(info_threshold)
						info_threshold--;
				}
				//小数点计数部分
				if(order_of_magnitude){
					if(!--order_of_magnitude)
						aret+=_fractional_sign;
				}
				//结束判断
				if(!info_threshold && !order_of_magnitude)
					break;
			}
			if(order_of_magnitude)
				aret.append(order_of_magnitude,_radix_table[0]);
			if(aret.ends_with(_fractional_sign))
				aret.pop_back();
			return aret;
		}
		else{
			string aret;
			aret.pre_alloc_before_begin(info_threshold);
			do{//do while，在num为0时也有返值
				const auto first_char_index = mod(num,_radix);
				num /= (T)_radix;
				aret.push_front(_radix_table[(size_t)first_char_index]);
			}while(num>=T{1});
			return aret;
		}
	}
	template<typename T>
	inline bool to_string_special_value_check(T num,string&str,bool is_negative)const noexcept{
		if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
			if(::std::isnan(num)){
				if constexpr(::std::numeric_limits<T>::has_signaling_NaN){
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
				if constexpr(::std::numeric_limits<T>::has_quiet_NaN){
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
				for(const byte c: view){
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
					if(is_negative){
						str=_negative_sign+_inf;
						return true;
					}
				str=_inf;
				return true;
			}
		}
		return false;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string_rough_no_special_value_check(T num)const noexcept{
		string aret;
		const bool is_negative=magic_number::is_negative(num);
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
		aret+=to_string_num_base(unum);
		return aret;
	}
public:
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string_rough(T num)const noexcept{
		string aret;
		const bool is_negative=magic_number::is_negative(num);
		if(to_string_special_value_check(num,aret,is_negative))
			return aret;
		return to_string_rough_no_special_value_check(num);
	}
private:
	template<typename T>
	inline T from_string_get_num_base(string str)const noexcept{
		if constexpr(::std::is_floating_point_v<T>){
			size_t order_of_magnitude = str.size();
			T aret{};
			size_t i=str.size();
			while(i){
				i--;
				const size_t index=_radix_table.find(str[i]);
				if(index==string::npos){
					if(str[i] == _fractional_sign){
						order_of_magnitude=i;
						continue;
					}
					return T();
				}
				aret+=(T)index;
				aret/=_radix;
			}
			aret*=pow(_radix, order_of_magnitude);
			return aret;
		}
		else {
			T aret{};
			for(size_t i = 0; i < str.size(); i++){
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
				suppress_msvc_warning(26494)//未初始化警告diss
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
	template<typename T>
	inline T from_string_no_tail_check_get(string str)const noexcept{
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
		UT unum=from_string_get_num_base<UT>(str);
		return copy_as_negative<T>(unum,is_negative);
	}
public:
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline T from_string_get(string str)const noexcept{
		//信息尾检查
		if constexpr(::std::is_floating_point_v<T>){
			const auto info_tail_size=get_info_tail_size<T>();
			if(str.size()>info_tail_size && str.ends_with(_radix_table)){
				const auto tail_pos=str.size()-info_tail_size;
				auto info_tail=str.substr(tail_pos);
				auto str_with_out_tail=str.substr(0,tail_pos);
				if(str_with_out_tail.back()==_fractional_sign)
					str_with_out_tail.pop_back();
				auto num=get_from_info_tail<T>(info_tail);
				if(to_string_rough(num).starts_with(str_with_out_tail))
					return num;
			}
		}
		return from_string_no_tail_check_get<T>(str);
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string(T num)const noexcept{
		string aret;
		if(to_string_special_value_check(num,aret,is_negative(num)))
			return aret;
		aret=to_string_rough_no_special_value_check(num);
		if constexpr(::std::is_floating_point_v<T>) {
			size_t dot_pos = aret.find(_fractional_sign);
			//检查是否可反向转换
			if(from_string_no_tail_check_get<T>(aret) != num) {
				//获取并追加信息尾
				string info_tail = get_info_tail(num);
				if(dot_pos == string::npos) {
					if(aret.ends_with(string{info_tail.size(), _radix_table[0]}))
						aret.pop_back(info_tail.size());
					else
						aret.push_back(_fractional_sign);
				}
				aret += info_tail;
				return aret;
			}
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
				while(i){
					if(rounding_up_char(str[i])){
						if(i--){
							if(str[i] == _fractional_sign)
								i--;
						}
						else
							str.push_front(_radix_table[1]);
						continue;
					}
					else
						break;
				}
			};
			{
				//二分法查找最合适的切割位点.
				size_t left_pos	 = 0;
				size_t right_pos = aret.size();
				string better_aret,better_aret_last;
				if(dot_pos==string::npos)
					dot_pos=aret.size();
				do {
					size_t step_pos = (left_pos + right_pos) / 2;
					better_aret		= aret.substr(0, step_pos);
					if(step_pos==dot_pos)
						step_pos++;
					//判断是否需要进位.(当被截断内容在radix_table的后半部分.)
					const char_t cut_char = aret[step_pos];
					const size_t cut_num	= _radix_table.find(cut_char);
					if(cut_num >= _radix / 2)
						rounding_up(better_aret);
					if(better_aret.size() < dot_pos)//0补全
						better_aret.resize(dot_pos,_radix_table[0]);
					//判断当前切割位点有效性.
					if(from_string_no_tail_check_get<T>(better_aret) == num){
						if(better_aret.back() == _radix_table[0]){
							const auto end_pos = max(better_aret.find_last_not_of(_radix_table[0])+1, dot_pos);
							better_aret.resize(end_pos);
							if(better_aret.back() == _fractional_sign)
								better_aret.pop_back();
						}
						right_pos		 = min(better_aret.size(), step_pos);
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
		return aret;
	}
};
push_and_disable_msvc_warning(26426);
distinctive inline numerical_representation_t trinary{3,es"012"_constexpr_str};
distinctive inline numerical_representation_t binary{2,es"01"_constexpr_str};
distinctive inline numerical_representation_t octal{8,es"01234567"_constexpr_str};
distinctive inline numerical_representation_t decimal{10,es"0123456789"_constexpr_str};
distinctive inline numerical_representation_t hexadecimal{16,es"0123456789abcdef"_constexpr_str};
distinctive inline numerical_representation_t hexadecimal_upper{16,es"0123456789ABCDEF"_constexpr_str};
pop_msvc_warning();

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

