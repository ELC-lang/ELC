//math.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
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
namespace math{
	using ::std::move;

	//isNaN
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool isNaN(const T&v)noexcept{
		if constexpr(has_NaN<T>)
			return v!=v;
		else
			return false;
	}
	//isInf
	template<basic_type T>
	[[nodiscard]]force_inline constexpr bool isInf(const T&v)noexcept{
		if constexpr(has_inf<T>)
			return v==arithmetic_type_info_prover<T>::Inf() || v==arithmetic_type_info_prover<T>::negative_Inf();
		else
			return false;
	}
	template<arithmetic_type T> requires(!has_inf<T>)
	[[nodiscard]]force_inline constexpr bool isInf(const T&)noexcept{
		return false;
	}
	/*! 符号位查询 */
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool is_negative(const T&x)noexcept{
		if constexpr(is_signed<T>){
			if constexpr(has_NaN<T>)
				if(isNaN(x))
					return false;
			return x<T{};
		}
		else
			return false;
	}
	/*! 符号位设置 */
	template<typename T=void,typename Arg_t=void>
	[[nodiscard]]force_inline constexpr auto copy_as_negative(Arg_t x,bool negative=1)noexcept{
		if constexpr(is_unsigned<T>)
			template_error("copy_as_negative:unsigned type");
		elseif constexpr(type_info<T> == type_info<void>)
			return copy_as_negative<to_signed_t<Arg_t>>(x,negative);
		elseif constexpr(is_basic_type<Arg_t>) {
			if constexpr(is_float_type<Arg_t>)
				return(T)::std::copysign(x,negative?-1:1);
			else
				return(T)negative?T{}-x:x;
		}
		else
			return(T)negative?T{}-x:x;
	}
	//copy_as_not_negative? 那不就是abs吗
	/*! 符号位反转 */
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr auto reverse_sign(const T&x)noexcept{
		return copy_as_negative(x,!is_negative(x));
	}
	/*! 符号位反转，如果X为真 */
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr auto reverse_sign_if(bool x,const T&y)noexcept{
		return x?reverse_sign(y):y;
	}
	/*! 倒数 */
	template<basic_float_type T>
	[[nodiscard]]force_inline constexpr auto reciprocal(const T&x)noexcept{
		return T{1}/x;
	}
	/*! 任意算数类型安全转型(ub避免.). */
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr T safe_arithmetic_cast(U x)noexcept{
		if constexpr(basic_float_type<U> && unsigned_type<T>)
			if(is_negative(x))
				return (T)(intmax_t)x;
			else{
				push_and_disable_msvc_warning(26467);//x已非负
				return (T)(uintmax_t)x;
				pop_msvc_warning();
			}
		else
			return (T)x;
	}
	/*! 任意算数类型转size_t */
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr size_t to_size_t(T x)noexcept{
		return safe_arithmetic_cast<size_t>(x);
	}
	/*! 任意算数类型转uintmax_t */
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr uintmax_t to_uintmax_t(T x)noexcept{
		return safe_arithmetic_cast<uintmax_t>(x);
	}
	/*! 求余 */
	template<arithmetic_type T1,arithmetic_type T2>
	[[nodiscard]]force_inline constexpr auto mod(const T1&a,const T2&b){
		if constexpr(basic_float_type<T1>||basic_float_type<T2>)
			return ::std::fmod(a,b);
		else
			return a%b;
	}
	//is_odd
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool is_odd(const T&x)noexcept{
		if constexpr(is_integer_type<T>)
			return x&1;
		else
			return mod(x,T(2));
	}
	//is_even
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool is_even(const T&x)noexcept{
		return !is_odd(x);
	}
	//divmod
	template<arithmetic_type T1,arithmetic_type T2> requires(!is_big_type<T1>&&!is_big_type<T2>)
	[[nodiscard]]force_inline constexpr auto divmod(const T1&a,const T2&b){
		typedef decltype(a/b) quot_t;
		typedef decltype(a%b) mod_t;
		struct divmod_result{
			quot_t quot;
			mod_t mod;
		};
		if constexpr(basic_float_type<T1>||basic_float_type<T2>)
			return divmod_result{::std::floor(a/b),::std::fmod(a,b)};
		else
			return divmod_result{a/b,a%b};
	}
	//ceil_div
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr auto ceil_div(const T&a,const U&b)noexcept{
		const auto[quot,mod]=divmod(a,b);
		return quot+(mod?1u:0u);
	}

	/*! 设置浮点舍入 */
	template<typename T>
	force_inline constexpr void set_rounding(int mode){
		if constexpr(basic_float_type<T>)
			::std::fesetround(mode);
	}
	/*!获取浮点舍入 */
	template<typename T>
	[[nodiscard]]force_inline constexpr int get_rounding(){
		if constexpr(basic_float_type<T>)
			return ::std::fegetround();
		else
			return 0;
	}
	/*! 设置并自动恢复浮点舍入 */
	template<typename T>
	struct rounding_auto_setter{
		int old_rounding;
		rounding_auto_setter(int new_rounding){
			old_rounding=get_rounding<T>();
			set_rounding<T>(new_rounding);
		}
		~rounding_auto_setter(){
			set_rounding<T>(old_rounding);
		}
	};

