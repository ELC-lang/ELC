// ice10187197.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "../../files/elc/base"
void func(){}
int main(size_t argc,char**_argv) {
	using namespace elc;
	using namespace elc::defs;
	function_t test=&func;
	test();
}
