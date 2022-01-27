// default.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
未完成的elc解释器dev文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include "../../files/string"

using namespace elc;

#include <iostream>
#include <steve.h>

void ste::stst()
{
	elc::string_t a = U"123";
	elc::string_t b = U"123"_elc_string;
	stest_accert(a.memory_cost() > b.memory_cost());
	stest_accert(a == b);
	stest_accert(a.memory_cost() == b.memory_cost());
}

