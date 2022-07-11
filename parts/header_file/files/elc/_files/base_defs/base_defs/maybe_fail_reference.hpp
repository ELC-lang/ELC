//maybe_fail_reference.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
class maybe_fail_reference{
	T*_ref_to;
public:
	constexpr maybe_fail_reference(T&a):_ref_to(&a){}
	constexpr maybe_fail_reference(const maybe_fail_reference&)=default;
	constexpr maybe_fail_reference(note::fail_t):_ref_to(nullptr){}

	[[nodiscard]]bool not_fail()noexcept{return _ref_to;}
	[[nodiscard]]bool fail()noexcept{return!not_fail();}
	T&get_ref()noexcept{return*_ref_to;}
};

//file_end

