// elc-header-file-dev_literal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
/*
未完成的elc解释器dev文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//
#include <iostream>

#include <elc/base_exception>
#include <elc/core>
#include <elc/string>

#include "../../files/elc/_files/_share/_defs.hpp"

namespace elc::defs{
	namespace base_read_n{
		struct base_read_error:exception{
			const char_t* _why;
			base_read_error(const char_t* why)noexcept{ _why=why; }
			virtual const char_t* what()noexcept override{ return _why; }
		};
	};
	auto operator ""_elc_object(const char_t*,size_t)noexcept(not_in_debug){
		//return elc_code_t({L""});
	}
	auto operator ""_elc_code(const char_t*,size_t)noexcept(not_in_debug){
		//return elc_code_t({L""});
	}
}
namespace elc{
	using defs::base_read_n::base_read_error;
}

#include "../../files/elc/_files/_share/_undefs.hpp"

#define GET_CODE_STRUCT(...) ((__VA_ARGS__).get_code_struct())

int main()
{
	using namespace elc;
	
}
