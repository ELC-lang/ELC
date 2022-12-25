//bigfloat.hpp
//at namespace elc::defs::big_number_types
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class bigfloat{
	bigint _numerator;
	bigint _denominator;
public:
	//constructors
	bigfloat()noexcept=default;
	bigfloat(const bigfloat& other)noexcept=default;
	bigfloat(bigfloat&& other)noexcept=default;
	bigfloat& operator=(const bigfloat& other)&noexcept=default;
	bigfloat& operator=(bigfloat&& other)&noexcept=default;
	bigfloat(const bigint& other)noexcept:_numerator(other),_denominator(1){}
	bigfloat(bigint&& other)noexcept:_numerator(move(other)),_denominator(1){}
	bigfloat(const ubigint& other)noexcept:_numerator(other),_denominator(1){}
	bigfloat(ubigint&& other)noexcept:_numerator(move(other)),_denominator(1){}

	template<typename T> requires ::std::is_integral_v<T>
	bigfloat(T num)noexcept:_numerator(num),_denominator(1){}
	template<typename T> requires ::std::is_floating_point_v<T>
	bigfloat(T num)noexcept{
		if(isNaN(num))return;
		//将一个浮点类型无损的转换为两个bigint相除
		bool sign = is_negative(num);
		if(sign)num = -num;
		if constexpr(::std::numeric_limits<T>::has_infinity)
			if(num == ::std::numeric_limits<T>::infinity()){
				_numerator = copy_as_negative(1, sign);
				_denominator = 0;
				return;
			}
		_numerator = 0;
		_denominator = 1;
		ptrdiff_t exp = 0;
		while(num > BIT_POSSIBILITY){
			num /= BIT_POSSIBILITY;
			exp++;
		}
		while(num != 0){
			num *= BIT_POSSIBILITY;
			_numerator *= BIT_POSSIBILITY;
			exp--;
			_numerator += static_cast<unsigned char>(num);
			num -= static_cast<unsigned char>(num);
		}
		if(exp > 0)
			_numerator *= (unsigned_specific_size_t<sizeof(T)>)pow(BIT_POSSIBILITY, exp);
		else
			_denominator *= (unsigned_specific_size_t<sizeof(T)>)pow(BIT_POSSIBILITY, -exp);
		_numerator = copy_as_negative(_numerator, sign);
		simplify();
	}
public:
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		constexpr auto min_value=min(type_info<T>);
		constexpr auto max_value=max(type_info<T>);
		if constexpr(min_value)
			if(*this<min_value)
				return false;
		if(*this>max_value)
			return false;
		if constexpr(::std::is_integral_v<T>)
			if(_numerator%_denominator)
				return false;
		return true;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]T convert_to()const noexcept{
		if constexpr(::std::is_integral_v<T>)
			return trunc(*this).convert_to<T>();
		else{
			T ans=0;
			{
				bigfloat tmp=abs(*this);
				bigint num=trunc(tmp);
				tmp-=num;
				typedef unsigned_specific_size_fast_t<sizeof(T)> integer_get_type;
				{
					size_t exp=0;
					bigfloat fnum=num;
					while(trunc(fnum)){
						fnum/=BIT_POSSIBILITY;
						exp++;
					}
					while(exp--){
						fnum*=BIT_POSSIBILITY;
						ans*=BIT_POSSIBILITY;
						num = trunc(fnum);
						fnum -= num;
						ans += num.convert_to<integer_get_type>();
					}
				}
				{
					T ans_mantissa=0;
					size_t exp=0;
					while(tmp){
						tmp*=BIT_POSSIBILITY;
						ans_mantissa*=BIT_POSSIBILITY;
						num=trunc(tmp);
						tmp-=num;
						ans_mantissa+=num.convert_to<integer_get_type>();
						exp++;
					}
					ans_mantissa/=pow(BIT_POSSIBILITY,exp);
					ans+=ans_mantissa;
				}
			}
			if(is_negative(*this))
				ans=-ans;
			return ans;
		}
	}
