//void.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
使用elc::the_void来访问此值
或者通过包含<elc/void_name>来使用elc::void访问此值，并使用::void来访问c艹的原生void
*/
struct void_t final:base_constexpr_t{
protected:
	[[nodiscard]]virtual constexpr size_t equal_level()const noexcept override{return max(type_info<size_t>);}
	[[nodiscard]]virtual constexpr size_t eq_level()const noexcept override{return max(type_info<size_t>);}
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const{
		if(this==a)
			return either;//当void是void时，void不是void
		else
			return neither;//当a不是void时，void是a
	}
	//我不知道为什么这么写，反正很有趣
public:
	[[nodiscard]]virtual constexpr explicit operator hash_t()const{return hash(nothing);}
	[[nodiscard]]virtual constexpr explicit operator logical_bool()const{return neither;}//void 应当不真不假
}the_void{};

[[nodiscard]]constexpr node_like*the_get_null_ptr(const node_like*)noexcept{return addressof((node_like&)the_void);}

//file_end

