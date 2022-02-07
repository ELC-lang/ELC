//instance_struct.hpp
//at namespace elc::defs::memory::abstract_base_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:26436)//无虚析构函数警告diss
#endif
template<class T>
class instance_struct:is_common_attribute(instance_struct),virtual public abstract_base_vtable{
protected:
	virtual void abstract_method_unget_this()noexcept_as(unget(get_handle(this)))override{unget(get_handle(this));}
	virtual void* _abstract_method_copy_get_this()noexcept(copy_get.able<T>?copy_get.nothrow<T>:1)override{
		if constexpr(copy_get.able<T>){
			is_instance_ptr(this);
			return copy_get(get_handle(this));
		}
		else{
			return get_handle(this);
		}
	}
	virtual void* _abstract_method_get_resize_this(size_t size)noexcept(get_resize.able<T>?get_resize.nothrow<T>:1)override{
		if constexpr(get_resize.able<T>){
			is_instance_ptr(this);
			return get_resize(get_handle(this),size);
		}
		else{
			return get_handle(this);
		}
	}
	virtual void* _abstract_method_get_forward_resize_this(size_t size)noexcept(get_forward_resize.able<T>?get_forward_resize.nothrow<T>:1)override{
		if constexpr(get_forward_resize.able<T>){
			is_instance_ptr(this);
			return get_forward_resize(get_handle(this),size);
		}
		else{
			return get_handle(this);
		}
	}
	virtual size_t abstract_method_get_size_of_get_for_this()noexcept override{return get_size_of_get(get_handle(this));}
};
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

//file_end

