//ref_t.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
typedef size_t link_num_t;
class ref_t:non_copyable,non_moveable{
	mutable link_num_t _ref_num=0;
public:
	constexpr ref_t()noexcept=default;
	constexpr explicit ref_t(never_ref_num_zero_t)noexcept:_ref_num(1){}
	constexpr inline void add_ref()const noexcept{_ref_num++;}
protected:
	[[nodiscard]]constexpr inline bool cut_ref()const noexcept{return!--_ref_num;}
public:
	constexpr inline void init_never_ref_num_zero()noexcept{add_ref();}
	[[nodiscard]]link_num_t link_num()const noexcept{return _ref_num;}
};

//file_end

