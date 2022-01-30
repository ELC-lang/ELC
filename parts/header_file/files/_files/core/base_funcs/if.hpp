//if.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF: 草稿代码
value func_if(ptr){
	auto tmp=logical_bool(eval(get_arg(1)));
	multi_node aret;
	if(tmp)
		aret.add(eval(get_arg(2)));
	if(!tmp)
		aret.add(eval(get_arg(3)));
	return aret;
}
value func_while(ptr){
	auto tmp=get_arg(1);
	multi_node aret;
	while(eval(tmp))
		aret.add(eval(get_arg_list(2)));
	return aret;
}
value is_void(ptr){
	logical_bool tmp=false;
	size_t index=0;
	while(index++ < arg_size())
		tmp &= (&the_void==get_arg(index));
	return aret;
}
value func_throw(prt){
	try{
		multi_node tmp;
		while(index++ < arg_size())
			tmp.add(eval(get_arg(index)));
		throw exception_n::elc_exception(tmp);
	}catch(...){
		throw;//?
	}
}
value jump(ptr)noexcept{

}

//file_end

