//func_call.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF: 草稿代码
struct Arg_Node:Comn_Node{
	typedef Arg_Node this_t;
	typedef Comn_Node base_t;
	using base_t::_value;

}
template<typename...Args>
add_args_to(Comn_Node&a,node_pair&b,Args&&...rest){
	a.add(ptr)
}
template<typename...Args>
add_args_to(Comn_Node&a,ptr&b,Args&&...rest){
	a.add(ptr)
}
template<typename...Args>
ptr make_arg_list(Args&&...rest){
	Arg_Node aret;

}


//file_end

