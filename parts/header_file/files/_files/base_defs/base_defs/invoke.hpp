//invoke.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct invoke_t{
	template<class...Args>
	static constexpr bool able= ::std::is_invocable_v<T,Args...>;
	template<class...Args>
	static constexpr bool nothrow= ::std::is_nothrow_invocable_v<T,Args...>;

	template<class...Args> requires able<Args...>
	auto _as(Args&&...rest)const noexcept(nothrow<Args...>){//invoke<T>._as(...)
		return lambda_with_catch(rest...)(const T&a)noexcept(nothrow<Args...>){return a(forward<Args>(rest)...);};
	}
};
template<typename T>
constexpr invoke_t<T>invoke{};

//file_end

