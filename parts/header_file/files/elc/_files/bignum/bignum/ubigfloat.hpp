//ubigfloat.hpp
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

	template<unsigned_integer_type T>
	ubigfloat(T&&num)noexcept:_numerator(forward<T>(num)){}
	template<basic_float_type T>
	ubigfloat(T num)noexcept{
		if(isNaN(num))return;
		//将一个浮点类型无损的转换为两个bigint相除
		if(isInf(num)){
			_numerator = 1u;
			_denominator = zero;
			return;
		}
		if(is_negative(num))num = -num;
		const auto info=get_precision_and_exponent(num);
		if(info.exponent<0){//小数，有猜测价值
			//优先进行猜测以处理近似的简单分数
			const auto divide=to_divide(num);
			if(divide){
				_numerator = (ubigint)divide.numerator;
				_denominator = (ubigint)divide.denominator;
				return;
			}
		}
		_numerator = info.precision;
		_denominator = 1u;
		if(info.exponent){
			if(info.exponent>0)
				_numerator<<=info.exponent;
			else
				_denominator<<=abs(info.exponent);
		}
		simplify();
	}
public:
	template<arithmetic_type T>
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		if constexpr(type_info<T>!=type_info<ubigfloat>){
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
		if constexpr(integer_type<T>)
			if(_numerator%_denominator)
				return false;
		return true;
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()&&noexcept{//只对于右值，便于编写。
		//左值版本只需要创建一个临时变量，然后调用右值版本即可。
		if constexpr(integer_type<T>)
			return trunc(move(*this)).convert_to<T>();
		elseif constexpr(type_info<T>!=type_info<ubigfloat>){//浮点数
			if(!_denominator)
				if constexpr(has_inf<T>)
					return arithmetic_type_info_prover<T>::Inf();
				else
					return arithmetic_type_info_prover<T>::max();

			constexpr auto precision_base_bitnum = bitnum_of(float_precision_base_t<T>);
			//将大分数转换为合适的指数和基数的组合
			signed_specific_size_fast_t<max(sizeof(float_exponent_t<T>),sizeof(ptrdiff_t))> exp=0;
			//将分子分母的指数部分提取出来
			{
				const auto tmp=countr_zero(_numerator);
				_numerator>>=tmp;
				exp+=tmp;
			}
			{
				const auto tmp=countr_zero(_denominator);
				_denominator>>=tmp;
				exp-=tmp;
			}
			//将精度调整到T的精度，获取有精度损失但仍然可能过精的数
			{
				auto result=divmod(_numerator,_denominator);//同时获取商和余数
				while(result.mod && !(result.quot>>precision_base_bitnum)){
					_numerator<<=precision_base_bitnum;//扩大数字来容纳可能在除法中产生的数
					exp-=precision_base_bitnum;//指数相应的减少
					result=divmod(_numerator,_denominator);
				}
				_numerator=move(result.quot);//有损舍入精度
				const auto tmp=countr_zero(_numerator);
				_numerator>>=tmp;
				exp+=tmp;
			}
			{
				//对多余的精度进行舍入，仍然，这是可能有损的
				const auto bitnum_now=get_bitnum(_numerator);
				const ptrdiff_t diff=bitnum_now-bitnum_of(basic_environment::float_precision_base_t<T>);
				if(diff>0){
					_numerator>>=diff;
					++_numerator;
					exp+=diff;
				}
			}
			{
				//大数转换为基础类型
				const auto num=_numerator.convert_to<float_precision_base_t<T>>();
				//num与exp满足关系：num*BIT_POSSIBILITY^exp≈原始数

				//现在我们有了一个合适的基数和合适的指数！
				//接下来只需要将它们转换为T即可
				return make_float<T>(num,float_exponent_t<T>(exp));
			}
		}
		else
			return move(*this);
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()const&noexcept{
		if constexpr(integer_type<T>)
			return trunc(*this).convert_to<T>();
		else
			return ubigfloat(*this).convert_to<T>();
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
	ubigfloat(ubigint&& numerator, ubigint&& denominator)noexcept:
		_numerator(move(numerator)),
		_denominator(move(denominator))
	{}
	ubigfloat(const ubigint& numerator, const ubigint& denominator)noexcept:
		_numerator(numerator),
		_denominator(denominator)
	{}
public:
	//build_from_numerator_and_denominator
	static ubigfloat build_from_numerator_and_denominator(const ubigint& numerator,const ubigint& denominator)noexcept{
		return ubigfloat{numerator,denominator};
	}
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
	//friend reciprocal
	[[nodiscard]]friend ubigfloat reciprocal(const ubigfloat& a)noexcept{
		return ubigfloat{a._denominator,a._numerator};
	}
	[[nodiscard]]friend ubigfloat&& reciprocal(ubigfloat&& a)noexcept{
		return swap(a._denominator,a._numerator),move(a);
	}
	//friend get_numerator
	[[nodiscard]]friend const ubigint& get_numerator(const ubigfloat& a)noexcept{
		return a._numerator;
	}
	[[nodiscard]]friend ubigint&& get_numerator(ubigfloat&& a)noexcept{
		return move(a._numerator);
	}
	[[nodiscard]]friend ubigint& get_numerator_as_ref(ubigfloat& a)noexcept{
		return a._numerator;
	}
	//friend get_denominator
	[[nodiscard]]friend const ubigint& get_denominator(const ubigfloat& a)noexcept{
		return a._denominator;
	}
	[[nodiscard]]friend ubigint&& get_denominator(ubigfloat&& a)noexcept{
		return move(a._denominator);
	}
	[[nodiscard]]friend ubigint& get_denominator_as_ref(ubigfloat& a)noexcept{
		return a._denominator;
	}
	//friend isNaN
	[[nodiscard]]friend constexpr bool isNaN(const ubigfloat&)noexcept{
		return false;
	}
	//friend isInf
	[[nodiscard]]friend bool isInf(const ubigfloat&a)noexcept{
		return!a._denominator;
	}
	//friend trunc
	[[nodiscard]]friend ubigint trunc(const ubigfloat& a)noexcept{
		return a._numerator/a._denominator;
	}
	[[nodiscard]]friend ubigint trunc(ubigfloat&& a)noexcept{
		return move(a._numerator)/move(a._denominator);
	}
	//friend split
	template<class=void>
	struct split_result_t_base{
		ubigint integer;
		ubigfloat fraction;
	};
	typedef split_result_t_base<> split_result_t;
	[[nodiscard]]friend auto split(const ubigfloat& a)noexcept{
		const auto result=divmod(a._numerator,a._denominator);
		return split_result_t{move(result.quot),ubigfloat{move(result.mod),a._denominator}};
	}
	[[nodiscard]]friend auto split(ubigfloat&& a)noexcept{
		const auto result=divmod(move(a._numerator),a._denominator);
		return split_result_t{move(result.quot),ubigfloat{move(result.mod),move(a._denominator)}};
	}
	//friend trunc_with_sub
	[[nodiscard]]friend ubigint trunc_with_sub(ubigfloat& a)noexcept{
		const auto result=divmod(move(a._numerator),a._denominator);
		a._numerator=move(result.mod);
		return result.quot;
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const ubigfloat& a)noexcept{
		return to_size_t(trunc(a));
	}
	[[nodiscard]]friend size_t to_size_t(ubigfloat&& a)noexcept{
		return to_size_t(trunc(move(a)));
	}
	//operator+
	[[nodiscard]]ubigfloat operator+(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator + other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat operator+(T&& other)const&noexcept{
		auto numerator = _numerator + forward<T>(other) * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	template<signed_integer_type T>
	[[nodiscard]]ubigfloat operator+(T&& other)const&noexcept{
		if(is_negative(other))
			return *this - abs(forward<T>(other));
		else
			return *this + abs(forward<T>(other));
	}
	//operator-
	[[nodiscard]]ubigfloat operator-(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator - other._numerator * _denominator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat operator-(T&& other)const&noexcept{
		auto numerator = _numerator - forward<T>(other) * _denominator;
		return ubigfloat{move(numerator), _denominator};
	}
	template<signed_integer_type T>
	[[nodiscard]]ubigfloat operator-(T&& other)const&noexcept{
		if(is_negative(other))
			return *this + abs(forward<T>(other));
		else
			return *this - abs(forward<T>(other));
	}
	//operator*
	[[nodiscard]]ubigfloat operator*(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._numerator;
		auto denominator = _denominator * other._denominator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat operator*(T&& other)const&noexcept{
		auto numerator = _numerator * forward<T>(other);
		return ubigfloat{move(numerator), _denominator};
	}
	//operator/
	[[nodiscard]]ubigfloat operator/(const ubigfloat& other)const&noexcept{
		auto numerator = _numerator * other._denominator;
		auto denominator = _denominator * other._numerator;
		return ubigfloat{move(numerator), move(denominator)};
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat operator/(T&& other)const&noexcept{
		auto denominator = _denominator * forward<T>(other);
		return ubigfloat{_numerator, move(denominator)};
	}
	//operator+=
	ubigfloat& operator+=(const ubigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator + other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	template<unsigned_integer_type T>
	ubigfloat& operator+=(T&& other)&noexcept{
		_numerator += forward<T>(other) * _denominator;
		return*this;
	}
	template<signed_integer_type T>
	ubigfloat& operator+=(T&& other)&noexcept{
		if(is_negative(other))
			return*this -= abs(forward<T>(other));
		else
			return*this += abs(forward<T>(other));
	}
	//operator-=
	ubigfloat& operator-=(const ubigfloat& other)&noexcept{
		_numerator = _numerator * other._denominator - other._numerator * _denominator;
		_denominator *= other._denominator;
		return*this;
	}
	template<unsigned_integer_type T>
	ubigfloat& operator-=(T&& other)&noexcept{
		_numerator -= forward<T>(other) * _denominator;
		return*this;
	}
	template<signed_integer_type T>
	ubigfloat& operator-=(T&& other)&noexcept{
		if(is_negative(other))
			return*this += abs(forward<T>(other));
		else
			return*this -= abs(forward<T>(other));
	}
	//operator*=
	ubigfloat& operator*=(const ubigfloat& other)&noexcept{
		_numerator *= other._numerator;
		_denominator *= other._denominator;
		return*this;
	}
	template<unsigned_integer_type T>
	ubigfloat& operator*=(T&& other)&noexcept{
		_numerator *= forward<T>(other);
		return*this;
	}
	//operator/=
	ubigfloat& operator/=(const ubigfloat& other)&noexcept{
		_numerator *= other._denominator;
		_denominator *= other._numerator;
		return*this;
	}
	template<unsigned_integer_type T>
	ubigfloat& operator/=(T&& other)&noexcept{
		_denominator *= forward<T>(other);
		return*this;
	}
	//operator==
	[[nodiscard]]bool operator==(const ubigfloat& other)const noexcept{
		return _numerator * other._denominator == other._numerator * _denominator;
	}
	template<integer_type T>
	[[nodiscard]]bool operator==(T&& other)const noexcept{
		if constexpr(is_signed<T>)
			if(is_negative(other))return false;
		return _numerator == abs(forward<T>(other)) * _denominator;
	}
	//operator<=>
	[[nodiscard]]auto operator<=>(const ubigfloat& other)const noexcept{
		return _numerator * other._denominator <=> other._numerator * _denominator;
	}
	template<integer_type T>
	[[nodiscard]]auto operator<=>(T&& other)const noexcept{
		if constexpr(is_signed<T>)
			if(is_negative(other))return strong_ordering::greater;
		return _numerator <=> abs(forward<T>(other)) * _denominator;
	}
	//operatorX for rvalue
	[[nodiscard]]ubigfloat&& operator+(const ubigfloat& other)&&noexcept{
		return move(*this += other);
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat&& operator+(T&& other)&&noexcept{
		return move(*this += forward<T>(other));
	}
	template<signed_integer_type T>
	[[nodiscard]]ubigfloat&& operator+(T&& other)&&noexcept{
		return move(*this += forward<T>(other));
	}
	[[nodiscard]]ubigfloat&& operator-(const ubigfloat& other)&&noexcept{
		return move(*this -= other);
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat&& operator-(T&& other)&&noexcept{
		return move(*this -= forward<T>(other));
	}
	template<signed_integer_type T>
	[[nodiscard]]ubigfloat&& operator-(T&& other)&&noexcept{
		return move(*this -= forward<T>(other));
	}
	[[nodiscard]]ubigfloat&& operator*(const ubigfloat& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]ubigfloat&& operator*(ubigfloat&& other)&&noexcept{
		return move(*this *= move(other));
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat&& operator*(T&& other)&&noexcept{
		return move(*this *= other);
	}
	[[nodiscard]]ubigfloat&& operator/(const ubigfloat& other)&&noexcept{
		return move(*this /= other);
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigfloat&& operator/(T&& other)&&noexcept{
		return move(*this /= other);
	}
	[[nodiscard]]ubigfloat&& operator+(ubigfloat&& other)const&noexcept{
		return move(move(other) + *this);
	}
	[[nodiscard]]ubigfloat&& operator*(ubigfloat&& other)const&noexcept{
		return move(move(other) * *this);
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
	//friend simplify
	friend auto& simplify(ubigfloat& a)noexcept{
		a.simplify();
		return a;
	}
	friend decltype(auto) simplify(ubigfloat&& a)noexcept{
		a.simplify();
		return move(a);
	}
	//friend pow
	friend [[nodiscard]] ubigfloat pow(ubigfloat base,bigint exp)noexcept{
		ubigfloat aret;
		if(is_negative(exp))
			aret={base._denominator,base._numerator};
		else
			aret=base;
		ubigint uexp=abs(move(exp));
		return math::pow(move(aret),move(uexp));
	}
	//强转
	[[nodiscard]]explicit operator ubigint()const&noexcept{
		return trunc(*this);
	}
	[[nodiscard]]explicit operator ubigint()&&noexcept{
		return trunc(move(*this));
	}
	[[nodiscard]]explicit operator bigint()const&noexcept{
		return this->operator ubigint();
	}
	[[nodiscard]]explicit operator bigint()&&noexcept{
		return move(*this).operator ubigint();
	}
	//hash
	[[nodiscard]]force_inline hash_t hash()const noexcept{
		return hash_t{hash_n::hash(_numerator)._value*13+hash_n::hash(_denominator)._value};
	}
	//swap_with
	void swap_with(ubigfloat& other)noexcept{
		swap(_numerator,other._numerator);
		swap(_denominator,other._denominator);
	}
	//friend swap
	friend force_inline void swap(ubigfloat& a,ubigfloat& b)noexcept{
		a.swap_with(b);
	}
};

template<typename T>
concept ubigfloat_cvref=type_info<remove_cvref<T>> == type_info<ubigfloat>;

//pow of bigint
using math::pow;
template<signed_integer_type T,ubigint_cvref ubigint_t>
[[nodiscard]]inline ubigfloat pow(ubigint_t base,T&&exp)noexcept{
	if(is_negative(exp))
		return ubigfloat{1u} / pow(forward<ubigint_t>(base),abs(forward<T>(exp)));
	else
		return pow(forward<ubigint_t>(base),abs(forward<T>(exp)));
}
template<unsigned_basic_integer_type T1,signed_integer_type T2>
[[nodiscard]]inline ubigfloat pow(T1&& base,T2&& exp)noexcept{
	return pow(ubigint{forward<T1>(base)},forward<T2>(exp));
}

template<unsigned_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<ubigfloat>)
[[nodiscard]]inline ubigfloat operator+(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return forward<ubigfloat_t>(rhs) + forward<T>(lhs);
}
template<unsigned_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<ubigfloat>)
[[nodiscard]]inline ubigfloat operator-(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return ubigfloat{forward<T>(lhs)} - forward<ubigfloat_t>(rhs);
}
template<unsigned_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<ubigfloat>)
[[nodiscard]]inline ubigfloat operator*(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return forward<ubigfloat_t>(rhs) * forward<T>(lhs);
}
template<unsigned_type T,ubigfloat_cvref ubigfloat_t> requires(type_info<remove_cvref<T>> != type_info<ubigfloat>)
[[nodiscard]]inline ubigfloat operator/(T&& lhs,ubigfloat_t&& rhs)noexcept{
	return ubigfloat{forward<T>(lhs)} / forward<ubigfloat_t>(rhs);
}

BREAK_NAMESPACE
//注入math::arithmetic_type_info_prover
namespace base::math{
	using namespace bignum_n;
	inline ubigfloat arithmetic_type_info_prover<ubigfloat>::min()noexcept{
		return ubigfloat{};
	}
}
INTER_NAMESPACE(bignum_n)

//file_end

