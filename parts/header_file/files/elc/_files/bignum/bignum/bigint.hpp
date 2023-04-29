//bigint.hpp
//at namespace elc::defs::bignum_n
/*
未完成的elc解释器bignum文件
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
class bigint;
[[nodiscard]]bigint copy_as_negative(const ubigint&,bool sign=true)noexcept;
[[nodiscard]]bigint copy_as_negative(ubigint&&,bool sign=true)noexcept;
using math::copy_as_negative;//避免可能的符号覆盖

class bigint{
	ubigint _num;
	bool _is_negative=0;

	bigint(const ubigint&num,bool is_negative)noexcept:_num(num),_is_negative(is_negative){
		if(!_num)_is_negative = false;
	}
	bigint(ubigint&&num,bool is_negative)noexcept:_num(move(num)),_is_negative(is_negative){
		if(!_num)_is_negative = false;
	}
public:
	bigint()noexcept = default;
	bigint(const bigint&)noexcept = default;
	bigint(bigint&&)noexcept = default;
	bigint(const ubigint&a)noexcept:_num(a),_is_negative(false){}
	bigint(ubigint&&a)noexcept:_num(move(a)),_is_negative(false){}
	template<integer_type T>
	bigint(T value)noexcept:_num(abs(value)),_is_negative(is_negative(value)){}

	bigint& operator=(const bigint&)&noexcept = default;
	bigint& operator=(bigint&&)&noexcept = default;
	bigint& operator=(const ubigint&a)&noexcept{
		_num = a;
		_is_negative = false;
		return *this;
	}
	bigint& operator=(ubigint&&a)&noexcept{
		_num = move(a);
		_is_negative = false;
		return *this;
	}
	template<typename T> requires(unsigned_integer_type<T>)
	bigint& operator=(T value)&noexcept{
		_num = value;
		_is_negative = false;
		return *this;
	}
	template<basic_float_type T>
	explicit bigint(T&&value)noexcept:_num(abs(value)),_is_negative(is_negative(value)){}

	~bigint() = default;
public:
	//operator==
	[[nodiscard]]bool operator==(const bigint& other)const noexcept{
		if(_is_negative != other._is_negative)
			return false;
		return _num == other._num;
	}
	template<integer_type T>
	[[nodiscard]]bool operator==(T value)const noexcept{
		if(_is_negative != is_negative(value))
			return false;
		return _num == abs(value);
	}
	//operator<=>
	[[nodiscard]]auto operator<=>(const bigint& other)const noexcept{
		if(_is_negative != other._is_negative)
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		auto tmp = _num <=> other._num;
		if(_is_negative)
			tmp=compare.reverse(tmp);
		return tmp;
	}
	template<integer_type T>
	[[nodiscard]]auto operator<=>(T value)const noexcept{
		if(_is_negative != is_negative(value))
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		auto tmp = _num <=> abs(value);
		if(_is_negative)
			tmp=compare.reverse(tmp);
		return tmp;
	}
public:
	template<arithmetic_type T>
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		if constexpr(type_info<T>!=type_info<bigint>){
			if constexpr(has_min<T>){
				constexpr auto min_value=min(type_info<T>);
				if(min_value)
					if(*this<min_value)
						return false;
			}
			if constexpr(has_max<T>){
				constexpr auto max_value=max(type_info<T>);
				if(*this>max_value)
					return false;
			}
		}
		return true;
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()const noexcept{
		if constexpr(signed_type<T>)
			return copy_as_negative(move(_num).convert_to<to_unsigned_t<T>>(),_is_negative);
		else
			return _num.convert_to<T>();
	}
	//explicit operator T
	template<arithmetic_type T>
	[[nodiscard]]explicit operator T()&&noexcept{
		return move(*this).convert_to<T>();
	}
	template<arithmetic_type T>
	[[nodiscard]]explicit operator T()const&noexcept{
		return convert_to<T>();
	}
public:
	[[nodiscard]]bigint operator-()const&noexcept{
		return {_num,!_is_negative};
	}
	[[nodiscard]]bigint&& operator-()&&noexcept{
		_is_negative=!_is_negative;
		return move(*this);
	}
	//friend abs
	[[nodiscard]]friend ubigint abs(const bigint& a)noexcept{
		return a._num;
	}
	[[nodiscard]]friend ubigint&& abs(bigint&& a)noexcept{
		return move(a._num);
	}
	//friend is_negative
	[[nodiscard]]friend bool is_negative(const bigint& a)noexcept{
		return a._is_negative;
	}
	//friend copy_as_negative
	[[nodiscard]]friend bigint copy_as_negative(const bigint& a,bool sign=true)noexcept{
		return {a._num,sign};
	}
	[[nodiscard]]friend bigint&& copy_as_negative(bigint&& a,bool sign=true)noexcept{
		a._is_negative=sign;
		return move(a);
	}
	[[nodiscard]]friend bigint copy_as_negative(const ubigint& a,bool sign)noexcept{
		return {a,sign};
	}
	[[nodiscard]]friend bigint copy_as_negative(ubigint&& a,bool sign)noexcept{
		return {move(a),sign};
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const bigint& a)noexcept{
		return a.convert_to<size_t>();
	}
	//operator+
	[[nodiscard]]bigint operator+(const bigint& other)const&noexcept{
		if(_is_negative == other._is_negative)
			return {_num+other._num,_is_negative};
		elseif(_is_negative)
			return other-(-*this);
		else
			return *this-(-other);
	}
	//operator-
	[[nodiscard]]bigint operator-(const bigint& other)const&noexcept{
		bool sign = _is_negative;
		if(_is_negative != other._is_negative)
			return {_num+other._num,sign};
		if(_num < other._num){
			sign=!sign;
			return {other._num-_num,sign};
		}
		return {_num-other._num,sign};
	}
	//operator*
	[[nodiscard]]bigint operator*(const bigint& other)const&noexcept{
		const bool sign = _is_negative != other._is_negative;
		return bigint{_num*other._num,sign};
	}
	[[nodiscard]]bigint operator*(const ubigint& other)const&noexcept{
		return bigint{_num*other,_is_negative};
	}
	template<integer_type T>
	[[nodiscard]]bigint operator*(T other)const&noexcept{
		if constexpr(signed_type<T>)
			return *this*bigint(other);
		else
			return *this*ubigint(other);
	}
	//operator/
	[[nodiscard]]bigint operator/(const bigint& other)const&noexcept{
		const bool sign = _is_negative != other._is_negative;
		return bigint{_num/other._num,sign};
	}
	[[nodiscard]]bigint operator/(const ubigint& other)const&noexcept{
		return bigint{_num/other,_is_negative};
	}
	template<integer_type T>
	[[nodiscard]]bigint operator/(T other)const&noexcept{
		if constexpr(signed_type<T>)
			return *this/bigint(other);
		else
			return *this/ubigint(other);
	}
	//operator%
	[[nodiscard]]bigint operator%(const bigint& other)const&noexcept{
		return bigint{_num%other._num,_is_negative};
	}
	[[nodiscard]]bigint operator%(const ubigint& other)const&noexcept{
		return bigint{_num%other,_is_negative};
	}
	template<integer_type T>
	[[nodiscard]]bigint operator%(T other)const&noexcept{
		return *this%ubigint(abs(other));
	}
	//operator<<
	template<integer_type T>
	[[nodiscard]]bigint operator<<(T n)const&noexcept{
		return bigint{_num<<n,_is_negative};
	}
	[[nodiscard]]bigint operator<<(const bigint& n)const&noexcept{
		if(n._is_negative)
			return *this>>n._num;
		else
			return *this<<n._num;
	}
	[[nodiscard]]bigint operator<<(const ubigint& n)const&noexcept{
		return {_num<<n,_is_negative};
	}
	//operator>>
	template<integer_type T>
	[[nodiscard]]bigint operator>>(T n)const&noexcept{
		return bigint{_num>>n,_is_negative};
	}
	[[nodiscard]]bigint operator>>(const bigint& n)const&noexcept{
		if(n._is_negative)
			return *this<<n._num;
		else
			return *this>>n._num;
	}
	[[nodiscard]]bigint operator>>(const ubigint& n)const&noexcept{
		return {_num>>n,_is_negative};
	}
	//friend operator<< and operator>> for ubigint
	[[nodiscard]]friend bigint operator<<(const ubigint& a,const bigint& b)noexcept{
		if(b._is_negative)
			return a>>b._num;
		else
			return a<<b._num;
	}
	[[nodiscard]]friend bigint operator>>(const ubigint& a,const bigint& b)noexcept{
		if(b._is_negative)
			return a<<b._num;
		else
			return a>>b._num;
	}
	//operator+=
	bigint& operator+=(const bigint& other)&noexcept{
		if(_is_negative == other._is_negative)
			_num+=other._num;
		elseif(_is_negative)
			*this = other-(-move(*this));
		else
			*this -= -other;
		return *this;
	}
	bigint& operator+=(bigint&& other)&noexcept{
		if(_is_negative == other._is_negative)
			_num+=move(other._num);
		elseif(_is_negative)
			*this = other-=(-move(*this));
		else
			*this -= -move(other);
		return *this;
	}
	//operator-=
	bigint& operator-=(const bigint& other)&noexcept{
		if(_is_negative != other._is_negative)
			_num+=other._num;
		if(_num < other._num){
			_is_negative=!_is_negative;
			_num = other._num-_num;
		}
		else
			_num-=other._num;
		return*this;
	}
	bigint& operator-=(bigint&& other)&noexcept{
		if(_is_negative != other._is_negative)
			_num+=move(other._num);
		if(_num < other._num){
			_is_negative=!_is_negative;
			_num = move(other._num)-_num;
		}
		else
			_num-=move(other._num);
		return*this;
	}
	//operator*=
	bigint& operator*=(const bigint& other)&noexcept{
		_is_negative = _is_negative != other._is_negative;
		_num *= other._num;
		return*this;
	}
	bigint& operator*=(bigint&& other)&noexcept{
		_is_negative = _is_negative != other._is_negative;
		_num *= move(other._num);
		return*this;
	}
	template<integer_type T>
	bigint& operator*=(T other)&noexcept{
		if constexpr(signed_type<T>)
			return *this*=bigint(other);
		else
			return *this*=ubigint(other);
	}
	//operator/=
	bigint& operator/=(const bigint& other)&noexcept{
		_is_negative = _is_negative != other._is_negative;
		_num /= other._num;
		return*this;
	}
	bigint& operator/=(bigint&& other)&noexcept{
		_is_negative = _is_negative != other._is_negative;
		_num /= move(other._num);
		return*this;
	}
	template<integer_type T>
	bigint& operator/=(T other)&noexcept{
		if constexpr(signed_type<T>)
			return *this/=bigint(other);
		else
			return *this/=ubigint(other);
	}
	//operator%=
	bigint& operator%=(const bigint& other)&noexcept{
		//_is_negative = _is_negative;
		_num %= other._num;
		return*this;
	}
	bigint& operator%=(bigint&& other)&noexcept{
		//_is_negative = _is_negative;
		_num %= move(other._num);
		return*this;
	}
	template<integer_type T>
	bigint& operator%=(T other)&noexcept{
		return *this%=ubigint(abs(other));
	}
	//operator<<=
	template<integer_type T>
	bigint& operator<<=(T n)&noexcept{
		_num <<= n;
		return*this;
	}
	bigint& operator<<=(const bigint& other)&noexcept{
		if(other._is_negative)
			return*this>>=other._num;
		else
			return*this<<=other._num;
	}
	bigint& operator<<=(const ubigint& other)&noexcept{
		_num <<= other;
		return*this;
	}
	//operator>>=
	template<integer_type T>
	bigint& operator>>=(T n)&noexcept{
		_num >>= n;
		return*this;
	}
	bigint& operator>>=(const bigint& other)&noexcept{
		if(other._is_negative)
			return*this<<=other._num;
		else
			return*this>>=other._num;
	}
	bigint& operator>>=(const ubigint& other)&noexcept{
		_num >>= other;
		return*this;
	}
	//friend operator<<= and operator>>= for ubigint
	friend ubigint& operator<<=(ubigint& lhs, const bigint& rhs)noexcept{
		if(rhs._is_negative)
			return lhs>>=rhs._num;
		else
			return lhs<<=rhs._num;
	}
	friend ubigint& operator>>=(ubigint& lhs, const bigint& rhs)noexcept{
		if(rhs._is_negative)
			return lhs<<=rhs._num;
		else
			return lhs>>=rhs._num;
	}
	//operatorX for rvalue
	[[nodiscard]]bigint&& operator+(const bigint& other)&&noexcept{
		return move(*this+=other);
	}
	[[nodiscard]]bigint&& operator-(const bigint& other)&&noexcept{
		return move(*this-=other);
	}
	[[nodiscard]]bigint&& operator*(const bigint& other)&&noexcept{
		return move(*this*=other);
	}
	[[nodiscard]]bigint&& operator*(const ubigint& other)&&noexcept{
		return move(*this*=other);
	}
	template<integer_type T>
	[[nodiscard]]bigint&& operator*(T other)&&noexcept{
		return move(*this*=other);
	}
	[[nodiscard]]bigint&& operator/(const bigint& other)&&noexcept{
		return move(*this/=other);
	}
	[[nodiscard]]bigint&& operator/(const ubigint& other)&&noexcept{
		return move(*this/=other);
	}
	template<integer_type T>
	[[nodiscard]]bigint&& operator/(T other)&&noexcept{
		return move(*this/=other);
	}
	[[nodiscard]]bigint&& operator%(const bigint& other)&&noexcept{
		return move(*this%=other);
	}
	[[nodiscard]]bigint&& operator%(const ubigint& other)&&noexcept{
		return move(*this%=other);
	}
	template<integer_type T>
	[[nodiscard]]bigint&& operator%(T other)&&noexcept{
		return move(*this%=other);
	}
	template<integer_type T>
	[[nodiscard]]bigint&& operator<<(T other)&&noexcept{
		return move(*this<<=other);
	}
	[[nodiscard]]bigint&& operator<<(const bigint& other)&&noexcept{
		return move(*this<<=other);
	}
	[[nodiscard]]bigint&& operator<<(const ubigint& other)&&noexcept{
		return move(*this<<=other);
	}
	template<integer_type T>
	[[nodiscard]]bigint&& operator>>(T other)&&noexcept{
		return move(*this>>=other);
	}
	[[nodiscard]]bigint&& operator>>(const bigint& other)&&noexcept{
		return move(*this>>=other);
	}
	[[nodiscard]]bigint&& operator>>(const ubigint& other)&&noexcept{
		return move(*this>>=other);
	}
	friend ubigint&& operator<<(ubigint&& lhs, const bigint& rhs)noexcept{
		return move(lhs<<=rhs);
	}
	friend ubigint&& operator>>(ubigint&& lhs, const bigint& rhs)noexcept{
		return move(lhs>>=rhs);
	}
	[[nodiscard]]bigint&& operator+(bigint&& other)const&noexcept{
		return move(move(other)+*this);
	}
	[[nodiscard]]bigint&& operator-(bigint&& other)const&noexcept{
		return move((-move(other))+*this);
	}
	[[nodiscard]]bigint&& operator*(bigint&& other)const&noexcept{
		return move(move(other)**this);
	}
	//operator!
	[[nodiscard]]bool operator!()const noexcept{
		return!_num;
	}
	[[nodiscard]]explicit operator bool()const noexcept{
		return !operator!();
	}
	//operator++
	bigint& operator++()&noexcept{
		if(_is_negative){
			--_num;
			if(!_num)
				_is_negative=false;
		}
		else
			++_num;
		return*this;
	}
	[[nodiscard]]bigint operator++(int)&noexcept{
		auto tmp = *this;
		++*this;
		return tmp;
	}
	//operator--
	bigint& operator--()&noexcept{
		if(!_is_negative && !_num)
			_is_negative=true;
		if(_is_negative)
			++_num;
		else
			--_num;
		return*this;
	}
	[[nodiscard]]bigint operator--(int)&noexcept{
		auto tmp = *this;
		--*this;
		return tmp;
	}
	//friend is_odd
	friend [[nodiscard]] bool is_odd(const bigint& n)noexcept{
		return is_odd(n._num);
	}
	//friend is_even
	friend [[nodiscard]] bool is_even(const bigint& n)noexcept{
		return is_even(n._num);
	}
};

[[nodiscard]]inline bigint operator-(const ubigint&num)noexcept{
	return copy_as_negative(num);
}

//file_end

