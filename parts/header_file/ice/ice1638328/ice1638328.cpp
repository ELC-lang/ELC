// ice1638328.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "../../files/core"

int main(){
	using namespace elc;
	using namespace elc::defs;
	memory::gc.add_gc_method(elc::defs::type_info<node_like>);
}
