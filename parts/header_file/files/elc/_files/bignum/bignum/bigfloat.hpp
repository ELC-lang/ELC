//bigfloat.hpp
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
class bigfloat;
[[nodiscard]]bigfloat copy_as_negative(const ubigfloat&,bool sign=true)noexcept;
[[nodiscard]]bigfloat copy_as_negative(ubigfloat&&,bool sign=true)noexcept;

class bigfloat{
	ubigfloat _num;
	bool _is_negative=false;
public:
	//constructors
	bigfloat()noexcept=default;
	bigfloat(const bigfloat& other)noexcept=default;
	bigfloat(bigfloat&& other)noexcept=default;
	bigfloat& operator=(const bigfloat& other)&noexcept=default;
	bigfloat& operator=(bigfloat&& other)&noexcept=default;
	bigfloat(const ubigfloat& number)noexcept:_num(number){}
	bigfloat(ubigfloat&& number)noexcept:_num(move(number)){}
	bigfloat(const bigint& other)noexcept:_num(abs(other)),_is_negative(is_negative(other)){}
	bigfloat(bigint&& other)noexcept:_num(abs(move(other))),_is_negative(is_negative(other)){}
	bigfloat(const ubigint& other)noexcept:_num(other){}
	bigfloat(ubigint&& other)noexcept:_num(move(other)){}

	template<typename T> requires ::std::is_arithmetic_v<T>
	bigfloat(T num)noexcept:_num(abs(num)),_is_negative(is_negative(num)){}
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
		if constexpr(integer_type<T>)
			if(trunc(*this)!=*this)
				return false;
		return true;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]T convert_to()const noexcept{
		if constexpr(unsigned_type<T>)
			return _num.convert_to<T>();
		else{
			using math::copy_as_negative;//貌似msvc在这里有bug
			return copy_as_negative(_num.convert_to<to_unsigned_t<T>>(),_is_negative);
		}
	}
private:
	bigfloat(ubigfloat&& number, bool is_negative)noexcept:
		_num(move(number)),
		_is_negative(is_negative)
	{}
	bigfloat(const ubigfloat& number, bool is_negative)noexcept:
		_num(number),
		_is_negative(is_negative)
	{}