private:
	bigfloat(bigint&& numerator, bigint&& denominator)noexcept:
		_numerator(move(numerator)),
		_denominator(move(denominator))
	{}
	bigfloat(const bigint& numerator, const bigint& denominator)noexcept:
		_numerator(numerator),
		_denominator(denominator)
	{}
public:
	//friend abs
	[[nodiscard]]friend bigfloat abs(const bigfloat& a)noexcept{
		return bigfloat{abs(a._numerator),abs(a._denominator)};
	}
	[[nodiscard]]friend bigfloat&& abs(bigfloat&& a)noexcept{
		a._numerator=abs(move(a._numerator));
		a._denominator=abs(move(a._denominator));
		return move(a);
	}
	//friend is_negative
	[[nodiscard]]friend bool is_negative(const bigfloat& a)noexcept{
		return is_negative(a._numerator)^is_negative(a._denominator);
	}
	//friend copy_as_negative
	[[nodiscard]]friend bigfloat copy_as_negative(const bigfloat& a,bool sign=true)noexcept{
		return bigfloat{copy_as_negative(a._numerator,sign),abs(a._denominator)};
	}
	[[nodiscard]]friend bigfloat&& copy_as_negative(bigfloat&& a,bool sign=true)noexcept{
		a._numerator=copy_as_negative(move(a._numerator),sign);
		a._denominator=abs(move(a._denominator));
		return move(a);
	}
	//friend trunc
	[[nodiscard]]friend bigint trunc(const bigfloat& a)noexcept{
		return a._numerator/a._denominator;
	}
	[[nodiscard]]explicit operator bigint()noexcept{
		return trunc(*this);
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const bigfloat& a)noexcept{
		return to_size_t(trunc(a));
	}
	//operator+
	[[nodiscard]]bigfloat operator+(const bigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator + other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return bigfloat(move(numerator), move(denominator));
	}
	[[nodiscard]]bigfloat operator+(const bigint& other)const&noexcept{
		auto numerator = _numerator + other * _denominator;
		return bigfloat(move(numerator), _denominator);
	}
	[[nodiscard]]bigfloat operator+(bigint&& other)const&noexcept{
		auto numerator = _numerator + move(other) * _denominator;
		return bigfloat(move(numerator), _denominator);
	}
	//operator-
	[[nodiscard]]bigfloat operator-(const bigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator - other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return bigfloat(move(numerator), move(denominator));
	}
	[[nodiscard]]bigfloat operator-(const bigint& other)const&noexcept{
		auto numerator = _numerator - other * _denominator;
		return bigfloat(move(numerator), _denominator);
	}
	[[nodiscard]]bigfloat operator-(bigint&& other)const&noexcept{
		auto numerator = _numerator - move(other) * _denominator;
		return bigfloat(move(numerator), _denominator);
	}
	//operator*
	[[nodiscard]]bigfloat operator*(const bigfloat& other)const&noexcept{
		auto numerator = _numerator * other._numerator;
		auto denominator = _denominator * other._denominator;
		return bigfloat(move(numerator), move(denominator));
	}
	[[nodiscard]]bigfloat operator*(const bigint& other)const&noexcept{
		auto numerator = _numerator * other;
		return bigfloat(move(numerator), _denominator);
	}
	[[nodiscard]]bigfloat operator*(bigint&& other)const&noexcept{
		auto numerator = _numerator * move(other);
		return bigfloat(move(numerator), _denominator);
	}
	//operator/
	[[nodiscard]]bigfloat operator/(const bigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator;
		auto denominator = _denominator * other._numerator;
		return bigfloat(move(numerator), move(denominator));
	}
	[[nodiscard]]bigfloat operator/(const bigint& other)const&noexcept{
		auto denominator = _denominator * other;
		return bigfloat(_numerator, move(denominator));
	}
	[[nodiscard]]bigfloat operator/(bigint&& other)const&noexcept{
		auto denominator = _denominator * move(other);
		return bigfloat(_numerator, move(denominator));
	}
	//operator+=
	bigfloat& operator+=(const bigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator + other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	bigfloat& operator+=(const bigint& other)&noexcept{
		_numerator += other * _denominator;
		return*this;
	}
	bigfloat& operator+=(bigint&& other)&noexcept{
		_numerator += move(other) * _denominator;
		return*this;
	}
	//operator-=
	bigfloat& operator-=(const bigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator - other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	bigfloat& operator-=(const bigint& other)&noexcept{
		_numerator -= other * _denominator;
		return*this;
	}
	bigfloat& operator-=(bigint&& other)&noexcept{
		_numerator -= move(other) * _denominator;
		return*this;
	}
	//operator*=
	bigfloat& operator*=(const bigfloat& other)&noexcept{
		_numerator *= other._numerator;
		_denominator *= other._denominator;
		return*this;
	}
	bigfloat& operator*=(const bigint& other)&noexcept{
		_numerator *= other;
		return*this;
	}
	bigfloat& operator*=(bigint&& other)&noexcept{
		_numerator *= move(other);
		return*this;
	}
	//operator/=
	bigfloat& operator/=(const bigfloat& other)&noexcept{
		_numerator *= other._denominator;
		_denominator *= other._numerator;
		return*this;
	}
	bigfloat& operator/=(const bigint& other)&noexcept{
		_denominator *= other;
		return*this;
	}
	bigfloat& operator/=(bigint&& other)&noexcept{
		_denominator *= move(other);
		return*this;
	}
	//operator==
	[[nodiscard]]bool operator==(const bigfloat& other)const noexcept{
		return _numerator * other._denominator == other._numerator * _denominator;
	}
	[[nodiscard]]bool operator==(const bigint& other)const noexcept{
		return _numerator == other * _denominator;
	}
	[[nodiscard]]bool operator==(bigint&& other)const noexcept{
		return _numerator == move(other) * _denominator;
	}
	//operator<=>
	[[nodiscard]]auto operator<=>(const bigfloat& other)const noexcept{
		return _numerator * other._denominator <=> other._numerator * _denominator;
	}
	[[nodiscard]]auto operator<=>(const bigint& other)const noexcept{
		return _numerator <=> other * _denominator;
	}
	[[nodiscard]]auto operator<=>(bigint&& other)const noexcept{
		return _numerator <=> move(other) * _denominator;
	}
	//operatorX for rvalue
	[[nodiscard]]bigfloat&& operator+(const bigfloat& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]bigfloat&& operator+(const bigint& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]bigfloat&& operator+(bigint&& other)&&noexcept{
		return move(*this += move(other));
	}
	[[nodiscard]]bigfloat&& operator-(const bigfloat& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]bigfloat&& operator-(const bigint& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]bigfloat&& operator-(bigint&& other)&&noexcept{
		return move(*this -= move(other));
	}
	[[nodiscard]]bigfloat&& operator*(const bigfloat& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]bigfloat&& operator*(const bigint& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]bigfloat&& operator*(bigint&& other)&&noexcept{
		return move(*this *= move(other));
	}
	[[nodiscard]]bigfloat&& operator/(const bigfloat& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]bigfloat&& operator/(const bigint& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]bigfloat&& operator/(bigint&& other)&&noexcept{
		return move(*this /= move(other));
	}
	[[nodiscard]]bigfloat&& operator+(bigfloat&& other)noexcept{
		return move(move(other) + *this);
	}
	[[nodiscard]]bigfloat&& operator-(bigfloat&& other)noexcept{
		return move(move(other) - *this);
	}
	[[nodiscard]]bigfloat&& operator*(bigfloat&& other)noexcept{
		return move(move(other) * *this);
	}
	[[nodiscard]]bigfloat&& operator/(bigfloat&& other)noexcept{
		return move(move(other) / *this);
	}
	//operator!
	[[nodiscard]]bool operator!()const noexcept{
		return!_numerator;
	}
	[[nodiscard]]explicit operator bool()const noexcept{
		return !operator!();
	}
public:
	//化简
	void simplify()noexcept{
		bigint g=gcd(abs(_numerator),abs(_denominator));
		_numerator/=g;
		_denominator/=g;
	}
};

//file_end

