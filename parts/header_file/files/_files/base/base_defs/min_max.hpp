//min_max.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
constexpr struct min_t{
	template<class...Args>
	static constexpr bool able=was_not_an_ill_form(::std::min(declvalue(Args)...));
	
	template<typename T>
	static constexpr bool get_limit_able=was_not_an_ill_form(::std::numeric_limits<T>::min());
	
	template<class...Args> requires able<Args...>
	constexpr auto operator()(Args&&...rest)const{
		return ::std::min(rest...);
	}
	template<typename T> requires get_limit_able<T>
	constexpr auto operator()(type_info_t<T>)const{
		return ::std::numeric_limits<T>::min();
	}
}min{};
constexpr struct max_t{
	template<class...Args>
	static constexpr bool able=was_not_an_ill_form(::std::max(declvalue(Args)...));
	
	template<typename T>
	static constexpr bool get_limit_able=was_not_an_ill_form(::std::numeric_limits<T>::max());
	
	template<class...Args> requires able<Args...>
	constexpr auto operator()(Args&&...rest)const{
		return ::std::max(rest...);
	}
	template<typename T> requires get_limit_able<T>
	constexpr auto operator()(type_info_t<T>)const{
		return ::std::numeric_limits<T>::max();
	}
}max{};

//file_end