	//abs
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr auto abs(const T&v)noexcept{
		if constexpr(is_signed<T>){
			//符号转无符号而不是num=-num避免INT_MAX这种情况下的溢出
			typedef to_unsigned_t<T> UT;
			return is_negative(v)?UT(-v):UT(v);
		}
		else
			return v;
	}
	//is_close
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool is_close(const T&a,const T&b)noexcept{
		if constexpr(basic_float_type<T>)
			return abs(a-b)<=arithmetic_type_info_prover<T>::ε();
		else
			return a==b;
	}
	//sub
	template<arithmetic_type T1,arithmetic_type T2>
	[[nodiscard]]force_inline constexpr auto sub(const T1&a,const T2&b)noexcept{
		if constexpr(is_basic_type<T1>&&is_basic_type<T2>){
			if constexpr(is_float_type<T1>||is_float_type<T2>)
				return a-b;
			else{
				using signedT = ::std::make_signed_t<::std::common_type_t<T1,T2,signed char>>;
				return static_cast<signedT>(a)-static_cast<signedT>(b);
			}
		}
		else
			return a-b;
	}
	//exp
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<float_type T>
	[[nodiscard]]force_inline constexpr T exp(const T&v)noexcept{
		if(in_consteval||!is_basic_type<T>){
			typedef decltype(λ{
				if constexpr(is_basic_type<T>)
					return ::std::exp(T{});
				else
					return T{};
			}()) RT;
			auto exp_impl = recursive_λ(RT x,RT sum,RT n,size_t i,RT t)noexcept -> RT{
				const auto ε=sum+t/n;
				if(is_close(sum,ε))
					return sum;
				else
					return self_recursion(x,ε,n*i,i+1,t*x);
			};
			auto exp_impl_caller = get_recursive_λ_caller(exp_impl);
			return exp_impl_caller(RT{v},RT{1u},RT{1u},2,RT{v});
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::exp(v);
	}
	//log
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr auto log(const T&a)noexcept{
		if(in_consteval||!is_basic_type<T>){
			typedef decltype(λ()noexcept{
				if constexpr(is_basic_type<T>)
					return ::std::log(T{});
				else
					return T{};
			}()) RT;
			auto log_impl = recursive_λ(const RT x,const RT y)noexcept -> RT{
				auto log_iter = λ(RT x,RT y)noexcept{
					const auto exp_y = exp(y);
					return y + T{2u}*(x-exp_y)/(x+exp_y);
				};
				return RT(is_close(y,log_iter(x,y)) ? y : self_recursion(x,log_iter(x,y)));
			};
			auto log_impl_caller = get_recursive_λ_caller(log_impl);
			return log_impl_caller((RT)a,RT{0u});
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::log(a);
	}
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr auto log(const T&a,const U&b)noexcept{
		return log(a)/log(b);
	}
	//integer_log
	template<unsigned_integer_type T>
	[[nodiscard]]force_inline constexpr size_t integer_log(const T& a,const auto& b)noexcept{
		auto integer_log_impl = recursive_λ(const T& a,T& tester,const T this_lv,const size_t num)noexcept -> size_t{
			size_t aret=0;
			{
				const auto next_lv=this_lv*this_lv;
				if(next_lv<a)
					aret=self_recursion(a,tester,next_lv,num*2);
			}
			floop
				if(auto tmp=tester*this_lv;tmp>a)return aret;
				else{
					tester=move(tmp);
					aret+=num;
				}
		};
		auto integer_log_impl_caller = get_recursive_λ_caller(integer_log_impl,noexcept);
		T tester=1u;
		return integer_log_impl_caller(a,tester,move(b),1u);
	}

	//pow
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<arithmetic_type T,arithmetic_type U=unsigned> requires(!is_big_type<T>)
	[[nodiscard]]force_inline constexpr auto pow(const T&a,const U&b=2)noexcept{
		if(in_consteval||!is_basic_type<T>){
			typedef decltype(::std::pow(a,b)) RT;
			return RT(exp(b*log(a)));
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::pow(a,b);
	}
	template<big_type T,unsigned_integer_type U=unsigned>
	[[nodiscard]]force_inline constexpr auto pow(T a,U b=2)noexcept{
		if constexpr(is_signed<T>){
			const bool isneg=is_negative(a) && is_odd(b);
			const auto abs_ret=pow(abs(a),b);
			return copy_as_negative(abs_ret,isneg);
		}
		else{
			T aret=1u;
			while(b){
				if(is_odd(b))aret*=a;
				a*=a;
				b>>=1u;
			}
			return aret;
		}
	}
	//trunc
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<float_type T> requires is_basic_type<T>
	[[nodiscard]]force_inline constexpr auto trunc(const T&v)noexcept{
		if in_consteval{
			typedef decltype(::std::trunc(v)) RT;
			return static_cast<RT>((intmax_t)v);
		}
		else
			return ::std::trunc(v);
	}
	//ceil
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<float_type T> requires is_basic_type<T>
	[[nodiscard]]force_inline constexpr auto ceil(const T&v)noexcept{
		if in_consteval{
			auto ceil_impl = λ(T x, T y)noexcept{
				return is_close(x,y) ? y : y+T{1};
			};
			return v<0 ? -static_cast<T>(to_uintmax_t(-v)) : ceil_impl(v,static_cast<T>(to_uintmax_t(v)));
		}
		else
			return ::std::ceil(v);
	}
	//floor
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<float_type T> requires is_basic_type<T>
	[[nodiscard]]force_inline constexpr auto floor(const T&v)noexcept{
		if in_consteval{
			auto floor_impl = λ(T x, T y)noexcept{
				return is_close(x,y) ? y : y-T{1};
			};
			return v<0 ? -static_cast<T>(to_uintmax_t(-v+T{1})) : floor_impl(v,static_cast<T>(to_uintmax_t(v)));
		}
		else
			return ::std::floor(v);
	}
	//invsqrt
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T invsqrt(const T&v)noexcept;
	template<float_type T,arithmetic_type U=T>
	inline constexpr T invsqrt_iteration(const T&num,const U&v)noexcept{
		//newton-raphson
		return num*(3u-v*num*num)/2u;
	}
	template<float_type T,arithmetic_type U=T>
	inline constexpr T invsqrt_to_new_ε(T num,const U&v,const to_unsigned_t<T>&ε,to_unsigned_t<T>&ε_saver)noexcept{
		while(ε_saver>ε){
			auto next_ret=invsqrt_iteration(num,v);
			ε_saver=abs(next_ret-num);
			num=move(next_ret);
		}
		return num;
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T quick_invsqrt(const T&v)noexcept{
		if constexpr(BIT_POSSIBILITY==2)// evil floating point bit level hacking
			if(!in_consteval)//编译期计算不能使用union_cast，让编译器慢慢算去吧
				if constexpr(is_big_type<T>)
					return quick_invsqrt(static_cast<long double>(v));
				else{
					using namespace basic_environment::float_infos;
					return union_cast<T>((quick_invsqrt_magic_number<T> - union_cast<const data_type<T>>(v))>>1);
				}
		return 2u/v;
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T invsqrt(const T&v,const to_unsigned_t<T>&ε,to_unsigned_t<T>&ε_saver)noexcept{
		if constexpr(has_NaN<T> && has_inf<T>)
			if(v < 0u || v >= arithmetic_type_info_prover<T>::Inf())
				return arithmetic_type_info_prover<T>::NaN();
		return invsqrt_to_new_ε(quick_invsqrt(v),v,ε,ε_saver);
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T invsqrt(const T&v,const to_unsigned_t<T>&ε)noexcept{
		if constexpr(has_NaN<T> && has_inf<T>)
			if(v < 0u || v >= arithmetic_type_info_prover<T>::Inf())
				return arithmetic_type_info_prover<T>::NaN();
		return invsqrt_to_new_ε(quick_invsqrt(v),v,ε);
	}
	//invsqrt with one parameter
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T invsqrt(const T&v)noexcept{
		return invsqrt(v,arithmetic_type_info_prover<T>::ε());
	}
	template<integer_type T>
	[[nodiscard]]force_inline constexpr auto invsqrt(const T&v)noexcept{
		return invsqrt(static_cast<float_type_of<T>>(v));
	}
	//sqrt
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T sqrt(const T&v)noexcept;
	template<float_type T,arithmetic_type U=T>
	inline constexpr T sqrt_iteration(const T&num,const U&v)noexcept{
		//newton-raphson
		return (num+v/num)/2u;
	}
	template<float_type T,arithmetic_type U=T>
	inline constexpr T sqrt_to_new_ε(T num,const U&v,const to_unsigned_t<T>&ε,to_unsigned_t<T>&ε_saver)noexcept{
		while(ε_saver>ε){
			auto next_ret=sqrt_iteration(num,v);
			ε_saver=abs(next_ret-num);
			num=move(next_ret);
		}
		return num;
	}
	template<float_type T,arithmetic_type U=T>
	inline constexpr T sqrt_to_new_ε(T num,const U&v,const to_unsigned_t<T>&ε)noexcept{
		auto ε_saver=to_unsigned_t<T>{ε*BIT_POSSIBILITY};
		return sqrt_to_new_ε(num,v,ε,ε_saver);
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T quick_sqrt(const T&v)noexcept{
		if constexpr(BIT_POSSIBILITY==2)// evil floating point bit level hacking
			if(!in_consteval)//编译期计算不能使用union_cast，让编译器慢慢算去吧
				if constexpr(is_big_type<T>)
					return quick_sqrt(static_cast<long double>(v));
				else{
					using namespace basic_environment::float_infos;
					return union_cast<T>((union_cast<const data_type<T>>(v)>>1) + (data_type<T>(exponent_diff<T>) << (precision_base_bit<T> - 1)));
				}
		return v/2u;
		//return reciprocal(quick_invsqrt(v));
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T sqrt(const T&v,const to_unsigned_t<T>&ε,to_unsigned_t<T>&ε_saver)noexcept{
		if constexpr(has_NaN<T> && has_inf<T>)
			if(v < 0u || v >= arithmetic_type_info_prover<T>::Inf())
				return arithmetic_type_info_prover<T>::NaN();
		return sqrt_to_new_ε(quick_sqrt(v),v,ε,ε_saver);
	}
	template<float_type T>
	[[nodiscard]]inline constexpr T sqrt(const T&v,const to_unsigned_t<T>&ε)noexcept{
		if constexpr(has_NaN<T> && has_inf<T>)
			if(v < 0u || v >= arithmetic_type_info_prover<T>::Inf())
				return arithmetic_type_info_prover<T>::NaN();
		return sqrt_to_new_ε(quick_sqrt(v),v,ε);
	}
	//sqrt with one parameter
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T sqrt(const T&v)noexcept{
		return sqrt(v,arithmetic_type_info_prover<T>::ε());
	}
	template<integer_type T>
	[[nodiscard]]force_inline constexpr auto sqrt(const T&v)noexcept{
		return sqrt(static_cast<float_type_of<T>>(v));
	}
	/*! 判断某数是否是素数,无预先检查以供其他素数相关函数快速调用. */
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr bool is_prime_num_no_pre_check(T a)noexcept{
		/*
		应某人的要求补注释(都是主人的任务罢了.).
		建议先看有预先检查的部分以便理解思路.
		*/
		T b=safe_arithmetic_cast<T>(sqrt(a));//若一个数可以分解为两因数之积,其中一个因数必定≤其开方:反指数式减少遍历范围.
		/*
		接下来:
		设要判定的数n(6x±1的缩写).
		测试数可以看为6i-1 6i 6i+1 6i+2 6i+3 6i+4的其中之一,同样以6为周期.
		对于测试数的整个周期,其中:
			如果n能被6i 6i+2 6i+4整除:则n要是偶数,但±1,排除.
			如果n能被6i+3整除:则n要是3倍数,但±1,排除.
		综上,循环中只要考虑6i±1的情况.
		所以以5起始,前进6(自然选择号,前进4!),逐次判断0与+2.
		虽然很想写成以6为起始逐次判断±1的对称格式但是这样会加重时空负担.(不甘心.....)
		*/
		for(T c=5u;c<=b;c+=6u)//遍历判断是否能被因数分解——不会有人看不懂吧?
			if((!mod(a,c))||(!mod(a,(c+2u))))
				return false;
		/*
		最后,为什么是6?
		就结论来说,此数值选择最常出现的两因数(除0或1外.)之积可以最大程度减少时间占用.
		所以虽然更喜欢7/8/9之类的数不过使用6收益最大.
		要不是这样早就写成7了.
		*/
		return true;
	}
	/*! 判断某数是否是素数 */
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr bool is_prime_num(T a)noexcept{
		if constexpr(is_float_type<T>)
			if(a != ceil(a))
				return false;
		auto b=abs(a);

		if(b<4)
			return true;//1和0也是prime,我不管.
		/*
		当x≥1,那么≥5的自然数如下:
		6x-1 6x 6x+1 6x+2 6x+3 6x+4
		6(x+1)-1 6(x+1) 6(x+1)+1 ... //这已经是下一周期了.

		考虑单个周期:
		6x+2 6x+3 6x+4 是 2(3x+1) 3(2x+1) 2(3x+2),排除.
		6x,排除.
		那么,只用考虑6x±1是否是prime.
		*/
		if(mod(mod(b,6)-1u,4u))
			return false;
		/*
		因为no_pre_check判定没有考虑到≤5的数,所以本函数第一个if进行判定补全.
		*/
		return is_prime_num_no_pre_check(b);
	}
	/// 求小于或等于某数的素数
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr T get_prime_num_less_or_eq_than(T a)noexcept;
	/// 求大于或等于某数的素数
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr T get_prime_num_big_or_eq_than(T a)noexcept{
		if constexpr(is_float_type<T>)
			a=ceil(a);
		if constexpr(is_signed<T>)
			if(is_negative(a)){
				auto tmp = get_prime_num_less_or_eq_than(abs(a));
				return copy_as_negative(tmp);
			}
		if(a<4u)return a;
		//将a转换为6x-1的形式.
		{
			T b=mod(a,6u);
			if(b==1u && is_prime_num_no_pre_check(a))return a;
			a+=b?5u-b:1u;
		}
		//循环判断.
		for(;;a+=6u)
			if(is_prime_num_no_pre_check(a))return a;
			elseif(is_prime_num_no_pre_check(a+2u))return a+2u;
	}
	/// 求小于或等于某数的素数
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr T get_prime_num_less_or_eq_than(T a)noexcept{
		if constexpr(is_float_type<T>)
			a=floor(a);
		if constexpr(is_signed<T>)
			if(is_negative(a)){
				auto tmp = get_prime_num_big_or_eq_than(abs(a));
				return copy_as_negative(tmp);
			}
		if(a<4u)return a;
		//将a转换为6x-1的形式.
		{
			T b=mod(a,6u);
			if(b==1u && is_prime_num_no_pre_check(a))return a;
			a-=b?b-1u:5u;
		}
		//循环判断.
		for(;;a-=6u)
			if(is_prime_num_no_pre_check(a))return a;
			elseif(is_prime_num_no_pre_check(a-2u))return a-2u;
	}
	/// 求大于某数的素数
	template<arithmetic_type T>
	[[nodiscard]]inline constexpr auto get_prime_num_big_than(T a)noexcept{
		return get_prime_num_big_or_eq_than(a+1u);
	}
	push_and_disable_msvc_warning(26467);//φ永远为正数
	/// 已知当前array的size，求下一个合适的提前分配大小
	template<unsigned_integer_type size_T>
	[[nodiscard]]inline constexpr size_T get_next_gold_size_to_resize_for_array(size_T size)noexcept{
		/*
		每次扩容后的空间与原空间比大致为φ可以最小化时空负担.
		*/
		return size_T(size*magic_number::φ);
	}
	/// 已知当前hash table的size，求下一个合适的桶大小
	template<unsigned_integer_type size_T>
	[[nodiscard]]inline constexpr size_T get_next_gold_size_to_resize_for_hash(size_T size)noexcept{
		/*
		素数大小的桶数可以使hash table中的每个桶尽可能活跃.
		每次扩容后的空间与原空间比大致为φ可以最小化时空负担.
		*/
		return size_T(get_prime_num_big_or_eq_than(size*magic_number::φ));
	}
	pop_msvc_warning();

	/// 线性插值.
	namespace linear_interpolation{
		template<arithmetic_type T1,arithmetic_type T2,arithmetic_type result_T>
		[[nodiscard]]inline constexpr auto get_k(T1 y1,T2 y2,result_T δx)noexcept{
			auto δy=sub(y2,y1);
			auto k=δy/δx;
			return k;
		}
		template<arithmetic_type T1,arithmetic_type T2>
		[[nodiscard]]inline constexpr auto get_k(T1 y1,T2 y2)noexcept{
			return get_k(y1,y2,1.0);
		}
		[[nodiscard]]inline constexpr auto get_result(auto y1,auto k,auto δx)noexcept{
			auto diff=k*δx;
			return y1+diff;
		}
		[[nodiscard]]inline constexpr auto get_reverse_result(auto k,auto δx,auto y)noexcept{
			auto diff=k*δx;
			return y-diff;
		}
	}
	//质因数分解
	//由于array在之后定义所以泛型容器
	//用法 get_prime_factorization(num).to<容器类型>(); 或 get_prime_factorization(num).unique().to<容器类型>();
	constexpr struct get_prime_factorization_t{
		template<class T>
		class result_t{
			T _m;
		public:
			template<class container_t>
			static constexpr bool able=was_not_an_ill_form(declvalue(container_t&).push_back(declvalue(T)));

			constexpr force_inline result_t(T m)noexcept:_m(m){}

			template<class container_t> requires able<container_t>
			[[nodiscard]]constexpr container_t to()const noexcept{
				container_t aret{};
				//思路：获取小于等于m的所有素数，依次除以m，若能整除则将素数加入结果，否则跳过，直到m为1.
				T i=T{2u},m=abs(_m);
				while(m!=1){
					while(!(m%i)){
						aret.push_back(i);
						m/=i;
					}
					i=get_prime_num_big_than(i);
				}
				if constexpr(signed_type<T>)
					if(is_negative(_m))
						aret.push_back(T{-1});
				return aret;
			}
			//operator container_t()
			template<class container_t>
			[[nodiscard]]constexpr force_inline operator container_t()const noexcept{
				return to<container_t>();
			}
			class unique_t{
				T _m;
			public:
				constexpr force_inline unique_t(T m)noexcept:_m(m){}

				template<class container_t> requires able<container_t>
				[[nodiscard]]constexpr container_t to()const noexcept{
					container_t aret{};
					//思路：获取小于等于m的所有素数，依次除以m，若能整除则将素数加入结果，否则跳过，直到m为1.
					T i=T{2u},m=abs(_m);
					while(m!=1){
						if(!(m%i)){
							aret.push_back(i);
							do m/=i;while(!(m%i));
						}
						i=get_prime_num_big_than(i);
					}
					if constexpr(signed_type<T>)
						if(is_negative(_m))
							aret.push_back(T{-1});
					return aret;
				}
				//operator container_t()
				template<class container_t>
				[[nodiscard]]constexpr force_inline operator container_t()const noexcept{
					return to<container_t>();
				}
			};
			[[nodiscard]]constexpr force_inline auto unique()const noexcept{
				return unique_t{_m};
			}
		};
		template<arithmetic_type T>
		[[nodiscard]]constexpr force_inline auto operator()(T m)const noexcept{
			return result_t<T>{m};
		}
	}get_prime_factorization{};
	//编译时质因数分解到质因数表
	template<unsigned_integer_type number_T,number_T number>
	struct prime_factorization_table_t{
		typedef prime_factorization_table_t<number_T,number> this_t;
		static constexpr size_t table_size=λ{
			number_T i=2,m=number;
			size_t aret=0;
			while(m!=1){
				while(!(m%i)){
					++aret;
					m/=i;
				}
				i=get_prime_num_big_than(i);
			}
			return aret;
		}();
		number_T _table[table_size];
		constexpr force_inline prime_factorization_table_t()noexcept{
			number_T i=2,m=number;
			number_T index=0;
			while(m!=1){
				while(!(m%i)){
					_table[index++]=i;
					m/=i;
				}
				i=get_prime_num_big_than(i);
			}
		}
		[[nodiscard]]constexpr force_inline number_T operator[](size_t index)const noexcept{
			return _table[index];
		}
		//begin and end for range for
		[[nodiscard]]constexpr force_inline number_T* begin()noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline number_T* end()noexcept{
			return _table+table_size;
		}
		[[nodiscard]]constexpr force_inline const number_T* begin()const noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline const number_T* end()const noexcept{
			return _table+table_size;
		}
		//size
		[[nodiscard]]constexpr force_inline size_t size()const noexcept{
			return table_size;
		}
		//friend
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto size_of_array_like(const this_t&)noexcept{
			return table_size;
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto begin_of_array_like(this_t&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto end_of_array_like(this_t&table)noexcept{
			return table.end();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto begin_of_array_like(const this_t&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto end_of_array_like(const this_t&table)noexcept{
			return table.end();
		}
	};
	//编译时的唯一质因数分解到质因数表
	template<unsigned_integer_type number_T,number_T number>
	struct unique_prime_factorization_table_t{
		typedef unique_prime_factorization_table_t<number_T,number> this_t;
		static constexpr size_t table_size=λ{
			number_T i=2,m=number;
			size_t aret=0;
			while(m!=1){
				if(!(m%i)){
					++aret;
					do m/=i;while(!(m%i));
				}
				i=get_prime_num_big_than(i);
			}
			return aret;
		}();
		number_T _table[table_size]={};
		constexpr force_inline unique_prime_factorization_table_t()noexcept{
			number_T i=2,m=number;
			size_t index=0;
			while(m!=1){
				if(!(m%i)){
					_table[index++]=i;
					do m/=i;while(!(m%i));
				}
				i=get_prime_num_big_than(i);
			}
		}
		[[nodiscard]]constexpr force_inline number_T operator[](size_t index)const noexcept{
			return _table[index];
		}
		//begin and end for range for
		[[nodiscard]]constexpr force_inline number_T* begin()noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline number_T* end()noexcept{
			return _table+table_size;
		}
		[[nodiscard]]constexpr force_inline const number_T* begin()const noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline const number_T* end()const noexcept{
			return _table+table_size;
		}
		//size
		[[nodiscard]]constexpr force_inline size_t size()const noexcept{
			return table_size;
		}
		//friend
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto size_of_array_like(const this_t&)noexcept{
			return table_size;
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto begin_of_array_like(this_t&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto end_of_array_like(this_t&table)noexcept{
			return table.end();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto begin_of_array_like(const this_t&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<number_T>)
		[[nodiscard]]friend inline auto end_of_array_like(const this_t&table)noexcept{
			return table.end();
		}
	};
	//阶乘快速计算
	template<unsigned_integer_type T>
	[[nodiscard]]constexpr inline auto factorial(unsigned n,unsigned start=1u)noexcept{
		constexpr size_t threshold=1<<4;
		if (n < threshold) {
			T s = 1u;
			unsigned end = start + n;
			while(start < end)
				s *= start++;
			return s;
		}
		unsigned m = (n+1u)/2;
		return factorial<T>(m,start)*factorial<T>(n-m,start+m);
	}
}
using math::is_negative;
using math::copy_as_negative;
using math::reverse_sign;
using math::reverse_sign_if;
using math::to_size_t;
using math::mod;
using math::is_odd;
using math::is_even;
using math::divmod;
using math::ceil_div;
using math::set_rounding;
using math::get_rounding;
using math::rounding_auto_setter;
using math::is_close;
using math::isNaN;
using math::isInf;
using math::abs;
using math::exp;
using math::log;
using math::integer_log;
using math::pow;
using math::ceil;
using math::floor;
using math::invsqrt_iteration;
using math::invsqrt_to_new_ε;
using math::quick_invsqrt;
using math::invsqrt;
using math::sqrt_iteration;
using math::sqrt_to_new_ε;
using math::quick_sqrt;
using math::sqrt;
using math::trunc;
using math::is_prime_num;
using math::get_prime_num_big_or_eq_than;
using math::get_prime_num_big_than;
using math::get_next_gold_size_to_resize_for_array;
using math::get_next_gold_size_to_resize_for_hash;
using math::get_prime_factorization;
using math::prime_factorization_table_t;
using math::unique_prime_factorization_table_t;
using math::factorial;

namespace bit{
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countl_zero(const T v)noexcept;
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countr_zero(const T v)noexcept;
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countl_one(const T v)noexcept;
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countr_one(const T v)noexcept;
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t get_bitnum(const T v)noexcept;
	template<signed_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t get_bitnum(const T v)noexcept;
}
using namespace bit;//干净的符号导出！

namespace math{
	using ::std::move;

	//求出最大公约数
	template<integer_type T>
	[[nodiscard]]inline auto binary_gcd(T x, T y)noexcept{
		static_assert(BIT_POSSIBILITY==2);
		size_t shift = 0;
		while(y){
			//都是偶数时，shift++并位移
			const auto x_rzero=countr_zero(x);
			const auto y_rzero=countr_zero(y);
			shift+=min(x_rzero,y_rzero);
			//一偶一奇时，只移动偶数 直到都是奇数
			x>>=x_rzero;
			y>>=y_rzero;
			//都是奇数时，大数-=小数
			if(x < y)swap(x, y);
			x-=y;
		}
		// 返回 x 左移 shift 位的结果
		return move(x) << shift;
	}
	//求出最大公约数：欧几里得算法
	template<integer_type T>
	[[nodiscard]]inline auto euclidean_gcd(T x, T y)noexcept{
		if constexpr(is_signed<T>)
			if(is_negative(x) && is_negative(y))
				return euclidean_gcd(abs(x), abs(y));
		while(y){
			if(x < y)swap(x, y);
			x = swap(y, x%y);
		}
		return x;
	}
	template<integer_type T>
	[[nodiscard]]inline auto base_gcd(T x, T y)noexcept{
		if constexpr(BIT_POSSIBILITY==2)
			return binary_gcd(move(x), move(y));
		else
			return euclidean_gcd(move(x), move(y));
	}
	template<unsigned_big_integer_type T>
	[[nodiscard]]inline auto lehmer_gcd(T x, T y)noexcept{
		//假设x≥y
		if(x < y)swap(x, y);

		size_t m=y.size_in_base_type();
		if(m <= 1)return base_gcd(move(x), move(y));//如果y只包含一位数（或根本没有），则使用其他方法求出结果
		T A, B, C, D;
	outer_loop:
		while(x && y) {//外循环
			--m;//提高精度
			T a = x.right_shift_in_base_type(m), b = y.right_shift_in_base_type(m);//取最显著数位
			/*初始化矩阵[A B a]为扩展的同一矩阵[1 0 a]
					  [C D b]			   [0 1 b]
			*/
			A = D = 1u; B = C = T{};
			//对(a+A,b+C)和(a+B,b+D)同时执行欧几里得算法，直到商不同为止
			do{//内循环
				const auto w = (a+A) / (b+C);//w为欧几里得算法长除法链中当前长除法的商
				{
					const auto w2 = (a+B) / (b+D);
					if(w != w2)break;
				}
				/*根据扩展欧几里得算法的矩阵表述替换当前矩阵[A B a]到矩阵积[0  1]	[A B a]   [C	D	 a	 ]
													 [C D b]			  ⋅			=
																   [1 −w]	[C D b]   [A−wC B−wD a−wb]
				*/
				A-=w*C;swap(A,C);
				B-=w*D;swap(B,D);
				a-=w*b;swap(a,b);
			}while(B);//B≠0则继续
			if(!B){//B=0意味着陷入了死锁；执行欧几里得算法的正常步骤，然后重新开始外循环
				x = swap(y, x%y);
				goto outer_loop;
			}
			//将对压缩形式的前数位执行的欧几里得算法步骤应用到x和y上
			auto new_x = A*x+B*y, new_y = C*x+D*y;
			x = move(new_x),y = move(new_y);
		}
		return x;
	}
	template<basic_integer_type T>
	[[nodiscard]]force_inline auto lehmer_gcd(T x, T y)noexcept{
		return euclidean_gcd(move(x), move(y));
	}
	template<signed_big_integer_type T>
	[[nodiscard]]force_inline auto lehmer_gcd(T x, T y)noexcept{
		const bool sign = is_negative(x) && is_negative(y);
		return copy_as_negative(gcd(abs(move(x)), abs(move(y))), sign);
	}
	
	template<integer_type T>
	[[nodiscard]]inline auto gcd(T x, T y)noexcept{
		if constexpr(BIT_POSSIBILITY==2){
			if constexpr(is_big_type<T>){
				constexpr size_t lehmer_threshold = 1<<8;
				if(max(x.size_in_base_type(),y.size_in_base_type()) >= lehmer_threshold)
					return lehmer_gcd(move(x), move(y));
			}
			return binary_gcd(move(x), move(y));
		}
		else
			return lehmer_gcd(move(x), move(y));
	}
}
using math::gcd;

namespace magic_number{
	using ::std::move;

	template<unsigned_big_float_type T>
	struct π_with_ε_impl_t{
	private:
		typedef to_signed_t<T> float_t;
		typedef to_unsigned_t<T> ufloat_t;
		typedef integer_type_of<T> int_t;
		typedef to_unsigned_t<int_t> uint_t;

		ufloat_t ε=reciprocal(ufloat_t{30u});
		float_t result{}, sum=ε;
		/*论证6k、3k、k不需要用大数：
		6k的迭代次数是max(type_info<size_t>) / 6
		考虑每次迭代π的有效位数增加14位（10进制下），那么即使在64bit的古董计算机下6k溢出时π可以达到的有效位数大约是4.3042403e+19个（十进制位）
		这远远超过了一般的计算机算力和大多数的精度要求，所以我们可以放心地使用size_t而不是uint_t来存储6k的值，更不用说3k和k了
		*/
		size_t k{}, _3k{}, _6k{}; bool sign = true;
		uint_t _545140134k_p13591409 = 13591409u; // 545140134*k+13591409
		uint_t _3k_factorial = 1u, _6k_factorial = 1u, k_factorial_pow_3 = 1u;
		ufloat_t sqrt_640320_ε_saver=reciprocal(ufloat_t{5u});
		ufloat_t sqrt_640320 = 800u;//我们需要缓存这个与ε有关的值 以便在下一次更高精度的迭代中对缓存的值进行利用
		uint_t _640320_pow_3kplus1 = 640320u;//初始值1次方
		static constexpr auto _640320pow3 = 262537412640768000u;
		size_t base_accurate = 0,sqrt_accuracy_multiplier = 3;//考虑到sqrt_iteration每次都会精度翻倍，而base_iteration每次只会精度增加14，所以我们可以通过追加这个值来省去一些不必要的sqrt迭代

	public:
		constexpr π_with_ε_impl_t()noexcept{}
		[[nodiscard]]constexpr T take_value()noexcept{
			return reciprocal(12u*abs(base_accurate?result:21u)/sqrt_640320);
		}
		constexpr void do_base_iteration()noexcept{
			sum = copy_as_negative(ufloat_t
				(				_545140134k_p13591409 * _6k_factorial				)
				/ //= --------------------------------------------------------
				(		_3k_factorial*k_factorial_pow_3 * _640320_pow_3kplus1		)
			,sign);
			for times(3) _3k_factorial *= ++_3k;
			for times(6) _6k_factorial *= ++_6k;
			k_factorial_pow_3 *= pow<uint_t>(++k,3u);
			_545140134k_p13591409 += 545140134u;
			_640320_pow_3kplus1 *= _640320pow3;
			sign = !sign;

			result += sum;
			base_accurate += 14;
		}
		constexpr void do_sqrt_iteration()noexcept{
			sqrt_640320=sqrt_iteration(sqrt_640320,640320u);
			sqrt_accuracy_multiplier++;
		}
		constexpr void do_iteration()noexcept{
			do_base_iteration();
			if(base_accurate && (base_accurate-8) >> sqrt_accuracy_multiplier)
				do_sqrt_iteration();
		}
		constexpr void clean_up()noexcept{
			simplify(sqrt_640320);
			simplify(result);
		}
		[[nodiscard]]constexpr T operator()()noexcept{
			if(!base_accurate)do_base_iteration();
			return take_value();
		}
		[[nodiscard]]constexpr T operator()(ufloat_t new_ε)noexcept{
			if(new_ε < ε){
				if(sqrt_640320_ε_saver>new_ε)//ε变小了，我们需要重新计算sqrt_640320
					sqrt_640320=sqrt_to_new_ε(sqrt_640320,640320u,new_ε,sqrt_640320_ε_saver);
				//更新ε.
				ε = move(new_ε);
				while (abs(sum) > ε)
					do_base_iteration();
			}
			return take_value();
		}
	};
	template<arithmetic_type T>
	using π_type = to_unsigned_t<float_type_of<remove_cvref<T>>>;
	distinctive inline constexpr struct π_with_ε_t{
		template<unsigned_big_float_type T>
		static inline π_with_ε_impl_t<T> for_type{};
		template<float_type T>
		auto operator()(T&&ε)const noexcept{
			if constexpr(is_basic_float_type<T>)
				return (T)π;//啊？
			else
				return for_type<T>(forward<T>(ε));
		}
	}π_with_ε{};
}
using magic_number::π_with_ε;

namespace math{
	template<signed_float_type T>
	[[nodiscard]]constexpr T sin(T num, const to_unsigned_t<T>&ε)noexcept{
		size_t i = 1; bool negation = false; // 取反
		T base = num, result = num, num_pow = pow(num); // 求num2的次方
		while (abs(base) > ε) {
			base *= num_pow/((i+1)*(i+2)); // 求阶乘
			negation = !negation; // 每次循环取反
			result += reverse_sign_if(negation,base); // 求和
			i += 2;
		}
		return result;
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T sin(T num)noexcept{
		return sin(num, arithmetic_type_info_prover<T>::ε());
	}

	template<float_type T>
	[[nodiscard]]constexpr T arctan(T num, const to_unsigned_t<T>&ε)noexcept{
		/*
		arctan (-x) = -arctan(x)
		arctan (1/x) = 0.5 * π - arctan(x) [x > 0]
		*/
		if constexpr(signed_type<T>)
			if (num < 0) return -arctan(abs(num), ε);
		if (num > 1) return π_with_ε(ε)/2u - arctan(reciprocal(move(num)), ε);

		size_t i = 1; bool negation = false; // 取反
		to_signed_t<T> result = num;
		T numerator_base = num, sum, num_pow = pow(num); // 求num2的次方
		do {
			numerator_base *= num_pow; // 求阶乘
			negation = !negation; // 每次循环取反
			sum = numerator_base/i; // 单步计算结果
			result += reverse_sign_if(negation,sum); // 求和
			i += 2;
		}while (abs(sum) > ε);
		return (T)result;
	}
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T arctan(T num)noexcept{
		return arctan(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T cos(T num, const to_unsigned_t<T>&ε)noexcept{
		return sin(π_with_ε(ε)/2u - num, ε);
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T cos(T num)noexcept{
		return cos(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T tan(T num, const to_unsigned_t<T>&ε)noexcept{
		return sin(num, ε)/cos(num, ε);
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T tan(T num)noexcept{
		return tan(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T cot(T num, const to_unsigned_t<T>&ε)noexcept{
		return cos(num, ε)/sin(num, ε);
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T cot(T num)noexcept{
		return cot(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T sec(T num, const to_unsigned_t<T>&ε)noexcept{
		return reciprocal(cos(num, ε));
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T sec(T num)noexcept{
		return sec(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T csc(T num, const to_unsigned_t<T>&ε)noexcept{
		return reciprocal(sin(num, ε));
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T csc(T num)noexcept{
		return csc(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr to_unsigned_t<T> arccos(T num, const to_unsigned_t<T>&ε)noexcept{
		return to_unsigned_t<T>(π_with_ε(ε)/2u - arctan(num, ε));
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr to_unsigned_t<T> arccos(T num)noexcept{
		return arccos(num, arithmetic_type_info_prover<T>::ε());
	}

	template<float_type T>
	[[nodiscard]]constexpr T arcsin(T num, const to_unsigned_t<T>&ε)noexcept{
		return arctan(num/reciprocal(sqrt(1-pow(num), ε)), ε);
	}
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T arcsin(T num)noexcept{
		return arcsin(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr to_unsigned_t<T> arccot(T num, const to_unsigned_t<T>&ε)noexcept{
		return to_unsigned_t<T>(π_with_ε(ε)/2u - arctan(num, ε));
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr to_unsigned_t<T> arccot(T num)noexcept{
		return arccot(num, arithmetic_type_info_prover<T>::ε());
	}

	template<signed_float_type T>
	[[nodiscard]]constexpr T arcsec(T num, const to_unsigned_t<T>&ε)noexcept{
		return arccos(reciprocal(num), ε);
	}
	template<signed_float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T arcsec(T num)noexcept{
		return arcsec(num, arithmetic_type_info_prover<T>::ε());
	}

	template<float_type T>
	[[nodiscard]]constexpr T arccsc(T num, const to_unsigned_t<T>&ε)noexcept{
		return arcsin(reciprocal(num), ε);
	}
	template<float_type T> requires(has_ε<T>)
	[[nodiscard]]force_inline constexpr T arccsc(T num)noexcept{
		return arccsc(num, arithmetic_type_info_prover<T>::ε());
	}
}
using math::sin;
using math::cos;
using math::tan;
using math::arccos;
using math::arcsin;
using math::arctan;
using math::cot;
using math::sec;
using math::csc;
using math::arccot;
using math::arcsec;
using math::arccsc;

//file_end

