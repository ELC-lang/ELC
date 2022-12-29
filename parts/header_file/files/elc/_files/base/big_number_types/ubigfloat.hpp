//ubigfloat.hpp
//at namespace elc::defs::big_number_types
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class ubigfloat{
	ubigint _numerator;
	ubigint _denominator=1u;
public:
	//constructors
	ubigfloat()noexcept=default;
	ubigfloat(const ubigfloat& other)noexcept=default;
	ubigfloat(ubigfloat&& other)noexcept=default;
	ubigfloat& operator=(const ubigfloat& other)&noexcept=default;
	ubigfloat& operator=(ubigfloat&& other)&noexcept=default;
	ubigfloat(const bigint& other)noexcept:_numerator(abs(other)){}
	ubigfloat(bigint&& other)noexcept:_numerator(abs(move(other))){}
	ubigfloat(const ubigint& other)noexcept:_numerator(other){}
	ubigfloat(ubigint&& other)noexcept:_numerator(move(other)){}

	template<typename T> requires (::std::is_integral_v<T> && ::std::is_unsigned_v<T>)
	ubigfloat(T num)noexcept:_numerator(num){}
	template<typename T> requires ::std::is_floating_point_v<T>
	ubigfloat(T num)noexcept{
		if(isNaN(num))return;
		//将一个浮点类型无损的转换为两个bigint相除
		if(is_negative(num))num = -num;
		if constexpr(::std::numeric_limits<T>::has_infinity)
			if(num == ::std::numeric_limits<T>::infinity()){
				_numerator = 1u;
				_denominator = zero;
				return;
			}
		{
			const auto exponent = get_exponent(num);
			const auto base_num = get_base_num(num);
			const auto precision = get_precision(num);
			constexpr auto precision_base = get_precision_base<T>();
			_numerator = base_num + precision;
			_denominator = precision_base;
			if(exponent){
				if(exponent>0)
					_numerator<<=exponent;
				else
					_denominator<<=abs(exponent);
			}
		}
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
	[[nodiscard]]T convert_to()&&noexcept{//只对于右值，便于编写。
		//左值版本只需要创建一个临时变量，然后调用右值版本即可。
		if constexpr(::std::is_integral_v<T>)
			return trunc(*this).convert_to<T>();
		else{//浮点数
			simplify();
			if(!_denominator)
				if constexpr(::std::numeric_limits<T>::has_infinity)
					return ::std::numeric_limits<T>::infinity();
				else
					return ::std::numeric_limits<T>::max();

			constexpr auto precision_base_bitnum = bitnum_of(float_precision_base_t<T>);
			ptrdiff_t exp=0;
			while(!(_numerator%BIT_POSSIBILITY)){
				_numerator/=BIT_POSSIBILITY;
				++exp;
			}
			while(!(_denominator%BIT_POSSIBILITY)){
				_denominator/=BIT_POSSIBILITY;
				--exp;
			}
			//将精度调整到T的精度
			_numerator<<=precision_base_bitnum;//扩大数字来容纳可能在除法中产生的数
			exp-=precision_base_bitnum;//指数相应的减少
			_numerator/=_denominator;//有损舍入精度
			//_denominator=1;_denominator<<=precision_base_bitnum;//不必要的数据更新
			while(_numerator>>precision_base_bitnum){//对多余的精度进行舍入，仍然，这是可能有损的
				_numerator/=BIT_POSSIBILITY;
				++exp;
			}
			//现在我们有了一个合适的基数和合适的指数！
			//接下来只需要将它们转换为T即可
			return make_float<T>(_numerator.convert_to<float_precision_base_t<T>>(),exp);
		}
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]T convert_to()const&noexcept{
		if constexpr(::std::is_integral_v<T>)
			return trunc(*this).convert_to<T>();
		else
			return ubigfloat(*this).convert_to<T>();
	}
private:
	ubigfloat(ubigint&& numerator, ubigint&& denominator)noexcept:
		_numerator(move(numerator)),
		_denominator(move(denominator))
	{}
	ubigfloat(const ubigint& numerator, const ubigint& denominator)noexcept:
		_numerator(numerator),
		_denominator(denominator)
	{}
public:
	//friend abs
	[[nodiscard]]friend ubigfloat abs(const ubigfloat& a)noexcept{
		return a;
	}
	[[nodiscard]]friend ubigfloat&& abs(ubigfloat&& a)noexcept{
		return move(a);
	}
	//friend is_negative
	[[nodiscard]]friend bool is_negative(const ubigfloat&)noexcept{
		return false;
	}
	//friend trunc
	[[nodiscard]]friend ubigint trunc(const ubigfloat& a)noexcept{
		return a._numerator/a._denominator;
	}
	[[nodiscard]]explicit operator ubigint()noexcept{
		return trunc(*this);
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const ubigfloat& a)noexcept{
		return to_size_t(trunc(a));
	}
	//operator+
	[[nodiscard]]ubigfloat operator+(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator + other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	[[nodiscard]]ubigfloat operator+(const ubigint& other)const&noexcept{
		auto numerator = _numerator + other * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	[[nodiscard]]ubigfloat operator+(ubigint&& other)const&noexcept{
		auto numerator = _numerator + move(other) * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	[[nodiscard]]ubigfloat operator+(const bigint& other)const&noexcept{
		if(is_negative(other))
			return *this - abs(other);
		else
			return *this + abs(other);
	}
	//operator-
	[[nodiscard]]ubigfloat operator-(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator - other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	[[nodiscard]]ubigfloat operator-(const ubigint& other)const&noexcept{
		auto numerator = _numerator - other * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	[[nodiscard]]ubigfloat operator-(ubigint&& other)const&noexcept{
		auto numerator = _numerator - move(other) * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	[[nodiscard]]ubigfloat operator-(const bigint& other)const&noexcept{
		if(is_negative(other))
			return *this + abs(other);
		else
			return *this - abs(other);
	}
	//operator*
	[[nodiscard]]ubigfloat operator*(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._numerator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	[[nodiscard]]ubigfloat operator*(const ubigint& other)const&noexcept{
		auto numerator = _numerator * other;
		return ubigfloat{move(numerator), _denominator};
	}
	[[nodiscard]]ubigfloat operator*(ubigint&& other)const&noexcept{
		auto numerator = _numerator * move(other);
		return ubigfloat{move(numerator), _denominator};
	}
	//operator/
	[[nodiscard]]ubigfloat operator/(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator;
		auto denominator = _denominator * other._numerator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	[[nodiscard]]ubigfloat operator/(const ubigint& other)const&noexcept{
		auto denominator = _denominator * other;
		return ubigfloat{_numerator, move(denominator)};
	}
	[[nodiscard]]ubigfloat operator/(ubigint&& other)const&noexcept{
		auto denominator = _denominator * move(other);
		return ubigfloat{_numerator, move(denominator)};
	}
	//operator+=
	ubigfloat& operator+=(const ubigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator + other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	ubigfloat& operator+=(const ubigint& other)&noexcept{
		_numerator += other * _denominator;
		return*this;
	}
	ubigfloat& operator+=(ubigint&& other)&noexcept{
		_numerator += move(other) * _denominator;
		return*this;
	}
	ubigfloat& operator+=(const bigint& other)&noexcept{
		if(is_negative(other))
			return*this -= abs(other);
		else
			return*this += abs(other);
	}
	ubigfloat& operator+=(bigint&& other)&noexcept{
		if(is_negative(other))
			return*this -= abs(move(other));
		else
			return*this += abs(move(other));
	}
	//operator-=
	ubigfloat& operator-=(const ubigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator - other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	ubigfloat& operator-=(const ubigint& other)&noexcept{
		_numerator -= other * _denominator;
		return*this;
	}
	ubigfloat& operator-=(ubigint&& other)&noexcept{
		_numerator -= move(other) * _denominator;
		return*this;
	}
	ubigfloat& operator-=(const bigint& other)&noexcept{
		if(is_negative(other))
			return*this += abs(other);
		else
			return*this -= abs(other);
	}
	ubigfloat& operator-=(bigint&& other)&noexcept{
		if(is_negative(other))
			return*this += abs(move(other));
		else
			return*this -= abs(move(other));
	}
	//operator*=
	ubigfloat& operator*=(const ubigfloat& other)&noexcept{
		_numerator *= other._numerator;
		_denominator *= other._denominator;
		return*this;
	}
	ubigfloat& operator*=(const ubigint& other)&noexcept{
		_numerator *= other;
		return*this;
	}
	ubigfloat& operator*=(ubigint&& other)&noexcept{
		_numerator *= move(other);
		return*this;
	}
	//operator/=
	ubigfloat& operator/=(const ubigfloat& other)&noexcept{
		_numerator *= other._denominator;
		_denominator *= other._numerator;
		return*this;
	}
	ubigfloat& operator/=(const ubigint& other)&noexcept{
		_denominator *= other;
		return*this;
	}
	ubigfloat& operator/=(ubigint&& other)&noexcept{
		_denominator *= move(other);
		return*this;
	}
	//operator==
	[[nodiscard]]bool operator==(const ubigfloat& other)const noexcept{
		return _numerator * other._denominator == other._numerator * _denominator;
	}
	[[nodiscard]]bool operator==(const ubigint& other)const noexcept{
		return _numerator == other * _denominator;
	}
	[[nodiscard]]bool operator==(ubigint&& other)const noexcept{
		return _numerator == move(other) * _denominator;
	}
	[[nodiscard]]bool operator==(const bigint& other)const noexcept{
		if(is_negative(other))return false;
		return _numerator == other * _denominator;
	}
	[[nodiscard]]bool operator==(bigint&& other)const noexcept{
		if(is_negative(other))return false;
		return _numerator == move(other) * _denominator;
	}
	//operator<=>
	[[nodiscard]]auto operator<=>(const ubigfloat& other)const noexcept{
		return _numerator * other._denominator <=> other._numerator * _denominator;
	}
	[[nodiscard]]auto operator<=>(const ubigint& other)const noexcept{
		return _numerator <=> other * _denominator;
	}
	[[nodiscard]]auto operator<=>(ubigint&& other)const noexcept{
		return _numerator <=> move(other) * _denominator;
	}
	[[nodiscard]]auto operator<=>(const bigint& other)const noexcept{
		if(is_negative(other))return strong_ordering::greater;
		return _numerator <=> other * _denominator;
	}
	[[nodiscard]]auto operator<=>(bigint&& other)const noexcept{
		if(is_negative(other))return strong_ordering::greater;
		return _numerator <=> move(other) * _denominator;
	}
	//operatorX for rvalue
	[[nodiscard]]ubigfloat&& operator+(const ubigfloat& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]ubigfloat&& operator+(const ubigint& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]ubigfloat&& operator+(ubigint&& other)&&noexcept{
		return move(*this += move(other));
	}
	[[nodiscard]]ubigfloat&& operator+(const bigint& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]ubigfloat&& operator+(bigint&& other)&&noexcept{
		return move(*this += move(other));
	}
	[[nodiscard]]ubigfloat&& operator-(const ubigfloat& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]ubigfloat&& operator-(const ubigint& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]ubigfloat&& operator-(ubigint&& other)&&noexcept{
		return move(*this -= move(other));
	}
	[[nodiscard]]ubigfloat&& operator-(const bigint& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]ubigfloat&& operator-(bigint&& other)&&noexcept{
		return move(*this -= move(other));
	}
	[[nodiscard]]ubigfloat&& operator*(const ubigfloat& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]ubigfloat&& operator*(const ubigint& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]ubigfloat&& operator*(ubigint&& other)&&noexcept{
		return move(*this *= move(other));
	}
	[[nodiscard]]ubigfloat&& operator/(const ubigfloat& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]ubigfloat&& operator/(const ubigint& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]ubigfloat&& operator/(ubigint&& other)&&noexcept{
		return move(*this /= move(other));
	}
	[[nodiscard]]ubigfloat&& operator+(ubigfloat&& other)noexcept{
		return move(move(other) + *this);
	}
	[[nodiscard]]ubigfloat&& operator-(ubigfloat&& other)noexcept{
		return move(move(other) - *this);
	}
	[[nodiscard]]ubigfloat&& operator*(ubigfloat&& other)noexcept{
		return move(move(other) * *this);
	}
	[[nodiscard]]ubigfloat&& operator/(ubigfloat&& other)noexcept{
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
		ubigint g=gcd(_numerator,_denominator);
		_numerator/=g;
		_denominator/=g;
	}
};
//operator +-*= of ubigint and ubigfloat
[[nodiscard]]inline ubigfloat operator+(const ubigint& lhs,const ubigfloat& rhs)noexcept{
	return rhs + lhs;
}
[[nodiscard]]inline ubigfloat operator+(ubigint&& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(move(lhs)) + rhs;
}
[[nodiscard]]inline ubigfloat&& operator+(const ubigint& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) + lhs;
}
[[nodiscard]]inline ubigfloat&& operator+(ubigint&& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) + move(lhs);
}
[[nodiscard]]inline ubigfloat operator-(const ubigint& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(lhs) - rhs;
}
[[nodiscard]]inline ubigfloat operator-(ubigint&& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(move(lhs)) - rhs;
}
[[nodiscard]]inline ubigfloat&& operator-(const ubigint& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) - lhs;
}
[[nodiscard]]inline ubigfloat&& operator-(ubigint&& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) - move(lhs);
}
[[nodiscard]]inline ubigfloat operator*(const ubigint& lhs,const ubigfloat& rhs)noexcept{
	return rhs * lhs;
}
[[nodiscard]]inline ubigfloat operator*(ubigint&& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(move(lhs)) * rhs;
}
[[nodiscard]]inline ubigfloat&& operator*(const ubigint& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) * lhs;
}
[[nodiscard]]inline ubigfloat&& operator*(ubigint&& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) * move(lhs);
}
[[nodiscard]]inline ubigfloat operator/(const ubigint& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(lhs) / rhs;
}
[[nodiscard]]inline ubigfloat operator/(ubigint&& lhs,const ubigfloat& rhs)noexcept{
	return ubigfloat(move(lhs)) / rhs;
}
[[nodiscard]]inline ubigfloat&& operator/(const ubigint& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) / lhs;
}
[[nodiscard]]inline ubigfloat&& operator/(ubigint&& lhs,ubigfloat&& rhs)noexcept{
	return move(rhs) / move(lhs);
}

//file_end

