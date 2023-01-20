//_body.hpp
//at namespace elc::defs::numerical_representation_n
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*!
基类，记录数字的表示方式以便客制化to_string
*/
struct base_numerical_representation_t{
	virtual ~base_numerical_representation_t()noexcept=default;
	[[nodiscard]]virtual size_t get_radix()const noexcept=0;
	[[nodiscard]]virtual char_t get_fractional_sign()const noexcept=0;
	[[nodiscard]]virtual char_t get_positive_sign()const noexcept=0;
	[[nodiscard]]virtual char_t get_negative_sign()const noexcept=0;
	[[nodiscard]]virtual string_view get_nan()const noexcept=0;
	[[nodiscard]]virtual string_view get_signaling_nan()const noexcept=0;
	[[nodiscard]]virtual string_view get_quiet_nan()const noexcept=0;
	[[nodiscard]]virtual string_view get_inf()const noexcept=0;
	[[nodiscard]]virtual char_t get_fractional_separator()const noexcept=0;
	[[nodiscard]]virtual char_t get_exponent_separator()const noexcept=0;
	[[nodiscard]]virtual char_t get_unknown_data_start_sign()const noexcept=0;
	[[nodiscard]]virtual char_t get_unknown_data_split_sign()const noexcept=0;
	[[nodiscard]]virtual char_t get_unknown_data_end_sign()const noexcept=0;
	[[nodiscard]]virtual char_t get_char(size_t index)const noexcept=0;
	[[nodiscard]]virtual size_t get_index(char_t c)const noexcept=0;
	[[nodiscard]]virtual bool is_valid_char(char_t ch)const noexcept=0;
	[[nodiscard]]bool is_finite(const bigfloat&num)const noexcept{
		return is_finite_denominator_part(get_denominator(num));
	}
	[[nodiscard]]bool is_finite(const ubigfloat&num)const noexcept{
		return is_finite_denominator_part(get_denominator(num));
	}
protected:
	//bigfloat的分母部分的质因数只有radix的质因数组成时才是有限小数
	[[nodiscard]]virtual bool is_finite_denominator_part(ubigint)const noexcept=0;
public:
	//根据分母部分获取弥补分母到radix的幂的乘数并返回，将分母部分置零并递增exp值
	[[nodiscard]]virtual ubigint get_denominator_complement(ubigint&denominator,ptrdiff_t&exp)const noexcept=0;
};
template<class T>
concept numerical_representation = is_base_of<base_numerical_representation_t,T>;//type_info<T>.is_base_on<base_numerical_representation_t>;

/// @brief constexpr数字表示方式的默认内容，用于快速定义constexpr数字表示方式
class comn_constexpr_numerical_representation_t:public base_numerical_representation_t{
	char_t _fractional_sign=ec('.');
	char_t _positive_sign=ec('+');
	char_t _negative_sign=ec('-');

	const constexpr_str _nan=es"NaN"_constexpr_str;
	const constexpr_str _signaling_nan=es"signaling_NaN"_constexpr_str;
	const constexpr_str _quiet_nan=es"quiet_NaN"_constexpr_str;
	const constexpr_str _inf=es"Infinity"_constexpr_str;

	char_t _fractional_separator=ec('/');
	char_t _exponent_separator=ec('e');
	char_t _unknown_data_start_sign=ec('['), _unknown_data_split_sign=ec(','), _unknown_data_end_sign=ec(']');
public:
	constexpr comn_constexpr_numerical_representation_t()noexcept=default;
	consteval comn_constexpr_numerical_representation_t(
		char_t fractional_sign,char_t positive_sign,char_t negative_sign,
		const constexpr_str& nan,
		const constexpr_str& signaling_nan,
		const constexpr_str& quiet_nan,
		const constexpr_str& inf,
		char_t fractional_separator,
		char_t exponent_separator,
		char_t unknown_data_start_sign, char_t unknown_data_split_sign, char_t unknown_data_end_sign)noexcept:
		_fractional_sign(fractional_sign),_positive_sign(positive_sign),_negative_sign(negative_sign),
		_nan(nan),_signaling_nan(signaling_nan),_quiet_nan(quiet_nan),_inf(inf),
		_fractional_separator(fractional_separator),
		_exponent_separator(exponent_separator),
		_unknown_data_start_sign(unknown_data_start_sign),_unknown_data_split_sign(unknown_data_split_sign),_unknown_data_end_sign(unknown_data_end_sign){}
	constexpr comn_constexpr_numerical_representation_t(char_t exponent_separator)noexcept:
		_exponent_separator(exponent_separator){}

