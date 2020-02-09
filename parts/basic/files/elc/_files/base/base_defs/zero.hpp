//zero.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
constexpr struct zero_t{
	template<class T,enable_if(type_arg<decltype(0)>.can_convert_to<T>)>
	operator T()const noexcept(type_arg<decltype(0)>.can_nothrow_convert_to<T>){return 0;}
}zero{};
