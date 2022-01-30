/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//*
#define STEVE_H_TEST
#include <steve.h>
/*/
#define al_adapt_steve_h
#define STEST_ON
#include <ala>
//*/

#include "../files/core"
#include "../files/void_name"

void ste::stst(){
	using namespace elc;
	using elc::void;
	{
		ptr a = defs::get<defs::core::common_node>();
		stest_accert((*a)[nil] == void);
		(*a)[nil] = t;
		stest_accert((*a)[nil] == t);
		delete a;
		stest_accert(a == &nil);
	}
}
