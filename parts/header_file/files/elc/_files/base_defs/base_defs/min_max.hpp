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
获取几个参数中的最小值或某类型能表示的最小值.
用法:
	min.able<T1,T2...> -> bool T...等类型能否获得参数最小值.
	min.get_limit_able<T> -> bool 能否获得T类型能表示的最小值.
	min(v1,v2...) -> auto 获取v...的最小值.
	min(type_info<T>) -> auto 获取T能表示的最小值.
*/
constexpr struct min_t{
	template<class...Args>
	static constexpr bool able=was_not_an_ill_form(::std::min(declvalue(Args)...));

	template<typename T>
	static constexpr bool get_limit_able=was_not_an_ill_form(::std::numeric_limits<T>::min());

	template<class...Args> requires able<Args...>
	[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
		return ::std::min(forward<Args>(rest)...);
	}
	template<class T,class...Args> requires able<::std::initializer_list<T>>
	[[nodiscard]]constexpr auto operator()(T a,Args&&...rest)const noexcept{
		return ::std::min(a,T(rest)...);
	}
	template<typename T>
	struct for_type_t{
		[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept requires able<::std::initializer_list<T>>{
			return ::std::min(l);
		}
		template<class...Args>
		[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
			return ::std::min(T(rest)...);
		}
	};
	template<typename T>
	static constexpr for_type_t<T>for_type{};

	template<class T> requires able<::std::initializer_list<T>>
	[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept{
		return ::std::min(l);
	}
	template<typename T> requires get_limit_able<T>
	[[nodiscard]]constexpr auto operator()(type_info_t<T>)const noexcept{
		return ::std::numeric_limits<T>::min();
	}
}min{};
/*!
获取几个参数中的最大值或某类型能表示的最大值.
用法:
	max.able<T1,T2...> -> bool T...等类型能否获得参数最大值.
	max.get_limit_able<T> -> bool 能否获得T类型能表示的最大值.
	max(v1,v2...) -> auto 获取v...的最大值.
	max(type_info<T>) -> auto 获取T能表示的最大值.
*/
constexpr struct max_t{
	template<class...Args>
	static constexpr bool able=was_not_an_ill_form(::std::max(declvalue(Args)...));

	template<typename T>
	static constexpr bool get_limit_able=was_not_an_ill_form(::std::numeric_limits<T>::max());

	template<class...Args> requires able<Args...>
	[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
		return ::std::max(forward<Args>(rest)...);
	}
	template<class T,class...Args> requires able<::std::initializer_list<T>>
	[[nodiscard]]constexpr auto operator()(T a,Args&&...rest)const noexcept{
		return ::std::max(a,T(rest)...);
	}
	template<typename T>
	struct for_type_t{
		[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept requires able<::std::initializer_list<T>>{
			return ::std::max(l);
		}
		template<class...Args>
		[[nodiscard]]constexpr auto operator()(Args&&...rest)const noexcept{
			return ::std::max(T(rest)...);
		}
	};
	template<typename T>
	static constexpr for_type_t<T>for_type{};

	template<class T> requires able<::std::initializer_list<T>>
	[[nodiscard]]constexpr auto operator()(::std::initializer_list<T>l)const noexcept{
		return ::std::max(l);
	}
	template<typename T> requires get_limit_able<T>
	[[nodiscard]]constexpr auto operator()(type_info_t<T>)const noexcept{
		return ::std::numeric_limits<T>::max();
	}
}max{};

//file_end

