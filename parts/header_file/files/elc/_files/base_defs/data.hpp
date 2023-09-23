//data.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
/*!
	每个T的可能性大小
*/
template<basic_integer_type T>
constexpr auto number_of_possible_values_per=uintmax_index_t(max(type_info<::std::make_unsigned_t<T>>))+1;

/*!
	功能: byte* 类型数据转换为 T&，不进行任何检查
	用法: data_cast<T>(byte*) -> T&
*/
template<class T>
[[nodiscard]]force_inline constexpr T&data_cast(byte*p)noexcept{return*reinterpret_cast<T*>(p);}
template<class T>
[[nodiscard]]force_inline constexpr T&launder_data_cast(byte*p)noexcept{return*::std::launder(data_cast<T>(p));}
/*!
	功能: byte* 类型数据转换为 T*，不进行任何检查
	用法: data_ptr_cast<T>(byte*) -> T*
*/
template<class T>
[[nodiscard]]force_inline constexpr T*data_ptr_cast(byte*p)noexcept{return reinterpret_cast<T*>(p);}
template<class T>
[[nodiscard]]force_inline constexpr T*launder_data_ptr_cast(byte*p)noexcept{return ::std::launder(data_ptr_cast<T>(p));}
/*!
	功能: T* 指针转换为 byte*，不进行任何检查
	用法: cast_to_data(T*) -> byte*
*/
template<class T>
[[nodiscard]]force_inline constexpr byte*cast_to_data(T*p)noexcept{return reinterpret_cast<byte*>(p);}
template<class T>
[[nodiscard]]force_inline constexpr byte*launder_cast_to_data(T*p)noexcept{return ::std::launder(cast_to_data(p));}
/*!
	功能: const T* 指针转换为 const byte*，不进行任何检查
	用法: cast_to_data(const T*) -> const byte*
*/
template<class T>
[[nodiscard]]force_inline constexpr const byte*cast_to_data(const T*p)noexcept{return reinterpret_cast<const byte*>(p);}
template<class T>
[[nodiscard]]force_inline constexpr const byte*launder_cast_to_data(const T*p)noexcept{return ::std::launder(cast_to_data(p));}
/*!
	功能: T& 转换为 U&，不进行任何检查
	用法: union_cast<U>(T&) -> U&
*/
template<class U,class T>
[[nodiscard]]force_inline constexpr U&union_cast(T&&t)noexcept{return*reinterpret_cast<U*>(&t);}
template<class U,class T>
[[nodiscard]]force_inline constexpr U&launder_union_cast(T&&t)noexcept{return*::std::launder(union_cast<U>(t));}

/*!
	功能: data_block辅助变量模板，接受数个类型并表现为其中的最大对齐值
*/
template<class...Ts>
inline constexpr size_t max_align_of=max({alignof(Ts)...});
/*!
	功能: data_block辅助变量模板，接受数个类型并表现为其中的最大大小值
*/
template<class...Ts>
inline constexpr size_t max_size_of=max({sizeof(Ts)...});
//byteset
template<size_t byte_num,size_t align=alignof(byte)>
struct byteset:non_copy_construct_able,non_move_construct_able{
	push_and_disable_msvc_warning(4324);
	alignas(align)byte _data[byte_num];
	pop_msvc_warning();

	push_and_disable_msvc_warning(26495);
	force_inline constexpr byteset()noexcept{
		if in_consteval
			for(size_t i=0;i<byte_num;++i)
				_data[i]=byte{};
	};
	template<class T> requires(sizeof(T)<=byte_num&&alignof(T)<=align)
	force_inline constexpr byteset(T&&t)noexcept{
		data_cast<T>(_data)=forward<T>(t);
		if constexpr(sizeof(T)<byte_num)
			if in_consteval
				for(size_t i=sizeof(T);i<byte_num;++i)
					_data[i]=byte{};
	}
	pop_msvc_warning();
	force_inline constexpr operator byte*()noexcept{return _data;}
	force_inline constexpr operator const byte*()const noexcept{return _data;}
	template<class T> requires(sizeof(T)<=byte_num&&alignof(T)<=align)
	force_inline constexpr auto&operator=(T&&t)noexcept{
		return data_cast<remove_cvref<T>>(_data)=forward<T>(t);
	}
	//begin & end
	force_inline constexpr byte*begin()noexcept{return _data;}
	force_inline constexpr byte*end()noexcept{return _data+byte_num;}
	force_inline constexpr const byte*begin()const noexcept{return _data;}
	force_inline constexpr const byte*end()const noexcept{return _data+byte_num;}
};
/*!
	功能: data_block类模板,接受多个类型参数,实例化为内含最大体积最大对齐要求的byte数组的结构体
			data_block -> byte*
	用法: data_block<T1,T2,...>value;
*/
template<class...Ts>
struct data_block:byteset<max_size_of<Ts...>,max_align_of<Ts...>>{
	static constexpr size_t size=max_size_of<Ts...>;
	static constexpr size_t align=max_align_of<Ts...>;
};
/*!
	功能: data_block辅助类模板，接受一个size_t描述对齐大小的类型模板
*/
push_and_disable_msvc_warning(4324);
template<class...Ts>
struct alignas(max_align_of<Ts...>)align_as_t{};
pop_msvc_warning();

/*!
	功能: data_view类模板,接受一个类型参数，提供对byte数组的访问和遍历
	用法: data_view<T>value{&value};
*/
template<class T>
struct data_view:array_like_view_t<byte>{
	using array_like_view_t<byte>::array_like_view_t;
	constexpr data_view(T*p)noexcept:array_like_view_t<byte>{cast_to_data(p),sizeof(T)}{}
};
template<class T>
struct data_view<const T>:array_like_view_t<const byte>{
	using array_like_view_t<const byte>::array_like_view_t;
	constexpr data_view(const T*p)noexcept:array_like_view_t<const byte>{cast_to_data(p),sizeof(T)}{}
};

/*!
	功能: 比较两个类实例在内存中的内容是否相同
	用法: full_equal_in_byte(const T&a,const T&b) -> bool
*/
template<class T>
[[nodiscard]]constexpr bool full_equal_in_byte(const T&a,const T&b)noexcept{
	return equal(cast_to_data(&a),cast_to_data(&b),sizeof(T));
}

//file_end

