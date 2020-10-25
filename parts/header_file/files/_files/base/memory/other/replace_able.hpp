//replace_able.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//此属性配合 "../ptr" 食用
template<typename T>
struct replace_able:non_copyable,non_moveable,is_common_attribute(replace_able){
private:
	mutable T*_m;
public:
	constexpr replace_able()noexcept:_m(get_handle(this)){}
	constexpr bool replaced()const noexcept{return _m!=get_handle(this);}
	constexpr T*get_ptr()const noexcept{
		if(!replaced())
			return _m;
		else
			return static_cast<replace_able*>(_m)->get_ptr();
	}
	constexpr void be_replace_as(T*new_p)noexcept{_m=new_p;}
	constexpr void be_replace_as(nullptr_t)noexcept{be_replace_as(null_ptr);}
};

//file_end

