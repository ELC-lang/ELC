// default.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include <cstdlib>
namespace std{//cpp20还未实现，占位。
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]]inline void* aligned_alloc([[maybe_unused]]std::size_t alignment,std::size_t size){ return malloc(size); }
	#endif
}

#include "../../files/string"

using namespace elc;

#include "../../../_share/_defs.hpp"



#include <iostream>

int main()
{
	std::cout << elc::APIs::str_code_convert::to_char_str(es"114514.191980");
}

#include "../../../_share/_undefs.hpp"
