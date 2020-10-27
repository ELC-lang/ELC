//instance_struct.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
class instance_struct:is_common_attribute(instance_struct){
	virtual void abstract_method_unget_this()final{unget(get_handle(this));}
	virtual void* _abstract_method_copy_get_this()final{
		is_instance_ptr(this);
		return copy_get(get_handle(this));
	}
	virtual void* _abstract_method_get_resize_this(size_t size)final{
		is_instance_ptr(this);
		return get_resize(get_handle(this),size);
	}
	virtual size_t abstract_method_get_size_of_get_for_this()noexcept final{return get_size_of_get(get_handle(this));}
};

//file_end

