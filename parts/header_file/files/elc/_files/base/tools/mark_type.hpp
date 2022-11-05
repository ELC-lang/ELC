//mark_type.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
表明是否被标记的enum字面量.
*/
enum class mark_type:bool{marked=true,not_mark=false};
using enum mark_type;
/*
用于gc的可达性分析的普通模板式elc属性.
*/
template<typename T>
class mark_able:non_copyable,non_moveable{
	mutable flag _m;
public:
	constexpr explicit mark_able(mark_type a=not_mark)noexcept:_m(a==not_mark?not_set:seted){};
	inline void mark()const noexcept{_m.set();}
	inline void unmark()const noexcept{_m.unset();}
	[[nodiscard]]bool was_marked()const noexcept{return(bool)_m;}
};

//file_end

