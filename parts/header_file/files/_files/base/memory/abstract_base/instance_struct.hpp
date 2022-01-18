//instance_struct.hpp
//at namespace elc::defs::memory::abstract_base_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
class instance_struct:is_common_attribute(instance_struct),virtual public abstract_base_vtable{
	virtual void abstract_method_unget_this()override final{unget(get_handle(this));}
	virtual void* _abstract_method_copy_get_this()override final{
		if constexpr(copy_get.able<T>){
			is_instance_ptr(this);
			return copy_get(get_handle(this));
		}
		else{
			return get_handle(this);
		}
	}
	virtual void* _abstract_method_get_resize_this(size_t size)override final{
		if constexpr(get_resize.able<T>){
			is_instance_ptr(this);
			return get_resize(get_handle(this),size);
		}
		else{
			return get_handle(this);
		}
	}
	virtual void* _abstract_method_get_forward_resize_this(size_t size)override final{
		if constexpr(get_forward_resize.able<T>){
			is_instance_ptr(this);
			return get_forward_resize(get_handle(this),size);
		}
		else{
			return get_handle(this);
		}
	}
	virtual size_t abstract_method_get_size_of_get_for_this()noexcept override final{return get_size_of_get(get_handle(this));}
};

//file_end

