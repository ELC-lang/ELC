//defs.hpp
//at namespace elc::container::stack_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
class base_stack_t:non_copyable{//基础容器，可用于hashtable
	typedef base_stack_t<T>this_t;
protected:
	struct data_t{
		T _data;
		data_t*_next;
	}*_m;
public:
	constexpr base_stack_t():_m(null_ptr){}
	this_t&operator=(this_t&&a)noexcept{
		using::std::swap;
		swap(_m,a._m);
		return*this;
	}
	base_stack_t(this_t&&a):base_stack_t(){
		operator=(a);
	}
	~base_stack_t(){
		data_t*tmp;
		while(_m!=null_ptr){
			tmp=_m;
			_m=_m->_next;
			unget(tmp);
		}
	}
	template<typename T_>
	maybe_fail_reference<T>get(T_&&a){
		data_t*tmp=_m;
		while(tmp!=null_ptr){
			if(a==tmp->_data)
				return tmp->_data;
			tmp=tmp->_next;
		}
		return note::fail;
	}
	bool in_stack(const T&a){
		return get(a).not_fail();
	}
	void add(const T&a){
		_m=get<data_t>({a,_m});
	}
};
