//attribute.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T,typename attribute_name>
struct attribute{
protected:
	friend T*get_handle(attribute<T,attribute_name>*a);
	constexpr T*get_handle()noexcept{return static_cast<T*>(static_cast<attribute_name*>(this));}
	constexpr const T*get_handle()const noexcept{return static_cast<const T*>(static_cast<const attribute_name*>(this));}
};
template<class T,typename attribute_name>
T*get_handle(attribute<T,attribute_name>*a){return get_handle(a->get_handle());}

template<special_attribute_t attribute_name,class T>
auto attribute_ptr_cast(T*ptr)noexcept{
	return static_cast<attribute_name*>(ptr);
}
template<common_attribute_t attribute_name,class T>
auto attribute_ptr_cast(T*ptr)noexcept{
	return attribute_ptr_cast<attribute_name<T>>(ptr);
}

template<special_attribute_t attribute_name,class T>
auto attribute_cast(T&t)noexcept{
	return*attribute_ptr_cast<attribute_name>(&t);
}
template<common_attribute_t attribute_name,class T>
auto attribute_cast(T&t)noexcept{
	return*attribute_ptr_cast<attribute_name>(&t);
}

template<special_attribute_t... attribute_names>
struct special_attribute_pack:virtual attribute_names...{
	/*兼容用*/
	template<class T>
	struct on_type:special_attribute_pack<attribute_names>...{};
};
template<common_attribute_t... attribute_names>
struct common_attribute_pack{
	template<class T>
	struct on_type:attribute_names<T>...{};
};

//file_end

