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
	/// 算术类型帮助类型
	/// 任何后续定义的新算术类型都可以重载这些类型来实现数学库的泛型支持
	template<typename T>
	struct arithmetic_type_info_prover{
		//bool：是否是算数类型
		static constexpr bool is_arithmetic_type=::std::is_arithmetic_v<T> || is_elc_expansion_base_type<T>();
		//bool：是否是基础类型
		static constexpr bool is_basic_type=::std::is_arithmetic_v<T>;
		//bool：是否是大数类型
		static constexpr bool is_big_type=false;
		//bool：是否是浮点类型
		static constexpr bool is_float_type=::std::is_floating_point_v<T>;
		//bool：是否是有符号类型
		static constexpr bool is_signed=::std::is_signed_v<T>;
		//bool：是否有NaN
	private:
		static constexpr bool has_NaN_helper()noexcept{
			if constexpr(is_arithmetic_type)
				return ::std::numeric_limits<T>::has_quiet_NaN || ::std::numeric_limits<T>::has_signaling_NaN;
			else
				return false;
		}
	public:
		static constexpr bool has_NaN=has_NaN_helper();
		//bool：是否有inf
	private:
		static constexpr bool has_inf_helper()noexcept{
			if constexpr(is_arithmetic_type)
				return ::std::numeric_limits<T>::has_infinity;
			else
				return false;
		}
	public:
		static constexpr bool has_inf=has_inf_helper();
		//对应的无符号和有符号类型
		using unsigned_type=decltype(lambda{
			if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
				return T();
			elseif constexpr(::std::is_signed_v<T>)
				return::std::make_unsigned_t<T>();
		}());
		using signed_type=decltype(lambda{
			if constexpr(::std::is_signed_v<T>||::std::is_floating_point_v<T>)
				return T();
			elseif constexpr(::std::is_unsigned_v<T> && (type_info<T>!=type_info<bool>))
				return::std::make_signed_t<T>();
		}());
	};
	/*! 无符号位的对应类型 */
	template<typename T>
	using to_unsigned_t = typename arithmetic_type_info_prover<remove_cvref<T>>::unsigned_type;
	/*! 有符号位的对应类型 */
	template<typename T>
	using to_signed_t = typename arithmetic_type_info_prover<remove_cvref<T>>::signed_type;

	/// 概念名称空间
	/// 基于标准库的概念，但是使用帮助类型来便于后续定义中的新算数类型重载
	inline namespace concepts{
		/// 算术类型概念
		template<typename T>
		constexpr bool is_arithmetic_type=arithmetic_type_info_prover<remove_cvref<T>>::is_arithmetic_type;
		template<typename T>
		concept arithmetic_type=is_arithmetic_type<T>;

		/// 基础类型概念
		template<typename T>
		constexpr bool is_basic_type=arithmetic_type_info_prover<remove_cvref<T>>::is_basic_type;
		template<typename T>
		concept basic_type=is_basic_type<T>;

		/// 大数类型概念
		template<typename T>
		constexpr bool is_big_type=arithmetic_type_info_prover<remove_cvref<T>>::is_big_type;
		template<typename T>
		concept big_type=is_big_type<T>;

		/// 浮点类型概念
		template<typename T>
		constexpr bool is_float_type=arithmetic_type_info_prover<remove_cvref<T>>::is_float_type;
		template<typename T>
		concept float_type=is_float_type<T>;

		/// 整数类型概念
		template<typename T>
		constexpr bool is_integer_type=!is_float_type<T>;
		template<typename T>
		concept integer_type=is_integer_type<T>;

		/// 有符号类型概念
		template<typename T>
		constexpr bool is_signed=arithmetic_type_info_prover<remove_cvref<T>>::is_signed;
		template<typename T>
		concept signed_type=is_signed<T>;

		/// 无符号类型概念
		template<typename T>
		constexpr bool is_unsigned=!is_signed<T>;
		template<typename T>
		concept unsigned_type=is_unsigned<T>;

		/// 有符号整数类型概念
		template<typename T>
		constexpr bool is_signed_integer_type=is_integer_type<T>&&is_signed<T>;
		template<typename T>
		concept signed_integer_type=is_signed_integer_type<T>;

		/// 无符号整数类型概念
		template<typename T>
		constexpr bool is_unsigned_integer_type=is_integer_type<T>&&is_unsigned<T>;
		template<typename T>
		concept unsigned_integer_type=is_unsigned_integer_type<T>;

		/// 有NaN的类型概念
		template<typename T>
		concept has_NaN=arithmetic_type_info_prover<remove_cvref<T>>::has_NaN;

		/// 有inf的类型概念
		template<typename T>
		concept has_inf=arithmetic_type_info_prover<remove_cvref<T>>::has_inf;
	}

	//isNaN
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr bool isNaN(const T&v)noexcept{
		if constexpr(has_NaN<T>)
			return v!=v;
		else
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
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr T copy_as_negative(auto x,bool negative=1)noexcept{
		if constexpr(is_unsigned<T>)
			template_error("copy_as_negative:unsigned type");
		if constexpr(is_basic_type<decltype(x)>){
			if constexpr(is_float_type<decltype(x)>)
				return(T)::std::copysign(x,negative?-1:1);
			else
				return(T)negative?T{}-x:x;
		}
		else
			return copy_as_negative<T>((T)x,negative);
	}
	[[nodiscard]]force_inline constexpr auto copy_as_negative(auto x,bool negative=1)noexcept{
		return copy_as_negative<to_signed_t<decltype(x)>>(x,negative);
	}
	[[nodiscard]]force_inline constexpr auto copy_as_not_negative(auto x)noexcept{
		return copy_as_negative(x,false);
	}
	/*! 任意算数类型安全转型(ub避免.). */
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr T safe_arithmetic_cast(U x)noexcept{
		if constexpr(::std::is_floating_point_v<U> && ::std::is_unsigned_v<T>)
			return (T)(intmax_t)x;
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
		if constexpr(::std::is_floating_point_v<T1>||::std::is_floating_point_v<T2>)
			return ::std::fmod(a,b);
		else
			return a%b;
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
		if constexpr(::std::is_floating_point_v<T1>||::std::is_floating_point_v<T2>)
			return divmod_result{::std::floor(a/b),::std::fmod(a,b)};
		else
			return divmod_result{a/b,a%b};
	}

	/*! 设置浮点舍入 */
	template<typename T>
	force_inline constexpr void set_rounding(int mode){
		if constexpr(::std::is_floating_point_v<T>)
			::std::fesetround(mode);
	}
	/*!获取浮点舍入 */
	template<typename T>
	[[nodiscard]]force_inline constexpr int get_rounding(){
		if constexpr(::std::is_floating_point_v<T>)
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
		if constexpr(is_float_type<T> && is_basic_type<T>)
			return abs(a-b)<=::std::numeric_limits<T>::epsilon();
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
			typedef decltype(lambda{
				if constexpr(is_basic_type<T>)
					return ::std::exp(T{});
				else
					return T{};
			}()) RT;
			auto exp_impl = recursive_lambda(RT x,RT sum,RT n,size_t i,RT t)noexcept -> RT{
				const auto epsilon=sum+t/n;
				if(is_close(sum,epsilon))
					return sum;
				else
					return self_recursion(x,epsilon,n*i,i+1,t*x);
			};
			auto exp_impl_caller = get_recursive_lambda_caller(exp_impl);
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
			typedef decltype(lambda()noexcept{
				if constexpr(is_basic_type<T>)
					return ::std::log(T{});
				else
					return T{};
			}()) RT;
			auto log_impl = recursive_lambda(const RT x,const RT y)noexcept -> RT{
				auto log_iter = lambda(RT x,RT y)noexcept{
					const auto exp_y = exp(y);
					return y + T{2u}*(x-exp_y)/(x+exp_y);
				};
				return RT(is_close(y,log_iter(x,y)) ? y : self_recursion(x,log_iter(x,y)));
			};
			auto log_impl_caller = get_recursive_lambda_caller(log_impl);
			return log_impl_caller((RT)a,RT{0u});
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::log(a);
	}
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr auto log(const T&a,const U&b)noexcept{
		return log(a)/log(b);
	}
	//pow
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<arithmetic_type T,arithmetic_type U>
	[[nodiscard]]force_inline constexpr auto pow(const T&a,const U&b)noexcept{
		if(in_consteval||!is_basic_type<T>){
			typedef decltype(::std::pow(a,b)) RT;
			return RT(exp(b*log(a)));
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::pow(a,b);
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
			typedef decltype(::std::ceil(v)) RT;
			auto ceil_impl = lambda(T x, T y)noexcept{
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
			typedef decltype(::std::floor(v)) RT;
			auto floor_impl = lambda(T x, T y)noexcept{
				return is_close(x,y) ? y : y-T{1};
			};
			return v<0 ? -static_cast<T>(to_uintmax_t(-v+T{1})) : floor_impl(v,static_cast<T>(to_uintmax_t(v)));
		}
		else
			return ::std::floor(v);
	}
	//sqrt
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<arithmetic_type T>
	[[nodiscard]]force_inline constexpr auto sqrt(const T&v)noexcept{
		if(in_consteval||!is_basic_type<T>){
			typedef decltype(lambda()noexcept{
				if constexpr(is_basic_type<T>)
					return ::std::sqrt(T{});
				else
					return T{};
			}()) RT;
			auto sqrt_impl = recursive_lambda(const RT x,const RT curr,const RT prev)noexcept -> RT{
				return is_close(curr,prev) ? curr : self_recursion(x,(curr+x/curr)/RT{2u},curr);
			};
			auto sqrt_impl_caller = get_recursive_lambda_caller(sqrt_impl);
			if(v >= 0u && v < ::std::numeric_limits<RT>::infinity())
				return sqrt_impl_caller((RT)v,(RT)v,(RT)0u);
			else
				return ::std::numeric_limits<RT>::quiet_NaN();
		}
		elseif constexpr(is_basic_type<T>)
			return ::std::sqrt(v);
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
	push_and_disable_msvc_warning(26467);//gold_of_resize永远为正数
	/// 已知当前array的size，求下一个合适的提前分配大小
	[[nodiscard]]inline constexpr size_t get_next_gold_size_to_resize_for_array(size_t size)noexcept{
		/*
		每次扩容后的空间与原空间比大致为gold of resize可以最小化时空负担.
		*/
		return size_t(size*magic_number::gold_of_resize);
	}
	/// 已知当前hash table的size，求下一个合适的桶大小
	[[nodiscard]]inline constexpr size_t get_next_gold_size_to_resize_for_hash(size_t size)noexcept{
		/*
		素数大小的桶数可以使hash table中的每个桶尽可能活跃.
		每次扩容后的空间与原空间比大致为gold of resize可以最小化时空负担.
		*/
		return size_t(get_prime_num_big_or_eq_than(size*magic_number::gold_of_resize));
	}
	pop_msvc_warning();

	/// 线性插值.
	namespace linear_interpolation{
		[[nodiscard]]inline constexpr auto get_k(auto y1,auto y2,auto δx)noexcept{
			auto δy=sub(y2,y1);
			auto k=δy/δx;
			return k;
		}
		[[nodiscard]]inline constexpr auto get_k(auto y1,auto y2)noexcept{
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
				container_t aret;
				//思路：获取小于等于m的所有素数，依次除以m，若能整除则将素数加入结果，否则跳过，直到m为1.
				T i=T{2},m=abs(_m);
				while(m!=1){
					while(!(m%i)){
						aret.push_back(i);
						m/=i;
					}
					i=get_prime_num_big_than(i);
				}
				if constexpr(::std::is_signed_v<T> || ::std::is_floating_point_v<T>)
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
					container_t aret;
					//思路：获取小于等于m的所有素数，依次除以m，若能整除则将素数加入结果，否则跳过，直到m为1.
					T i=T{2},m=abs(_m);
					while(m!=1){
						if(!(m%i)){
							aret.push_back(i);
							do m/=i;while(!(m%i));
						}
						i=get_prime_num_big_than(i);
					}
					if constexpr(::std::is_signed_v<T> || ::std::is_floating_point_v<T>)
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
		template<class T> requires ::std::is_arithmetic_v<T>
		[[nodiscard]]constexpr force_inline auto operator()(T m)const noexcept{
			return result_t<T>{m};
		}
	}get_prime_factorization{};
	//编译时质因数分解到质因数表
	template<size_t number>
	struct prime_factorization_table_t{
		static constexpr size_t table_size=lambda{
			size_t i=2,m=number;
			size_t ret=0;
			while(m!=1){
				while(!(m%i)){
					++ret;
					m/=i;
				}
				i=get_prime_num_big_than(i);
			}
			return ret;
		}();
		size_t _table[table_size];
		constexpr force_inline prime_factorization_table_t()noexcept{
			size_t i=2,m=number;
			size_t index=0;
			while(m!=1){
				while(!(m%i)){
					_table[index++]=i;
					m/=i;
				}
				i=get_prime_num_big_than(i);
			}
		}
		[[nodiscard]]constexpr force_inline size_t operator[](size_t index)const noexcept{
			return _table[index];
		}
		//begin and end for range for
		[[nodiscard]]constexpr force_inline size_t* begin()noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline size_t* end()noexcept{
			return _table+table_size;
		}
		[[nodiscard]]constexpr force_inline const size_t* begin()const noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline const size_t* end()const noexcept{
			return _table+table_size;
		}
		//size
		[[nodiscard]]constexpr force_inline size_t size()const noexcept{
			return table_size;
		}
		//friend
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto size_of_array_like(const prime_factorization_table_t<number>&)noexcept{
			return table_size;
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto begin_of_array_like(prime_factorization_table_t<number>&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto end_of_array_like(prime_factorization_table_t<number>&table)noexcept{
			return table.end();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto begin_of_array_like(const prime_factorization_table_t<number>&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto end_of_array_like(const prime_factorization_table_t<number>&table)noexcept{
			return table.end();
		}
	};
	//编译时的唯一质因数分解到质因数表
	template<size_t number>
	struct unique_prime_factorization_table_t{
		static constexpr size_t table_size=lambda{
			size_t i=2,m=number;
			size_t ret=0;
			while(m!=1){
				if(!(m%i)){
					++ret;
					do m/=i;while(!(m%i));
				}
				i=get_prime_num_big_than(i);
			}
			return ret;
		}();
		size_t _table[table_size]={};
		constexpr force_inline unique_prime_factorization_table_t()noexcept{
			size_t i=2,m=number;
			size_t index=0;
			while(m!=1){
				if(!(m%i)){
					_table[index++]=i;
					do m/=i;while(!(m%i));
				}
				i=get_prime_num_big_than(i);
			}
		}
		[[nodiscard]]constexpr force_inline size_t operator[](size_t index)const noexcept{
			return _table[index];
		}
		//begin and end for range for
		[[nodiscard]]constexpr force_inline size_t* begin()noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline size_t* end()noexcept{
			return _table+table_size;
		}
		[[nodiscard]]constexpr force_inline const size_t* begin()const noexcept{
			return _table;
		}
		[[nodiscard]]constexpr force_inline const size_t* end()const noexcept{
			return _table+table_size;
		}
		//size
		[[nodiscard]]constexpr force_inline size_t size()const noexcept{
			return table_size;
		}
		//friend
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto size_of_array_like(const unique_prime_factorization_table_t<number>&)noexcept{
			return table_size;
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto begin_of_array_like(unique_prime_factorization_table_t<number>&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto end_of_array_like(unique_prime_factorization_table_t<number>&table)noexcept{
			return table.end();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto begin_of_array_like(const unique_prime_factorization_table_t<number>&table)noexcept{
			return table.begin();
		}
		template<class T> requires(type_info<remove_cv<T>> == type_info<size_t>)
		[[nodiscard]]friend inline auto end_of_array_like(const unique_prime_factorization_table_t<number>&table)noexcept{
			return table.end();
		}
	};
	//求出最大公约数
	template<integer_type T>
	[[nodiscard]]inline auto gcd(T x, T y)noexcept{
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
		return x << shift;
	}
}
using namespace math::concepts;
using math::to_unsigned_t;
using math::is_negative;
using math::copy_as_negative;
using math::copy_as_not_negative;
using math::to_size_t;
using math::mod;
using math::divmod;
using math::set_rounding;
using math::get_rounding;
using math::rounding_auto_setter;
using math::is_close;
using math::isNaN;
using math::abs;
using math::exp;
using math::log;
using math::pow;
using math::ceil;
using math::floor;
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
using math::gcd;

//file_end

