//data.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*!
	每个T的可能性大小
*/
template<class T> requires ::std::is_integral_v<T>
constexpr size_t number_of_possible_values_per=max(type_info<::std::make_unsigned_t<T>>)+size_t{1};
/*!
	每个T有多少位
*/
template<class T>
constexpr size_t bitnumof=sizeof(T)*CHAR_BIT;

/*!
	功能: byte* 类型数据转换为 T&，不进行任何检查
	用法: data_cast<T>(byte*) -> T&
*/
template<class T>
[[nodiscard]]constexpr T&data_cast(byte*p){return*::std::launder(reinterpret_cast<T*>(p));}
/*!
	功能: T* 指针转换为 byte*，不进行任何检查
	用法: cast_to_data(T*) -> byte*
*/
template<class T>
[[nodiscard]]constexpr byte*cast_to_data(T*p){return ::std::launder(reinterpret_cast<byte*>(p));}
/*!
	功能: const T* 指针转换为 const byte*，不进行任何检查
	用法: cast_to_data(const T*) -> const byte*
*/
template<class T>
[[nodiscard]]constexpr const byte*cast_to_data(const T*p){return ::std::launder(reinterpret_cast<const byte*>(p));}

/*!
	功能: data_block类模板,接受多个类型参数,实例化为内含最大体积最大对齐要求的byte数组的结构体
			data_block -> byte*
	用法: data_block<T1,T2,...>value;
*/
template<class...Ts>
struct data_block:non_copyable,non_moveable{
	push_and_disable_msvc_warning(4324);
	alignas(max({alignof(Ts)...}))
	byte _data[max({sizeof(Ts)...})];
	constexpr operator byte*(){return _data;}
	pop_msvc_warning();
};

/*!
	功能: data_view类模板,接受一个类型参数，提供对byte数组的访问和遍历
	用法: data_view<T>value{&value};
*/
template<class T>
struct data_view:array_like_view_t<byte>{
	using array_like_view_t<byte>::array_like_view_t;
	constexpr data_view(T*p):array_like_view_t<byte>{cast_to_data(p),sizeof(T)}{}
};
template<class T>
struct data_view<const T>:array_like_view_t<const byte>{
	using array_like_view_t<const byte>::array_like_view_t;
	constexpr data_view(const T*p):array_like_view_t<const byte>{cast_to_data(p),sizeof(T)}{}
};

/*!
	功能: 比较两个类实例在内存中的内容是否相同
	用法: full_equal_in_byte(const T&a,const T&b) -> bool
*/
template<class T>
[[nodiscard]]constexpr bool full_equal_in_byte(const T&a,const T&b){
	return equal(cast_to_data(&a),cast_to_data(&b),sizeof(T));
}

//file_end

