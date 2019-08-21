//special_flag.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

#define defspecflag(name)\
constexpr struct name##_t{} name{}

defspecflag(never_ref_num_zero);
defspecflag(special_init);

#undef defspecflag
