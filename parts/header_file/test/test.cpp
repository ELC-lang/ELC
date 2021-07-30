//*
#define STEVE_H_TEST
#include <steve.h>
/*/
#define al_adapt_steve_h
#define STEST_ON
#include <ala>
//*/

#include <type_traits>
#include <cstdlib>
namespace std{//cpp20还未实现，占位。
	template<size_t N,class T>
	[[nodiscard]]constexpr T*assume_aligned(T*ptr){return ptr;}
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
