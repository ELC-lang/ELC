//func_call.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF: 草稿代码
/*
elc中的call不再是“列表调用”而是“意群调用”，每个参数都对应一个“意义”
这意味着参数的顺序将是无关紧要的，但也将造成调用表达式的难以书写
函数可以提供默认“意义列表”以完成“列表调用”
*/
struct Arg_Node:Comn_Node{
	typedef Arg_Node this_t;
	typedef Comn_Node base_t;
	using base_t::_value;

}
template<typename...Args>
void add_args_to(Comn_Node&a,Node_List&mean_list,Node arg){
	auto mean=mean_list.top();
	a[mean]=arg;
}
template<typename...Args>
void add_args_to(Comn_Node&a,Node_List&mean_list,Node arg,Args&&...rest){
	add_args_to(a,mean_list.nexts(),)
}
template<typename...Args>
ptr make_arg_list(Args&&...rest){
	Arg_Node aret;

}

//file_end

