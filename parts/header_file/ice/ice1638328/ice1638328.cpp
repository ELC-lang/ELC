// ice1638328.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdlib>
namespace std{//cpp20还未实现，占位。
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]] inline void* aligned_alloc([[maybe_unused]] std::size_t alignment,std::size_t size){ return malloc(size); }
	#endif
}

#include "../../files/core"

int main(){
	using namespace elc;
	using namespace elc::defs;
	memory::gc.add_gc_method(elc::defs::type_info<node_like>);
}
