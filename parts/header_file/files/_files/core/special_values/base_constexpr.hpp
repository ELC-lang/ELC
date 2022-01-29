//base_constexpr.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct base_constexpr_t:node_like,instance_struct<base_constexpr_t>{
protected:
	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<base_constexpr_t>;}
	[[nodiscard]]virtual constexpr size_t equal_level()const noexcept override{return magic_number::god;}
	[[nodiscard]]virtual constexpr size_t eq_level()const noexcept override{return magic_number::god;}
	virtual void waiting_for_destroy()noexcept override{ return; }
public:
	base_constexpr_t()noexcept:node_like(never_ref_num_zero){}
	constexpr virtual ~base_constexpr_t()noexcept override=default;

	[[nodiscard]]virtual setter arec(const setter)noexcept override{return setter(this);}

	virtual void destroy()noexcept override final{nothing}
	virtual void clear()noexcept override final{nothing}

	virtual void be_replace_as(ptr)noexcept override{nothing}
};

//file_end

