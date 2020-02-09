//little_tools.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
enum set_type:bool{seted=true,not_set=false};
class flag:data_struct{
	bool _;
public:
	explicit constexpr flag(set_type a=not_set)noexcept:_(a){}
	[[nodiscard]]explicit operator bool()const noexcept{return _;}
	flag(flag&a)noexcept:_((bool)a){}
	flag(flag&&a)noexcept:_((bool)a){}
	inline void operator=(flag&)&noexcept=delete;
	inline void operator=(flag&&)&noexcept=delete;
	inline void set()noexcept{_=true;}
	inline void unset()noexcept{_=false;}
};

enum mark_type:bool{marked=true,not_mark=false};
template<typename T>
class mark_able:basic_struct,non_copyable,non_moveable{
	mutable flag m;
public:
	explicit constexpr mark_able(mark_type a=not_mark)noexcept:m(a==not_mark?not_set:seted){};
	inline void mark()const noexcept{m.set();}
	inline void unmark()const noexcept{m.unset();}
	[[nodiscard]]bool was_marked()const noexcept{return(bool)m;}
};
