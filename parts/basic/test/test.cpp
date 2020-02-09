/*
#define STEVE_H_TEST
#include<evest.h>
#define STEST_ON
#include<steve.h>
/*/
#define al_adapt_steve_h
#define STEST_ON
#include <ala>
//*/

#define ELC_TEST_ON
#include"../files/elc/base"
#include"../files/elc/test"

int ste::stst(){
	elc::test();
	elc::test_log_out();
	elc::test_end();
	ret 0;
}
