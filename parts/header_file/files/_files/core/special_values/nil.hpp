//nil.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline struct nil_t final:base_constexpr_t{
protected:
	[[nodiscard]]virtual const base_type_info_t& get_type_info()const noexcept override{return type_info<nil_t>;}
	[[nodiscard]]virtual logical_bool equal_with(ptr a)const{
		if(a->get_type_info()==type_info<nil_t>)
			return true;
		else
			return false;
	}
public:
	[[nodiscard]]virtual constexpr explicit operator hash_t()const{return hash(false);}
	[[nodiscard]]virtual constexpr explicit operator logical_bool()const{return false;}
}nil{};

[[nodiscard]]constexpr node_like*get_null_ptr(type_info_t<node_like>)noexcept{return addressof(nil);}

//file_end

