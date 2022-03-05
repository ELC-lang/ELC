//function_node.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct function_node:common_node,instance_struct<function_node>{
	typedef function_node this_t;
	override_instance_struct;

	function_node(function_t<value(ptr)> func)noexcept:_func(func){}

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<this_t>;}
	[[nodiscard]]virtual value be_call(ptr arg_list)override{return _func(arg_list);}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const noexcept override{
		
	}
};

//file_end

