//*
#define STEVE_H_TEST
#include <steve.h>
/*/
#define al_adapt_steve_h
#define STEST_ON
#include <ala>
//*/

#include <cstdlib>//aligned_alloc:malloc
namespace std{//cpp20还未实现，占位。
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]]inline void*aligned_alloc([[maybe_unused]]std::size_t alignment,std::size_t size){return malloc(size);}
	#endif
}

#define ELC_TEST_ON
#include "../files/base"
#include "../files/core"
#include "../files/test"

void ste::stst(){
	elc::test();
	elc::test_log_out();
	elc::test_end();
}
