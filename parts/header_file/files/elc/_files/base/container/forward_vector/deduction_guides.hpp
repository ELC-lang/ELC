//deduction_guides.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace forward_vector_n{
	template <class T,class...U>
	forward_vector_t(T,U...) -> forward_vector_t<T>;
	template <class T>
	forward_vector_t(forward_vector_t<T>) -> forward_vector_t<T>;
}

//file_end

