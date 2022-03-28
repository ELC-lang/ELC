//data.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
	每个T的可能性大小
*/
template<class T> requires ::std::is_integral_v<T>
constexpr size_t number_of_possible_values_per=max(type_info<::std::make_unsigned_t<T>>)+1;

/*
	功能: byte* 类型数据转换为 T&，不进行任何检查
	用法: data_cast<T>(byte*) -> T&
*/
template<class T>
[[nodiscard]]constexpr T&data_cast(byte*p){return*::std::launder(reinterpret_cast<T*>(p));}
/*
	功能: T* 指针转换为 byte*，不进行任何检查
	用法: cast_to_data(T*) -> byte*
*/
template<class T>
[[nodiscard]]constexpr byte*cast_to_data(T*p){return ::std::launder(reinterpret_cast<byte*>(p));}
/*
	功能: const T* 指针转换为 const byte*，不进行任何检查
	用法: cast_to_data(const T*) -> const byte*
*/
template<class T>
[[nodiscard]]constexpr const byte*cast_to_data(const T*p){return ::std::launder(reinterpret_cast<const byte*>(p));}

/*
	功能: data_block类模板,接受多个类型参数,实例化为内含最大体积最大对齐要求的byte数组的结构体
			data_block -> byte*
	用法: data_block<T1,T2,...>value;
*/
template<class...Ts>
struct data_block:non_copyable,non_moveable{
	alignas(max({alignof(Ts)...}))
	byte _data[max({sizeof(Ts)...})];
	constexpr operator byte*(){return _data;}
};

//file_end

