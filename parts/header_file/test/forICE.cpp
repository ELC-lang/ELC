#include <cstdlib>//aligned_alloc:malloc
namespace std{//cpp20还未实现，占位。
	template<size_t N,class T>
	[[nodiscard]]constexpr T*assume_aligned(T*ptr){return ptr;}
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]]inline void*aligned_alloc([[maybe_unused]]std::size_t alignment,std::size_t size){return malloc(size);}
	#endif
}

#include "../files/base"
#include "../files/core"
