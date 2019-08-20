//replace_able.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct replace_able:non_copyable,non_moveable,attribute<T,replace_able<T>>{
private:
	mutable T*_m;
	typedef attribute<T,replace_able<T>> attribute;
public:
	constexpr replace_able()noexcept:_m(attribute::get_handle()){}
	constexpr bool replaced()const noexcept{return _m!=attribute::get_handle()();}
	constexpr T*get_ptr()const noexcept{
		if(!replaced())
			return _m;
		else
			return static_cast<replace_able*>(_m)->get_ptr();
	}
	constexpr void replace(T*new_p)noexcept{_m=new_p;}
	constexpr void replace(nullptr_t)noexcept{replace(null_ptr);}
};
