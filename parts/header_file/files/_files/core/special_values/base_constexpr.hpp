//base_constexpr.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct base_constexpr_t:node_like{
protected:
	[[nodiscard]]virtual const base_type_info_t& get_type_info()const=0;
	[[nodiscard]]virtual logical_bool equal_with(ptr)const=0;
	[[nodiscard]]virtual constexpr size_t equal_level()const noexcept override final{return magic_number::god;}
	[[nodiscard]]virtual constexpr size_t eq_level()const noexcept override final{return magic_number::god;}
public:
	virtual ~base_constexpr_t()noexcept override=default;

	[[nodiscard]]virtual setter operator[](ptr)noexcept override final{return setter(this);}

	virtual void clear()noexcept override final{nothing}

	[[nodiscard]]virtual explicit operator logical_bool()const=0;

	virtual void destroy()noexcept override=0;
	virtual void be_replace_as(ptr)noexcept override{nothing}
	[[nodiscard]]virtual explicit operator hash_t()const=0;
};

//file_end

