//data.hpp
//at namespace elc::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
using ::std::byte;
template<class T>
[[nodiscard]]constexpr T&data_cast(byte*p){return*::std::launder(reinterpret_cast<T*>(p));}

#if !defined(_MSC_VER)//for syntax check
//https://developercommunity.visualstudio.com/content/problem/683248/a-bug-of-parameter-pack.html
template<class...Ts>
struct data_block:non_copyable,non_moveable{
	alignas(max({alignof(Ts)...}))
	byte _data[max({sizeof(Ts)...})];
	constexpr operator byte*(){return _data;}
};
#endif
