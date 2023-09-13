//
// pch.h
//

#pragma once

#define _STL_EXTRA_DISABLED_WARNINGS \
26465 26429 26490 26497 26472 26482 26446 26481   \
26485 26434 26492 26457 26440 26434 26494 26408   \
26455 26432 26447 26415 26409 26432 26403 26401   \
26474 26402 26436 26475 26451 26473 26495 26456   \
26418 26819 26818 26496 26462 26460 26461 26814   \

//benchmark static definition
#define BENCHMARK_STATIC_DEFINE
#pragma comment(lib, "Shlwapi.lib")

#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <string>
#include <functional>

#define ELC_TEST_COUNT_MEMORY_ALLOC
#include "../../files/elc/string"
#include "../../files/elc/numerical_representation"
#include "../../files/elc/resolve_global_name_conflicts"//using rand gobally

template<class T>
struct TestAllocator {
	typedef T value_type;

	TestAllocator() = default;
	template<class U>
	constexpr TestAllocator(const TestAllocator<U>&)noexcept{}

	[[nodiscard]] T* allocate(std::size_t n){
		if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length{};
		auto p = (T*)elc::APIs::alloc::aligned_alloc(alignof(T), n * sizeof(T));
		if(!p)
			throw std::bad_alloc{};
		return p;
	}

	void deallocate(T* p, std::size_t n)noexcept{
		elc::APIs::alloc::free((elc::defs::byte*)p, alignof(T));
	}
};
