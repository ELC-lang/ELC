//_body.hpp
//at namespace elc::defs::exception_n
/*
未完成的elc解释器base_exception文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
no_vtable_class exception{
	const char_t* _constexpr_str=locale::str::exception::unknow_error;
public:
	constexpr exception()noexcept=default;
	constexpr exception(constexpr_str_t<char_t> str)noexcept:_constexpr_str(str){}
	virtual constexpr ~exception()noexcept=0;
	virtual constexpr const char_t*what()noexcept{return _constexpr_str;}
};
inline constexpr exception::~exception()noexcept{}

struct runtime_exception:exception{
	using exception::exception;
};

//file_end

