//glum_binary_function_node.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct glum_binary_function_node:node_like,instance_struct<glum_binary_function_node>{
	typedef glum_binary_function_node this_t;

	function_t<value(ptr)> _func;

	glum_binary_function_node(function_t<value(ptr)> func)noexcept:_func(func){}
	glum_binary_function_node(function_t<void(ptr)> func)noexcept{
		_func = lambda_with_catch(func)(ptr p) {
			func(p);
			return as_value(the_void);
		};
	}
	glum_binary_function_node(function_t<void()> func)noexcept{
		_func = lambda_with_catch(func)(ptr) {
			func();
			return as_value(the_void);
		};
	}

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<this_t>;}
	[[nodiscard]]virtual function_t<value(ptr)> get_call_of_this()noexcept override{return _func;}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const override{
		if(a->get_type_info() != this->get_type_info())
			return false;
		const this_t*p=static_cast<const this_t*>(a.get());
		return _func==p->_func;
	}
public:
	[[nodiscard]]virtual value arec(const value index)override{return the_void[index];}

	virtual void clear()override{_func=nullptr;}
};

//file_end

