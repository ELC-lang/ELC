//min_max.hpp
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
/*!
获取几个参数中的最大值或某类型能表示的最大值.
用法:
	max.able<T1,T2...> -> bool T...等类型能否获得参数最大值.
	max.get_limit_able<T> -> bool 能否获得T类型能表示的最大值.
	max(v1,v2...) -> auto 获取v...的最大值.
	max(type_info<T>) -> auto 获取T能表示的最大值.
*/
constexpr struct max_t{
private:
	template<class A>
	static constexpr A type_helper()noexcept{
		return declvalue(A);
	}
	template<class A,class B>
	static constexpr auto type_helper()noexcept{
		return 1?declvalue(A):declvalue(B);
	}
	template<class A,class B,class C,class...Args>
	static constexpr auto type_helper()noexcept{
		return type_helper<A,decltype(type_helper<B,C,Args...>())>();
	}
public:
	template<class...Args>
	using type=decltype(type_helper<Args...>());
private:
	template<class A>
	static constexpr bool able_helper()noexcept{
		return true;
	}
	template<class A,class B>
	static constexpr bool able_helper()noexcept{
		return compare.able<A,B>;
	}
	template<class A,class B,class C,class...Args>
	static constexpr bool able_helper()noexcept{
		if constexpr(!able_helper<B,C,Args...>())
			return false;
		else
			return able_helper<A,type<B,C,Args...>>();
	}
public:
	template<class...Args>
	static constexpr bool able=able_helper<Args...>();
private:
	template<class A>
	static constexpr bool nothrow_helper()noexcept{
		return true;
	}
	template<class A,class B>
	static constexpr bool nothrow_helper()noexcept{
		return compare.nothrow<A,B>;
	}
	template<class A,class B,class C,class...Args>
	static constexpr bool nothrow_helper()noexcept{
		if constexpr(!nothrow_helper<B,C,Args...>())
			return false;
		else
			return nothrow_helper<A,type<B,C,Args...>>();
	}
public:
	template<class...Args>
	static constexpr bool nothrow=nothrow_helper<Args...>();

	template<typename T>
	static constexpr bool get_limit_able=math::arithmetic_type_info_prover<T>::has_max;
private:
	template<class A,class B> requires able<A,B>
	[[nodiscard]]static constexpr auto base_call(A&&a,B&&b)noexcept{
		const auto v=compare(a,b);
		return v<0?b:a;
	}
	template<class A,class B,class C,class...Args> requires able<A,B,Args...>
	[[nodiscard]]static constexpr auto base_call(A&&a,B&&b,C&&c,Args&&...rest)noexcept{
		return base_call(forward<A>(a),base_call(forward<B>(b),forward<C>(c),forward<Args>(rest)...));
	}
public:
	template<class...Args> requires able<Args...>
	[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
		return base_call(forward<Args>(rest)...);
	}
	template<typename T>
	struct for_type_t{
		[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept requires able<const T,const T>{
			auto maxlest=l.begin();
			for(const auto&i:l){
				const auto v=compare(*maxlest,i);
				if(v<0)
					maxlest=&i;
			}
			return *maxlest;
		}
		template<class...Args>
		[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
			return base_call(T(forward<Args>(rest))...);
		}
	};
	template<typename T>
	static constexpr for_type_t<T>for_type{};

	template<class T> requires able<const T,const T>
	[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept{
		return for_type<T>(l);
	}
	template<typename T> requires get_limit_able<T>
	[[nodiscard]]constexpr auto operator()(const type_info_t<T>&)const noexcept{
		return math::arithmetic_type_info_prover<T>::max();
	}
}max{};
/*!
获取几个参数中的最小值或某类型能表示的最小值.
用法:
	min.able<T1,T2...> -> bool T...等类型能否获得参数最小值.
	min.get_limit_able<T> -> bool 能否获得T类型能表示的最小值.
	min(v1,v2...) -> auto 获取v...的最小值.
	min(type_info<T>) -> auto 获取T能表示的最小值.
*/
constexpr struct min_t{
private:
	template<class A>
	static constexpr A type_helper()noexcept{
		return declvalue(A);
	}
	template<class A,class B>
	static constexpr auto type_helper()noexcept{
		return 1?declvalue(A):declvalue(B);
	}
	template<class A,class B,class C,class...Args>
	static constexpr auto type_helper()noexcept{
		return type_helper<A,decltype(type_helper<B,C,Args...>())>();
	}
public:
	template<class...Args>
	using type=decltype(type_helper<Args...>());
private:
	template<class A>
	static constexpr bool able_helper()noexcept{
		return true;
	}
	template<class A,class B>
	static constexpr bool able_helper()noexcept{
		return compare.able<A,B>;
	}
	template<class A,class B,class C,class...Args>
	static constexpr bool able_helper()noexcept{
		if constexpr(!able_helper<B,C,Args...>())
			return false;
		else
			return able_helper<A,type<B,C,Args...>>();
	}
public:
	template<class...Args>
	static constexpr bool able=able_helper<Args...>();
private:
	template<class A>
	static constexpr bool nothrow_helper()noexcept{
		return true;
	}
	template<class A,class B>
	static constexpr bool nothrow_helper()noexcept{
		return compare.nothrow<A,B>;
	}
	template<class A,class B,class C,class...Args>
	static constexpr bool nothrow_helper()noexcept{
		if constexpr(!nothrow_helper<B,C,Args...>())
			return false;
		else
			return nothrow_helper<A,type<B,C,Args...>>();
	}
public:
	template<class...Args>
	static constexpr bool nothrow=nothrow_helper<Args...>();

	template<typename T>
	static constexpr bool get_limit_able=math::arithmetic_type_info_prover<T>::has_min;
private:
	template<class A,class B> requires able<A,B>
	[[nodiscard]]static constexpr auto base_call(A&&a,B&&b)noexcept{
		const auto v=compare(a,b);
		return v<0?a:b;
	}
	template<class A,class B,class C,class...Args> requires able<A,B,Args...>
	[[nodiscard]]static constexpr auto base_call(A&&a,B&&b,C&&c,Args&&...rest)noexcept{
		return base_call(forward<A>(a),base_call(forward<B>(b),forward<C>(c),forward<Args>(rest)...));
	}
public:
	template<class...Args> requires able<Args...>
	[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
		return base_call(forward<Args>(rest)...);
	}
	template<typename T>
	struct for_type_t{
		[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept requires able<const T,const T>{
			auto minlest=l.begin();
			for(const auto&i:l){
				const auto v=compare(*minlest,i);
				if(v>0)
					minlest=&i;
			}
			return *minlest;
		}
		template<class...Args>
		[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
			return base_call(T(forward<Args>(rest))...);
		}
	};
	template<typename T>
	static constexpr for_type_t<T>for_type{};

	template<class T> requires able<const T,const T>
	[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept{
		return for_type<T>(l);
	}
	template<typename T> requires get_limit_able<T>
	[[nodiscard]]constexpr auto operator()(const type_info_t<T>&)const noexcept{
		return math::arithmetic_type_info_prover<T>::min();
	}
}min{};

//file_end

