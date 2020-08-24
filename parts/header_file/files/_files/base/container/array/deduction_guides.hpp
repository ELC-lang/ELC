//deduction_guides.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_n{
	template <class T,class...U>
	array_t(T,U...) -> array_t<T>;
	template <class T>
	array_t(array_t<T>) -> array_t<T>;
	template <class T>
	array_t(copy_on_write_array_n::copy_on_write_array_t<T>) -> array_t<T>;
}

//file_end

