//nil.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
distinctive inline struct nil_t final:base_constexpr_t{
	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<nil_t>;}
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const noexcept override{return this==a;}
public:
	[[nodiscard]]virtual constexpr explicit operator hash_t()const noexcept override{return hash(false);}
	[[nodiscard]]virtual constexpr explicit operator logical_bool()const noexcept override{return false;}
}nil{};

//file_end

