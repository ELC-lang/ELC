//common_node.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct common_node final:node_like,instance_struct<common_node>{
	typedef common_node this_t;

	map_t<value,const_weak_ptr> _m;

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<this_t>;}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const override{
		if(a->get_type_info() != this->get_type_info())
			return false;
		const this_t*p=static_cast<const this_t*>(a.get());
		return _m==p->_m;
	}
public:
	[[nodiscard]]virtual value arec(const value index)override{return _m[&index];}

	virtual void clear()override{_m.clear();}
};

//file_end