	[[nodiscard]]constexpr virtual char_t get_fractional_sign()const noexcept override{
		return _fractional_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_positive_sign()const noexcept override{
		return _positive_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_negative_sign()const noexcept override{
		return _negative_sign;
	}
	[[nodiscard]]constexpr virtual string_view get_nan()const noexcept override{
		return _nan;
	}
	[[nodiscard]]constexpr virtual string_view get_signaling_nan()const noexcept override{
		return _signaling_nan;
	}
	[[nodiscard]]constexpr virtual string_view get_quiet_nan()const noexcept override{
		return _quiet_nan;
	}
	[[nodiscard]]constexpr virtual string_view get_inf()const noexcept override{
		return _inf;
	}
	[[nodiscard]]constexpr virtual char_t get_fractional_separator()const noexcept override{
		return _fractional_separator;
	}
	[[nodiscard]]constexpr virtual char_t get_exponent_separator()const noexcept override{
		return _exponent_separator;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_start_sign()const noexcept override{
		return _unknown_data_start_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_split_sign()const noexcept override{
		return _unknown_data_split_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_end_sign()const noexcept override{
		return _unknown_data_end_sign;
	}
};

/// @brief 一个简单的数字表示方式，只有一个字符表示0，其他数字可以直接用_zero+index表示
/// 由于需要编译时构建质因数分解表，所以radix必须是模板参数
/// 顺带将zero也作为模板参数让看起来更好看
/// @tparam zero 0的字符
/// @tparam radix 基数
template<char_t zero,size_t radix>
class constexpr_continuous_numerical_representation_t final:public comn_constexpr_numerical_representation_t{
	unique_prime_factorization_table_t<radix> _prime_factorization_table;
public:
	consteval constexpr_continuous_numerical_representation_t(
		char_t fractional_sign,char_t positive_sign,char_t negative_sign,
		const constexpr_str& nan,
		const constexpr_str& signaling_nan,
		const constexpr_str& quiet_nan,
		const constexpr_str& inf,
		char_t fractional_separator,
		char_t exponent_separator,
		char_t unknown_data_start_sign, char_t unknown_data_split_sign, char_t unknown_data_end_sign)noexcept:
		comn_constexpr_numerical_representation_t(
			fractional_sign,positive_sign,negative_sign,
			nan,signaling_nan,quiet_nan,inf,
			fractional_separator,
			exponent_separator,
			unknown_data_start_sign,unknown_data_split_sign,unknown_data_end_sign
		){}
	constexpr constexpr_continuous_numerical_representation_t()noexcept{};
	constexpr constexpr_continuous_numerical_representation_t(char_t exponent_separator)noexcept:
		comn_constexpr_numerical_representation_t(exponent_separator){}

	[[nodiscard]]constexpr virtual size_t get_radix()const noexcept override{
		return radix;
	}
	[[nodiscard]]constexpr virtual char_t get_char(size_t index)const noexcept override{
		return char_t(zero+index);
	}
	[[nodiscard]]constexpr virtual size_t get_index(char_t c)const noexcept override{
		return c-zero;
	}
	[[nodiscard]]constexpr virtual bool is_valid_char(char_t ch)const noexcept override{
		return ch>=zero&&ch<zero+radix;
	}
protected:
	//bigfloat的分母部分的质因数只有radix的质因数组成时才是有限小数
	[[nodiscard]]constexpr virtual bool is_finite_denominator_part(ubigint num)const noexcept override{
		for(const auto i:_prime_factorization_table){
			auto result=divmod(num,i);
			while(!result.mod){
				num=move(result.quot);
				result=divmod(num,i);
			}
			if(num==1u)
				return true;
		}
		return false;
	}
public:
	[[nodiscard]]constexpr virtual ubigint get_denominator_complement(ubigint&denominator,ptrdiff_t&exp)const noexcept override{
		ubigint aret=1u;
		for(const auto i:_prime_factorization_table){
			const auto i_complement=radix/i;
			auto result=divmod(denominator,i);
			while(result.quot && !result.mod){
				denominator=move(result.quot);
				aret*=i_complement;
				result=divmod(denominator,i);
				exp--;
			}
		}
		return aret;
	}
};
push_and_disable_msvc_warning(26426);
distinctive inline constexpr constexpr_continuous_numerical_representation_t<ec('0'),3> trinary{};
distinctive inline constexpr constexpr_continuous_numerical_representation_t<ec('0'),2> binary{};
distinctive inline constexpr constexpr_continuous_numerical_representation_t<ec('0'),8> octal{};
distinctive inline constexpr constexpr_continuous_numerical_representation_t<ec('0'),10> decimal{};
distinctive inline constexpr constexpr_continuous_numerical_representation_t<ec('ꀀ'),1165> valanlottish{
	/*fractional_sign*/ec('꒛'),/*positive_sign*/ec('꒜'),/*negative_sign*/ec('꒝'),
	/*nan*/es"꒘"_constexpr_str,
	/*signaling_nan*/es"꒗"_constexpr_str,
	/*quiet_nan*/es"꒖"_constexpr_str,
	/*inf*/es"꒾"_constexpr_str,
	/*fractional_separator*/ec('꒼'),
	/*exponent_separator*/ec('꒫'),
	/*unknown_data_start_sign*/ec('꒸'),/*unknown_data_split_sign*/ec('꓄'),/*unknown_data_end_sign*/ec('꒹')
};
//16和72进制的码表不是连续的，所以不能用constexpr_continuous_numerical_representation_t
pop_msvc_warning();

/// @brief 一个简单的数字表示方式，以constexpr_str存储进制内容
/// 由于需要编译时构建质因数分解表，所以radix必须是模板参数，导致radix_table是模板参数
/// @tparam radix_table_p 进制表的指针
template<const constexpr_str*radix_table_p>
class constexpr_str_numerical_representation_t final:public comn_constexpr_numerical_representation_t{
	static constexpr size_t radix=radix_table_p->size();
	static constexpr const constexpr_str& radix_table=*radix_table_p;

	unique_prime_factorization_table_t<radix> _prime_factorization_table;
public:
	consteval constexpr_str_numerical_representation_t(
		char_t fractional_sign,char_t positive_sign,char_t negative_sign,
		const constexpr_str& nan,
		const constexpr_str& signaling_nan,
		const constexpr_str& quiet_nan,
		const constexpr_str& inf,
		char_t fractional_separator,
		char_t exponent_separator,
		char_t unknown_data_start_sign, char_t unknown_data_split_sign, char_t unknown_data_end_sign)noexcept:
		comn_constexpr_numerical_representation_t(
			fractional_sign,positive_sign,negative_sign,
			nan,signaling_nan,quiet_nan,inf,
			fractional_separator,
			exponent_separator,
			unknown_data_start_sign,unknown_data_split_sign,unknown_data_end_sign
		){}
	constexpr constexpr_str_numerical_representation_t()noexcept{};
	constexpr constexpr_str_numerical_representation_t(char_t exponent_separator)noexcept:
		comn_constexpr_numerical_representation_t(exponent_separator){}

	[[nodiscard]]constexpr virtual size_t get_radix()const noexcept override{
		return radix_table.size();
	}
	[[nodiscard]]constexpr virtual char_t get_char(size_t index)const noexcept override{
		return radix_table[index];
	}
	[[nodiscard]]constexpr virtual size_t get_index(char_t ch)const noexcept override{
		return radix_table.find(ch);
	}
	[[nodiscard]]constexpr virtual bool is_valid_char(char_t ch)const noexcept override{
		return radix_table.find(ch)!=constexpr_str::npos;
	}
protected:
	//bigfloat的分母部分的质因数只有radix的质因数组成时才是有限小数
	[[nodiscard]]constexpr virtual bool is_finite_denominator_part(ubigint num)const noexcept override{
		for(const auto i:_prime_factorization_table){
			auto result=divmod(num,i);
			while(!result.mod){
				num=move(result.quot);
				result=divmod(num,i);
			}
			if(num==1u)
				return true;
		}
		return false;
	}
public:
	[[nodiscard]]constexpr virtual ubigint get_denominator_complement(ubigint&denominator,ptrdiff_t&exp)const noexcept override{
		ubigint aret=1u;
		for(const auto i:_prime_factorization_table){
			const auto i_complement=radix/i;
			auto result=divmod(denominator,i);
			while(result.quot && !result.mod){
				denominator=move(result.quot);
				aret*=i_complement;
				result=divmod(denominator,i);
				exp--;
			}
		}
		return aret;
	}
};
push_and_disable_msvc_warning(26426);
//在上方已经定义了trinary,binary,octal,decimal
//这里定义hexadecimal和hexadecimal_upper
distinctive inline constexpr constexpr_str_numerical_representation_t<&es"0123456789abcdef"_constexpr_str> hexadecimal{ec('V')};
distinctive inline constexpr constexpr_str_numerical_representation_t<&es"0123456789ABCDEF"_constexpr_str> hexadecimal_upper{ec('v')};
pop_msvc_warning();

/// @brief 一个简单的数字表示方式，以string存储进制内容
/// @note 用于在编译期无法确定进制内容的情况，拥有已知最大的开销
class comn_numerical_representation_t final:public base_numerical_representation_t{
	string _radix_table;
	char_t _fractional_sign;
	char_t _positive_sign;
	char_t _negative_sign;
	string _nan, _signaling_nan, _quiet_nan;
	string _inf;
	char_t _fractional_separator;
	char_t _exponent_separator;
	char_t _unknown_data_start_sign, _unknown_data_split_sign, _unknown_data_end_sign;
	array_t<size_t> _unique_prime_factorization_table;
public:
	comn_numerical_representation_t(
		string radix_table=es"0123456789"_constexpr_str,
		char_t fractional_sign=ec('.'),char_t positive_sign=ec('+'),char_t negative_sign=ec('-'),
		string nan=es"NaN"_constexpr_str,
		string signaling_nan=es"signaling_NaN"_constexpr_str,
		string quiet_nan=es"quiet_NaN"_constexpr_str,
		string inf=es"Infinity"_constexpr_str,
		char_t fractional_separator=ec('.'),
		char_t exponent_separator=ec('e'),
		char_t unknown_data_start_sign=ec('['), char_t unknown_data_split_sign=ec(','), char_t unknown_data_end_sign=ec(']'))noexcept:
		_radix_table(move(radix_table)),
		_fractional_sign(fractional_sign),_positive_sign(positive_sign),_negative_sign(negative_sign),
		_nan(move(nan)),_signaling_nan(move(signaling_nan)),_quiet_nan(move(quiet_nan)),_inf(move(inf)),
		_fractional_separator(fractional_separator),
		_exponent_separator(exponent_separator),
		_unknown_data_start_sign(unknown_data_start_sign),_unknown_data_split_sign(unknown_data_split_sign),_unknown_data_end_sign(unknown_data_end_sign){
		//计算质因数：get_prime_factorization(num).unique().to<容器类型>(); 
		_unique_prime_factorization_table=get_prime_factorization(_radix_table.size()).unique().to<array_t<size_t>>();
	}
	[[nodiscard]]virtual size_t get_radix()const noexcept override{
		return _radix_table.size();
	}
	[[nodiscard]]constexpr virtual char_t get_fractional_sign()const noexcept override{
		return _fractional_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_positive_sign()const noexcept override{
		return _positive_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_negative_sign()const noexcept override{
		return _negative_sign;
	}
	[[nodiscard]]virtual string_view get_nan()const noexcept override{
		return _nan;
	}
	[[nodiscard]]virtual string_view get_signaling_nan()const noexcept override{
		return _signaling_nan;
	}
	[[nodiscard]]virtual string_view get_quiet_nan()const noexcept override{
		return _quiet_nan;
	}
	[[nodiscard]]virtual string_view get_inf()const noexcept override{
		return _inf;
	}
	[[nodiscard]]constexpr virtual char_t get_fractional_separator()const noexcept override{
		return _fractional_separator;
	}
	[[nodiscard]]constexpr virtual char_t get_exponent_separator()const noexcept override{
		return _exponent_separator;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_start_sign()const noexcept override{
		return _unknown_data_start_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_split_sign()const noexcept override{
		return _unknown_data_split_sign;
	}
	[[nodiscard]]constexpr virtual char_t get_unknown_data_end_sign()const noexcept override{
		return _unknown_data_end_sign;
	}
	[[nodiscard]]virtual char_t get_char(size_t index)const noexcept override{
		return _radix_table[index];
	}
	[[nodiscard]]virtual size_t get_index(char_t ch)const noexcept override{
		return _radix_table.find(ch);
	}
	[[nodiscard]]virtual bool is_valid_char(char_t ch)const noexcept override{
		return _radix_table.find(ch)!=string::npos;
	}
protected:
	[[nodiscard]]virtual bool is_finite_denominator_part(ubigint num)const noexcept override{
		for(const auto& i:_unique_prime_factorization_table){
			auto result=divmod(num,i);
			while(!result.mod){
				num=move(result.quot);
				result=divmod(num,i);
			}
			if(num==1u)
				return true;
		}
		return false;
	}
public:
	[[nodiscard]]virtual ubigint get_denominator_complement(ubigint&denominator,ptrdiff_t&exp)const noexcept override{
		ubigint aret=1u;
		const auto radix = get_radix();
		for(const auto& i: _unique_prime_factorization_table) {
			const auto i_complement=radix/i;
			auto result=divmod(denominator,i);
			while(result.quot && !result.mod){
				denominator=move(result.quot);
				aret *=i_complement;
				result=divmod(denominator,i);
				exp--;
			}
		}
		return aret;
	}
};

//file_end

