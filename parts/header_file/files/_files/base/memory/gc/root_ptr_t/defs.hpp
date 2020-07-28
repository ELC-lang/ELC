//defs.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
using ::elc::defs::memory::gc_n::root_of;
using ::elc::defs::memory::gc_n::root_of;
template<typename T,enable_if(
								was_ref_able<T>&&
								ptr_t<T,ref_able<T>>::check_nothrow&&
								type_info<T>.has_attribute(map_and_mark_able)
							)>
struct root_ptr_t:base_ptr_t<T,ref_able<T>>,root_of<T>{
	typedef base_ptr_t<T,ref_able<T>>base_t;
	typedef root_ptr_t<T,ref_able<T>>this_t;
public:
	using base_t::base_t;
	template<class assign_t,enable_if_not_ill_form(declvalue(base_t)=declvalue(assign_t))>
	this_t&operator=(assign_t&&a)noexcept_as(declvalue(base_t)=declvalue(assign_t)){
		base_t::operator=(forward<assign_t>(a));
		return*this;
	}
	virtual T& get()noexcept override{
		return operator*();
	}
	virtual void map_and_mark()noexcept override{
		return operator*();
	}
};

//file_end

