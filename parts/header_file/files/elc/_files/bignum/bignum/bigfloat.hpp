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
using math::copy_as_negative;//避免可能的符号覆盖

[[nodiscard]]inline bigfloat operator-(const ubigfloat&num)noexcept;
[[nodiscard]]inline bigfloat operator-(ubigfloat&&num)noexcept;

class bigfloat{
	bool _is_negative=false;
	ubigfloat _num;
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

	template<arithmetic_type T>
	bigfloat(T&&num)noexcept:_is_negative(is_negative(num)),_num(abs(forward<T>(num))){}
public:
	template<arithmetic_type T>
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		return _num.is_safe_convert_to<to_unsigned_t<T>>();
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()&&noexcept{
		if constexpr(signed_type<T>)
			return copy_as_negative(move(_num).convert_to<to_unsigned_t<T>>(),_is_negative);
		else
			return move(_num).convert_to<T>();
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()const&noexcept{
		if constexpr(signed_type<T>)
			return copy_as_negative(_num.convert_to<to_unsigned_t<T>>(),_is_negative);
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
	//friend reciprocal
	[[nodiscard]]friend bigfloat reciprocal(const bigfloat& a)noexcept{
		return bigfloat{reciprocal(a._num), a._is_negative};
	}
	[[nodiscard]]friend bigfloat&& reciprocal(bigfloat&& a)noexcept{
		discard=reciprocal(move(a._num));//swap it!
		return move(a);
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
	template<arithmetic_type T>
	[[nodiscard]]bigfloat operator+(const T&other)const&noexcept{
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
	//operator-
	template<arithmetic_type T>
	[[nodiscard]]bigfloat operator-(const T&other)const&noexcept{
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
	//operator*
	template<arithmetic_type T>
	[[nodiscard]]bigfloat operator*(const T&other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num*abs(other), sign};
	}
	//operator/
	template<arithmetic_type T>
	[[nodiscard]]bigfloat operator/(const T&other)const&noexcept{
		const bool sign=_is_negative!=is_negative(other);
		return bigfloat{_num/abs(other), sign};
	}
	//operator+=
	template<unsigned_type T>
	bigfloat& operator+=(const T& other)&noexcept{
		_num+=other;
		return*this;
	}
	template<arithmetic_type T>
	bigfloat& operator+=(const T&other)&noexcept{
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
	//operator-=
	template<arithmetic_type T>
	bigfloat& operator-=(const T&other)&noexcept{
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
	//operator*=
	template<unsigned_type T>
	bigfloat& operator*=(const T& other)&noexcept{
		_num*=other;
		return*this;
	}
	template<arithmetic_type T>
	bigfloat& operator*=(const T&other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num*=abs(other);
		return*this;
	}
	//operator/=
	template<unsigned_type T>
	bigfloat& operator/=(const T& other)&noexcept{
		_num/=other;
		return*this;
	}
	template<arithmetic_type T>
	bigfloat& operator/=(const T&other)&noexcept{
		_is_negative=_is_negative!=is_negative(other);
		_num/=abs(other);
		return*this;
	}
	//operator==
	template<arithmetic_type T>
	[[nodiscard]]bool operator==(const T& other)const noexcept{
		if(_is_negative!=is_negative(other))return false;
		return _num==abs(other);
	}
	//operator<=>
	template<arithmetic_type T>
	[[nodiscard]]auto operator<=>(const T& other)const noexcept{
		if(_is_negative!=is_negative(other))
			return _is_negative?strong_ordering::less:strong_ordering::greater;
		const bool needs_reverse=_is_negative;
		auto result=_num <=> abs(other);
		if(needs_reverse)
			result=compare.reverse(result);
		return result;
	}
	//operatorX for rvalue
	template<arithmetic_type T>
	[[nodiscard]]bigfloat&& operator+(const T& other)&&noexcept{
		return move(*this += other);
	}
	template<arithmetic_type T>
	[[nodiscard]]bigfloat&& operator-(const T&other)&&noexcept{
		return move(*this -= other);
	}
	template<arithmetic_type T>
	[[nodiscard]]bigfloat&& operator*(const T&other)&&noexcept{
		return move(*this *= other);
	}
	template<arithmetic_type T>
	[[nodiscard]]bigfloat&& operator/(const T&other)&&noexcept{
		return move(*this /= other);
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
	//friend simplify
	friend decltype(auto) simplify(bigfloat& a)noexcept{
		a.simplify();
		return a;
	}
	friend decltype(auto) simplify(bigfloat&& a)noexcept{
		a.simplify();
		return move(a);
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

[[nodiscard]]inline bigfloat operator-(const ubigfloat&num)noexcept{
	return copy_as_negative(num);
}
[[nodiscard]]inline bigfloat operator-(ubigfloat&&num)noexcept{
	return copy_as_negative(move(num));
}

template<typename T>
concept bigfloat_cvref=type_info<remove_cvref<T>> == type_info<bigfloat>;

//pow of bigint
using math::pow;
template<signed_integer_type T,bigint_cvref bigint_t>
[[nodiscard]]inline bigfloat pow(bigint_t&&base,T&&exp)noexcept{
	if(is_negative(exp))
		return bigfloat{1u} / pow(forward<bigint_t>(base),abs(forward<T>(exp)));
	else
		return pow(forward<bigint_t>(base),abs(forward<T>(exp)));
}
template<signed_basic_integer_type T1,signed_integer_type T2>
[[nodiscard]]inline ubigfloat pow(T1&& base,T2&& exp)noexcept{
	return pow(bigint{forward<T1>(base)},forward<T2>(exp));
}

template<arithmetic_type T,bigfloat_cvref bigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator+(T&& lhs,bigfloat_t&& rhs)noexcept{
	return forward<bigfloat_t>(rhs) + forward<T>(lhs);
}
template<arithmetic_type T,bigfloat_cvref bigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator-(T&& lhs,bigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} - forward<bigfloat_t>(rhs);
}
template<arithmetic_type T,bigfloat_cvref bigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator*(T&& lhs,bigfloat_t&& rhs)noexcept{
	return forward<bigfloat_t>(rhs) * forward<T>(lhs);
}
template<arithmetic_type T,bigfloat_cvref bigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator/(T&& lhs,bigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} / forward<bigfloat_t>(rhs);
}

template<signed_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator+(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} + forward<ubigfloat_t>(rhs);
}
template<signed_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator-(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} - forward<ubigfloat_t>(rhs);
}
template<signed_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator*(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} * forward<ubigfloat_t>(rhs);
}
template<signed_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<bigfloat>)
[[nodiscard]]inline bigfloat operator/(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return bigfloat{forward<T>(lhs)} / forward<ubigfloat_t>(rhs);
}

//file_end

