//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
[[nodiscard]]function_t<value()> node_like::get_eval_of_this(){
	return lambda_with_catch(this)()noexcept{return value(this);};
}
[[nodiscard]]function_t<value(ptr)> node_like::get_call_of_this(){
	return lambda_with_catch(this)(ptr)noexcept{return value(this);};
}

[[nodiscard]]value node_like::operator[](const value index){
	return arec(index);
}

//file_end

