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

	binary_node_t(T a)noexcept:_m(a){}
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
	[[nodiscard]]virtual value arec(const value index)override{return the_void[index];}
};

//file_end