public:
	//operator-
	[[nodiscard]]bigfloat operator-()const&noexcept{
		return bigfloat{_num, !_is_negative};
	}
	[[nodiscard]]bigfloat&& operator-()&&noexcept{
		_is_negative=!_is_negative;
		return move(*this);
	}
	//friend abs
	[[nodiscard]]friend ubigfloat abs(const bigfloat& a)noexcept{
		return a._num;
	}
	[[nodiscard]]friend ubigfloat&& abs(bigfloat&& a)noexcept{
		return move(a._num);
	}
	//friend is_negative
	[[nodiscard]]friend bool is_negative(const bigfloat& a)noexcept{
		return a._is_negative;
	}
	//friend copy_as_negative
	[[nodiscard]]friend bigfloat copy_as_negative(const bigfloat& a,bool sign=true)noexcept{
		return bigfloat{a._num, sign};
	}
	[[nodiscard]]friend bigfloat&& copy_as_negative(bigfloat&& a,bool sign=true)noexcept{
		a._is_negative=sign;
		return move(a);
	}
	[[nodiscard]]friend bigfloat copy_as_negative(const ubigfloat& a,bool sign)noexcept{
		return bigfloat{a, sign};
	}
	[[nodiscard]]friend bigfloat copy_as_negative(ubigfloat&& a,bool sign)noexcept{
		return bigfloat{move(a), sign};
	}
	//friend get_numerator
	[[nodiscard]]friend ubigint get_numerator(const bigfloat& a)noexcept{
		return get_numerator(a._num);
	}
	[[nodiscard]]friend ubigint&& get_numerator(bigfloat&& a)noexcept{
		return get_numerator(move(a._num));
	}
	//friend get_denominator
	[[nodiscard]]friend ubigint get_denominator(const bigfloat& a)noexcept{
		return get_denominator(a._num);
	}
	[[nodiscard]]friend ubigint&& get_denominator(bigfloat&& a)noexcept{
		return get_denominator(move(a._num));
	}
	//friend trunc
	[[nodiscard]]friend bigint trunc(const bigfloat& a)noexcept{
		return copy_as_negative(trunc(a._num),a._is_negative);
	}
	//friend split
	[[nodiscard]]friend auto split(const bigfloat& a)noexcept{
		struct result_t{
			bigint integer;
			bigfloat fraction;
		};
		auto result=split(a._num);
		return result_t{copy_as_negative(result.integer,a._is_negative),
						copy_as_negative(result.fraction,a._is_negative)};
	}
	//friend trunc_with_sub
	[[nodiscard]]friend bigint trunc_with_sub(bigfloat& a)noexcept{
		auto result=trunc_with_sub(a._num);
		return copy_as_negative(result,a._is_negative);
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const bigfloat& a)noexcept{
		return to_size_t(trunc(a));
	}
	//operator+
	[[nodiscard]]bigfloat operator+(const bigfloat& other)const&noexcept{
		if(_is_negative==other._is_negative)
			return bigfloat{_num+other._num, _is_negative};
		else{
			if(_num>other._num)
				return bigfloat{_num-other._num, _is_negative};
			else
				return bigfloat{other._num-_num, other._is_negative};
		}
	}
	[[nodiscard]]bigfloat operator+(const bigint& other)const&noexcept{
		if(_is_negative==is_negative(other))
			return bigfloat{_num+abs(other), _is_negative};
		else{
			auto uother=abs(other);
			if(_num>uother)
				return bigfloat{_num-move(uother), _is_negative};
			else
				return bigfloat{move(uother)-_num, !_is_negative};
		}
	}
	[[nodiscard]]bigfloat operator+(bigint&& other)const&noexcept{
		if(_is_negative==is_negative(other))
			return bigfloat{_num+abs(move(other)), _is_negative};
		else{
			auto uother=abs(move(other));
			if(_num>uother)
				return bigfloat{_num-move(uother), _is_negative};
			else
				return bigfloat{move(uother)-_num, !_is_negative};
		}
	}
	[[nodiscard]]bigfloat operator+(const ubigint& other)const&noexcept{
		if(_is_negative){
			if(_num>other)
				return bigfloat{_num-other, _is_negative};
			else
				return bigfloat{other-_num, false};
		}
		else
			return bigfloat{_num+other, _is_negative};
	}
	[[nodiscard]]bigfloat operator+(ubigint&& other)const&noexcept{
		if(_is_negative){
			if(_num>other)
				return bigfloat{_num-move(other), _is_negative};
			else
				return bigfloat{move(other)-_num, false};
		}
		else
			return bigfloat{_num+move(other), _is_negative};
	}
	//operator-
	[[nodiscard]]bigfloat operator-(const bigfloat& other)const&noexcept{
		if(_is_negative!=other._is_negative)
			return bigfloat{_num+other._num, _is_negative};
		else{
			if(_num>other._num)
				return bigfloat{_num-other._num, _is_negative};
			else
				return bigfloat{other._num-_num, !other._is_negative};
		}
	}
	[[nodiscard]]bigfloat operator-(const bigint& other)const&noexcept{
		if(_is_negative!=is_negative(other))
			return bigfloat{_num+abs(other), _is_negative};
		else{
			auto uother=abs(other);
			if(_num>uother)
				return bigfloat{_num-move(uother), _is_negative};
			else
				return bigfloat{move(uother)-_num, !_is_negative};
		}
	}
	[[nodiscard]]bigfloat operator-(bigint&& other)const&noexcept{
		if(_is_negative!=is_negative(other))
			return bigfloat{_num+abs(move(other)), _is_negative};
		else{
			auto uother=abs(move(other));
			if(_num>uother)
				return bigfloat{_num-move(uother), _is_negative};
			else
				return bigfloat{move(uother)-_num, !_is_negative};
		}
	}
	[[nodiscard]]bigfloat operator-(const ubigint& other)const&noexcept{
		if(_is_negative)
			return bigfloat{_num+other, _is_negative};
		else{
			if(_num>other)
				return bigfloat{_num-other, _is_negative};
			else
				return bigfloat{other-_num, true};
		}
	}
	[[nodiscard]]bigfloat operator-(ubigint&& other)const&noexcept{
		if(_is_negative)
			return bigfloat{_num+move(other), _is_negative};
		else{
			if(_num>other)
				return bigfloat{_num-move(other), _is_negative};
			else
				return bigfloat{move(other)-_num, true};
		}
	}
	//operator*
	[[nodiscard]]bigfloat operator*(const bigfloat& other)const&noexcept{
		const bool sign=_is_negative!=other._is_negative;
		return bigfloat{_num*other._num, sign};
	}
	[[nodiscard]]bigfloat operator*(const bigint& other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num*abs(other), sign};
	}
	[[nodiscard]]bigfloat operator*(bigint&& other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num*abs(move(other)), sign};
	}
	[[nodiscard]]bigfloat operator*(const ubigint& other)const&noexcept{
		return bigfloat{_num*other, _is_negative};
	}
	[[nodiscard]]bigfloat operator*(ubigint&& other)const&noexcept{
		return bigfloat{_num*move(other), _is_negative};
	}
	//operator/
	[[nodiscard]]bigfloat operator/(const bigfloat& other)const&noexcept{
		const bool sign=_is_negative!=other._is_negative;
		return bigfloat{_num/other._num, sign};
	}
	[[nodiscard]]bigfloat operator/(const bigint& other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num/abs(other), sign};
	}
	[[nodiscard]]bigfloat operator/(bigint&& other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num/abs(move(other)), sign};
	}
	[[nodiscard]]bigfloat operator/(const ubigint& other)const&noexcept{
		return bigfloat{_num/other, _is_negative};
	}
	[[nodiscard]]bigfloat operator/(ubigint&& other)const&noexcept{
		return bigfloat{_num/move(other), _is_negative};
	}
	//operator+=
	bigfloat& operator+=(const bigfloat& other)&noexcept{
		if(_is_negative==other._is_negative){
			_num+=other._num;
			return*this;
		}
		else{
			if(_num>other._num){
				_num-=other._num;
				return*this;
			}
			else{
				_num=other._num-_num;
				_is_negative=!other._is_negative;
				return*this;
			}
		}
	}
	bigfloat& operator+=(const bigint& other)&noexcept{
		if(_is_negative==is_negative(other)){
			_num+=abs(other);
			return*this;
		}
		else{
			auto uother=abs(other);
			if(_num>uother){
				_num-=move(uother);
				return*this;
			}
			else{
				_num=move(uother)-_num;
				_is_negative=!_is_negative;
				return*this;
			}
		}
	}
	bigfloat& operator+=(bigint&& other)&noexcept{
		if(_is_negative==is_negative(other)){
			_num+=abs(move(other));
			return*this;
		}
		else{
			auto uother=abs(move(other));
			if(_num>uother){
				_num-=move(uother);
				return*this;
			}
			else{
				_num=move(uother)-_num;
				_is_negative=!_is_negative;
				return*this;
			}
		}
	}
	bigfloat& operator+=(const ubigint& other)&noexcept{
		_num+=other;
		return*this;
	}
	bigfloat& operator+=(ubigint&& other)&noexcept{
		_num+=move(other);
		return*this;
	}
	//operator-=
	bigfloat& operator-=(const bigfloat& other)&noexcept{
		if(_is_negative==other._is_negative){
			if(_num>other._num){
				_num-=other._num;
				return*this;
			}
			else{
				_num=other._num-_num;
				_is_negative=!other._is_negative;
				return*this;
			}
		}
		else{
			_num+=other._num;
			return*this;
		}
	}
	bigfloat& operator-=(const bigint& other)&noexcept{
		if(_is_negative==is_negative(other)){
			auto uother=abs(other);
			if(_num>uother){
				_num-=move(uother);
				return*this;
			}
			else{
				_num=move(uother)-_num;
				_is_negative=!_is_negative;
				return*this;
			}
		}
		else{
			_num+=abs(other);
			return*this;
		}
	}
	bigfloat& operator-=(bigint&& other)&noexcept{
		if(_is_negative==is_negative(other)){
			auto uother=abs(move(other));
			if(_num>uother){
				_num-=move(uother);
				return*this;
			}
			else{
				_num=move(uother)-_num;
				_is_negative=!_is_negative;
				return*this;
			}
		}
		else{
			_num+=abs(move(other));
			return*this;
		}
	}
	bigfloat& operator-=(const ubigint& other)&noexcept{
		if(_num>other){
			_num-=other;
			return*this;
		}
		else{
			_num=other-_num;
			_is_negative=!_is_negative;
			return*this;
		}
	}
	bigfloat& operator-=(ubigint&& other)&noexcept{
		if(_num>other){
			_num-=move(other);
			return*this;
		}
		else{
			_num=move(other)-_num;
			_is_negative=!_is_negative;
			return*this;
		}
	}
	//operator*=
	bigfloat& operator*=(const bigfloat& other)&noexcept{
		_is_negative=_is_negative!=other._is_negative;
		_num*=other._num;
		return*this;
	}
	bigfloat& operator*=(const bigint& other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num*=abs(other);
		return*this;
	}
	bigfloat& operator*=(bigint&& other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num*=abs(move(other));
		return*this;
	}
	bigfloat& operator*=(const ubigint& other)&noexcept{
		_num*=other;
		return*this;
	}
	bigfloat& operator*=(ubigint&& other)&noexcept{
		_num*=move(other);
		return*this;
	}
	//operator/=
	bigfloat& operator/=(const bigfloat& other)&noexcept{
		_is_negative=_is_negative!=other._is_negative;
		_num/=other._num;
		return*this;
	}
	bigfloat& operator/=(const bigint& other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num/=abs(other);
		return*this;
	}
	bigfloat& operator/=(bigint&& other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num/=abs(move(other));
		return*this;
	}
	bigfloat& operator/=(const ubigint& other)&noexcept{
		_num/=other;
		return*this;
	}
	bigfloat& operator/=(ubigint&& other)&noexcept{
		_num/=move(other);
		return*this;
	}
	//operator+-*/of any type
	template<typename T> requires is_arithmetic_type<T>
	[[nodiscard]]friend bigfloat operator+(T&&lhs,const bigfloat& rhs)noexcept{
		return bigfloat(forward<T>(lhs))+rhs;
	}
	template<typename T> requires is_arithmetic_type<T>
	[[nodiscard]]friend bigfloat operator-(T&&lhs,const bigfloat& rhs)noexcept{
		return bigfloat(forward<T>(lhs))-rhs;
	}
	template<typename T> requires is_arithmetic_type<T>
	[[nodiscard]]friend bigfloat operator*(T&&lhs,const bigfloat& rhs)noexcept{
		return bigfloat(forward<T>(lhs))*rhs;
	}
	template<typename T> requires is_arithmetic_type<T>
	[[nodiscard]]friend bigfloat operator/(T&&lhs,const bigfloat& rhs)noexcept{
		return bigfloat(forward<T>(lhs))/rhs;
	}
	//operator==
	[[nodiscard]]bool operator==(const bigfloat& other)const noexcept{
		if(_is_negative!=other._is_negative)return false;
		return _num==other._num;
	}
	[[nodiscard]]bool operator==(const bigint& other)const noexcept{
		if(_is_negative!=is_negative(other))return false;
		return _num==abs(other);
	}
	[[nodiscard]]bool operator==(bigint&& other)const noexcept{
		if(_is_negative!=is_negative(other))return false;
		return _num==abs(move(other));
	}
	[[nodiscard]]bool operator==(const ubigint& other)const noexcept{
		if(_is_negative)return false;
		return _num==other;
	}
	[[nodiscard]]bool operator==(ubigint&& other)const noexcept{
		if(_is_negative)return false;
		return _num==move(other);
	}
	//operator<=>
	[[nodiscard]]auto operator<=>(const bigfloat& other)const noexcept{
		if(_is_negative!=other._is_negative)
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		const bool needs_reverse=_is_negative;
		auto result=_num <=> other._num;
		if(needs_reverse)
			result=compare.reverse(result);
		return result;
	}
	[[nodiscard]]auto operator<=>(const bigint& other)const noexcept{
		if(_is_negative!=is_negative(other))
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		const bool needs_reverse=_is_negative;
		auto result=_num <=> abs(other);
		if(needs_reverse)
			result=compare.reverse(result);
		return result;
	}
	[[nodiscard]]auto operator<=>(bigint&& other)const noexcept{
		if(_is_negative!=is_negative(other))
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		const bool needs_reverse=_is_negative;
		auto result=_num <=> abs(move(other));
		if(needs_reverse)
			result=compare.reverse(result);
		return result;
	}
	[[nodiscard]]auto operator<=>(const ubigint& other)const noexcept{
		if(_is_negative)return strong_ordering::less;
		return _num <=> other;
	}
	[[nodiscard]]auto operator<=>(ubigint&& other)const noexcept{
		if(_is_negative)return strong_ordering::less;
		return _num <=> move(other);
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
	[[nodiscard]]bigfloat&& operator+(const ubigint& other)&&noexcept{
		return move(*this += other);
	}
	[[nodiscard]]bigfloat&& operator+(ubigint&& other)&&noexcept{
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
	[[nodiscard]]bigfloat&& operator-(const ubigint& other)&&noexcept{
		return move(*this -= other);
	}
	[[nodiscard]]bigfloat&& operator-(ubigint&& other)&&noexcept{
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
	[[nodiscard]]bigfloat&& operator*(const ubigint& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]bigfloat&& operator*(ubigint&& other)&&noexcept{
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
	[[nodiscard]]bigfloat&& operator/(const ubigint& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]bigfloat&& operator/(ubigint&& other)&&noexcept{
		return move(*this /= move(other));
	}
	[[nodiscard]]bigfloat&& operator+(bigfloat&& other)noexcept{
		return move(move(other) + *this);
	}
	[[nodiscard]]bigfloat&& operator-(bigfloat&& other)noexcept{
		return move((-move(other)) + *this);
	}
	[[nodiscard]]bigfloat&& operator*(bigfloat&& other)noexcept{
		return move(move(other) * *this);
	}
	//operator!
	[[nodiscard]]bool operator!()const noexcept{
		return!_num;
	}
	[[nodiscard]]explicit operator bool()const noexcept{
		return !operator!();
	}
public:
	//化简
	void simplify()noexcept{
		_num.simplify();
	}
	//friend pow
	friend [[nodiscard]] bigfloat pow(bigfloat base,ubigint exp)noexcept{
		const bool sign=is_negative(base)&&is_odd(exp);
		auto uret=pow(abs(base),exp);
		return {move(uret),sign};
	}
	friend [[nodiscard]] bigfloat pow(bigfloat base,bigint exp)noexcept{
		const bool sign=is_negative(base)&&is_odd(exp);
		auto uret=pow(abs(base),abs(exp));
		return {move(uret),sign};
	}
	//强转
	[[nodiscard]]explicit operator bigint()const&noexcept{
		return copy_as_negative(ubigint(_num),_is_negative);
	}
	[[nodiscard]]explicit operator bigint()&&noexcept{
		return copy_as_negative(ubigint(move(_num)),_is_negative);
	}
	[[nodiscard]]explicit operator ubigint()const&noexcept{
		return ubigint(_num);
	}
	[[nodiscard]]explicit operator ubigint()&&noexcept{
		return ubigint(move(_num));
	}
};

//file_end

