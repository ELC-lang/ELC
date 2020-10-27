//_body.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF
namespace abstract_base{
	inline thread_local pointer base_ptr;
	inline thread_local ptr_diff_t off_set;
	inline void is_base_ptr(void*a)noexcept{base_ptr=a;}
	inline void is_instance_ptr(void*a)noexcept{off_set=get_off_set(note::from(a),note::to(base_ptr));}
	inline void*get_ptr_after_off_set(void*a)noexcept{return apply_off_set(a,off_set);}
	/*
	ELC的虚基类模块，适用于unget
	处理虚基类到实例类的偏移与实例化模板差异
	原理：
		虚函数间接调用以从base_t&转为T&
		运行时计算偏移以从T*转换base_t*
	用法：is_common_attribute
	*/
	#include "instance_struct_t.hpp"
	#include "abstract_base_t.hpp"
}

//file_end

