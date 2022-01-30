//common_node.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct cons final:node_like,instance_struct<cons>{
	typedef cons this_t;

	value _car;
	value _cdr;

	cons(ptr car,ptr cdr){
		_car = car;
		_cdr = cdr;
	}

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<cons>;}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const override{
		if(a->get_type_info() != this->get_type_info())
			return false;
		const this_t*p=static_cast<const this_t*>(a.get());
		return _car==p->_car&&_cdr==p->_cdr;
	}
public:
	[[nodiscard]]virtual value arec(const value index)override{
		const_ptr v=index;
		if(v==&nil)
			return _cdr;
		elseif(v==&t)
			return _car;
		else{
			ptr to=get<common_node>();
			_car >> (*to)[t];
			_cdr >> (*to)[nil];
			be_replace_as(to);
			return (*to)[v];
		}
	}

	virtual void clear()override{the_void[t]>>_car>>_cdr;}
};

[[nodiscard]]inline ptr make_list()noexcept{return &nil;}
template<typename T,typename...Args>
[[nodiscard]]inline ptr make_list(T&&first,Args&&...rest){
	return get<cons>(as_ptr(first),make_list(forward<Args>(rest)...));
}

//file_end

