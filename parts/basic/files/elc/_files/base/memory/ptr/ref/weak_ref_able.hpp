//weak_ref_able.hpp
//at namespace elc::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct weak_ref_able:ref_t,can_t_use_default_null_ptr,attribute<T,weak_ref_able<T>>{
private:
	typedef ref_t base_t;
	typedef attribute<T,ref_able<T>> attribute;
public:
	inline void cut_ref()const noexcept{
		if constexpr(!base_on_ref_able<T>)
			template_error("base on ref_able before base on weak_ref_able,please.");
		T*this_T=attribute::get_handle();
		if(base_t::cut_ref())
			if(ref_num_is_zero(this_T))
				destroy_and_free(this_T);
	}
};
