//binary_node_t.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct binary_node_t:node_like,instance_struct<binary_node_t<T>>{
	typedef binary_node_t<T> this_t;

	T _m;

	binary_node_t(const T&a)noexcept:_m(a){}
	binary_node_t(T&&a)noexcept:_m(a){}

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<this_t>;}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const override{
		if(a->get_type_info() != this->get_type_info())
			return false;
		const this_t*p=static_cast<const this_t*>(a.get());
		return _m==p->_m;
	}
public:
	[[nodiscard]]virtual value arec(const value index)override{return as_value(this);}
};
template<typename T>
bool was_an(const_ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	if(p->get_type_info() == type_info<target_node_t>)
		return true;
	else
		return false;
}
template<typename T>
T& use_as(ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	return static_cast<target_node_t*>(p.get())->_m;
}
template<typename T>
const T& use_as(const_ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	if(was_an<T>(p))
		return static_cast<const target_node_t*>(p.get())->_m;
	else
		return const_default_value_of<T>;
}
template<typename T>
maybe_fail_reference<T> maybe_fail_use_as(ptr p)noexcept{
	if(was_an<T>(p))
		return use_as<T>(p);
	else
		return note::fail;
}
template<typename T>
ptr make_binary_node_from(T a)noexcept{
	return get<binary_node_t<T>>(move(a));
}

//file_end

