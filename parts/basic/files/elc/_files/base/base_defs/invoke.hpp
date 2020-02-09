//invoke.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
constexpr struct invoke_t{
	template<class...Args>
	auto _as(Args&&...rest)noexcept_as(MAGIC){//invoke<T>._as
		return[](T a)noexcept(::std::is_nothrow_invocable<T,Args...>){return a(forward(rest)...);};
	}
}invoke{};
