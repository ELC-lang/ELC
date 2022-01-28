//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
[[nodiscard]]function_t<setter()> node_like::get_eval_of_this(){
	return lambda_with_catch(this)()noexcept{return setter(this);};
}
[[nodiscard]]function_t<setter(ptr)> node_like::get_call_of_this(){
	return lambda_with_catch(this)(ptr)noexcept{return setter(this);};
}

[[nodiscard]]setter node_like::operator[](const setter index){
	return arec(index);
}
[[nodiscard]]setter node_like::operator[](ptr index){
	return arec(setter(index));
}

//file_end

