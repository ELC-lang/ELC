//data.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

/*
	功能: byte* 类型数据转换为 T&
	用法: data_cast<T>(byte*) -> T&
*/
template<class T>
[[nodiscard]]constexpr T&data_cast(byte*p){return*::std::launder(reinterpret_cast<T*>(p));}
/*
	功能: data_block结构体,传入多种类型Ts,转化为最大的结构体数据块
			data_block -> byte*
	用法: data_block<T1,T2,...>
*/
template<class...Ts>
struct data_block:non_copyable,non_moveable{
	alignas(max({alignof(Ts)...}))
	byte _data[max({sizeof(Ts)...})];
	constexpr operator byte*(){return _data;}
};

//file_end

