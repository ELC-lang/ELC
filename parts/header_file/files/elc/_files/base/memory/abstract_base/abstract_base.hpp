//abstract_base.hpp
//at namespace elc::defs::memory::abstract_base_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
class abstract_base:virtual public abstract_base_vtable{
	T* abstract_method_copy_get_this(){
		is_base_ptr(this);
		return reinterpret_cast<T*>(get_ptr_after_off_set(this->_abstract_method_copy_get_this()));
	}
	T* abstract_method_get_resize_this(size_t size){
		is_base_ptr(this);
		return reinterpret_cast<T*>(get_ptr_after_off_set(this->_abstract_method_get_resize_this(size)));
	}
	T* abstract_method_get_forward_resize_this(size_t size){
		is_base_ptr(this);
		return reinterpret_cast<T*>(get_ptr_after_off_set(this->_abstract_method_get_forward_resize_this(size)));
	}

	friend class get_n::unget_t;
	friend class get_n::get_resize_t;
	friend class get_n::get_size_of_get_t;
	friend class get_n::copy_get_t;
};

//file_end

