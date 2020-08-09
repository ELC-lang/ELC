//deduction_guides.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//无法给base_ptr_t上推导指引所以给每个常用特化都上一遍
template<class T,enable_if(type_info<T>.has_attribute(ref_able))>
comn_ptr_t(T*) -> comn_ptr_t<T>;
template<class T,enable_if(type_info<T>.has_attribute(weak_ref_able))>
weak_ptr_t(T*) -> weak_ptr_t<T>;
template<class T,enable_if(type_info<T>.has_attribute(ref_able))>
nocheck_ptr_t(T*) -> nocheck_ptr_t<T>;

template<class T>
comn_ptr_t(convert_interface_t<T>) -> comn_ptr_t<T>;
template<class T,enable_if(type_info<T>.has_attribute(weak_ref_able))>
weak_ptr_t(convert_interface_t<T>) -> weak_ptr_t<T>;
template<class T>
nocheck_ptr_t(convert_interface_t<T>) -> nocheck_ptr_t<T>;

//file_end

