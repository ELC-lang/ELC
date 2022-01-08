//nil.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
const struct nil_t final:base_constexpr_t{
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const{return this==a;}
public:
	[[nodiscard]]virtual constexpr explicit operator hash_t()const{return hash(false);}
	[[nodiscard]]virtual constexpr explicit operator logical_bool()const{return false;}
}nil{};

[[nodiscard]]constexpr node_like*get_null_ptr(node_like*)noexcept{return addressof((node_like&)nil);}

//file_end

