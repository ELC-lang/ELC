//_body.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace core_runtime_internal_helper_n{
	struct base_t:base_runtime_exception{
		using base_runtime_exception::base_runtime_exception;
		constexpr base_t()noexcept:
			base_runtime_exception(locale::str::exception::core_runtime_internal_helper_leak){}
	};
	//
	struct goto_t:base_t{
		ptr to;
	};
	struct return_t:base_t{};
}
//#include ".hpp"

//file_end

