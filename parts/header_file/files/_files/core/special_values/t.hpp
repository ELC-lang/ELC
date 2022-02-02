//t.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
distinctive inline struct t_t final:base_constexpr_t{
protected:
	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<t_t>;}
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const{
		return this==a;
	}
public:
	[[nodiscard]]virtual constexpr explicit operator hash_t()const{return hash(true);}
	[[nodiscard]]virtual constexpr explicit operator logical_bool()const{return true;}

	//nothing is true,everything is permitted.
	[[noreturn]]virtual void be_replace_as(ptr)noexcept override{::std::exit(0);}
}t{};

//file_end

