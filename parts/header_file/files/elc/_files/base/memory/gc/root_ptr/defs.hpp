//defs.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
enable_adl(map_and_mark_for_gc);
template<typename T>
void map_and_mark_for_gc(T*)noexcept{
	template_error("this function should not be instantiated,please overload the function map_and_mark_for_gc in the namespace where this type is defined");
}
using ::elc::defs::memory::gc_n::root_of;
template<class T,enable_if(
								was_ref_able<T> &&
								comn_ptr_t<T>::replace_check_nothrow &&
								type_info<T>.has_attribute(gc_n::have_root)
							)>
struct root_ptr_t:comn_ptr_t<T>,root_of<T>{
	typedef comn_ptr_t<T>base_t;
	typedef root_ptr_t<T>this_t;
public:
	using base_t::base_t;
	template<class assign_t> requires was_not_an_ill_form(declvalue(base_t)=declvalue(assign_t))
	this_t&operator=(assign_t&&a)noexcept_as(base_t::operator=(forward<assign_t>(a))){
		base_t::operator=(forward<assign_t>(a));
		return*this;
	}
	virtual T& get()noexcept override{
		return base_t::operator*();
	}
	virtual void map_and_mark()noexcept override{
		map_and_mark_for_gc(&**this);
	}
};

//file_end

