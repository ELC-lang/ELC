//comn_ptr_t.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T,enable_if(was_ref_able<T>)>
using comn_ptr_t=base_ptr_t<T,ref_able<T>,true>;

//file_end

