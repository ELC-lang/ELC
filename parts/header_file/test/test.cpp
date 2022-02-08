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

#define ELC_TEST_CHECK_MEMORY_LACK
#include <elc/core>
#include <elc/string>
#include <elc/void_name>

void ste::stst(){
	using namespace elc;
	using namespace elc::defs;
	using namespace elc::defs::core;
	using elc::void;
	{
		ptr a = get<common_node>();
		ptr b = get<common_node>();
		stest_accert((*a)[nil] == void);
		(*a)[nil] = t;
		stest_accert((*a)[nil] == t);
		stest_accert((*b)[t] == void);
		(*a)[nil] >> (*b)[t];
		stest_accert((*b)[t] == t);
		(*a)[nil] = a;
		stest_accert((*b)[t] == a);
		weak_ptr wk;
		{
			ptr c = get<common_node>();
			wk = c;
			(*a)[c] = t;
			stest_accert((*a)[c] == t);
		}
		stest_accert((*a)[wk] == void);
		delete a;
		stest_accert(a == &void);
		stest_accert((*b)[t] == void);
		stest_accert(a == nullptr);
		stest_accert(a(void,a,t) == void);
		a = get<base_binary_function_node>([b] {
			delete (*b)[t];
		});
		(*b)[t] = a;
		stest_accert((*b)[t] != void);
		a();
		stest_accert((*b)[t] == void);
		stest_accert(a == nullptr);
		a = make_binary_node_from<int_t>(72);
		stest_accert(use_as<int_t>(a) == 72);
		stest_accert(a == make_binary_node_from<int_t>(72));
		b = make_binary_node_from<string>(U"12345");
		stest_accert(use_as<char_t>((*b)[1]) == U'2');
		(*b)[1] = U'3';
		stest_accert(use_as<string>(b) == U"13345");
		struct 机枪1942 {
			int 子弹数 = 114514;
		};
		struct 希特勒: 机枪1942 {
			int 人头数 = 191980;
		};
		a = make_binary_node_from<希特勒>(希特勒{});
		stest_accert(use_as<机枪1942>(a).子弹数 == 114514);
		stest_accert(use_as<希特勒>(a).人头数 == 191980);
	}
}
