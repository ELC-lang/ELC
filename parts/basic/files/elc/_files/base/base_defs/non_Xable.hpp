//non_Xable.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct non_copyable{
	constexpr non_copyable()=default;
	constexpr non_copyable(non_copyable&)=delete;
	constexpr non_copyable(non_copyable&&)=default;
	constexpr non_copyable& operator=(non_copyable&)&noexcept=delete;
	constexpr non_copyable& operator=(non_copyable&&)&noexcept=default;
};
struct non_moveable{
	constexpr non_moveable()=default;
	constexpr non_moveable(non_moveable&)=default;
	constexpr non_moveable(non_moveable&&)=delete;
	constexpr non_moveable& operator=(non_moveable&)&noexcept=default;
	constexpr non_moveable& operator=(non_moveable&&)&noexcept=delete;
};
