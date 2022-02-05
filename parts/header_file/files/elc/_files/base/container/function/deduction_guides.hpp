//deduction_guides.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	template<typename T,typename Func_t=get_function_type<T>,bool promise_nothrow_at_destruct>
	base_function_t(T)->base_function_t<Func_t,promise_nothrow_at_destruct>;
}

//file_end

