//min_max.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
constexpr struct min_t{
	template<class...Args,enable_if_not_ill_form(::std::min(declvalue(Args)...))>
	constexpr auto operator()(Args&&...rest)const{
		return ::std::min(rest...);
	}
	template<typename T,enable_if_not_ill_form(::std::numeric_limits<T>::min())>
	constexpr auto operator()(type_info_t<T>)const{
		return ::std::numeric_limits<T>::min();
	}
}min{};
constexpr struct max_t{
	template<class...Args,enable_if_not_ill_form(::std::max(declvalue(Args)...))>
	constexpr auto operator()(Args&&...rest)const{
		return ::std::max(rest...);
	}
	template<typename T,enable_if_not_ill_form(::std::numeric_limits<T>::max())>
	constexpr auto operator()(type_info_t<T>)const{
		return ::std::numeric_limits<T>::max();
	}
}max{};

//file_end

