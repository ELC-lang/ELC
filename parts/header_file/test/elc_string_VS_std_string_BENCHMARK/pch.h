//
// pch.h
//

#pragma once

#include <gtest/gtest.h>
#pragma comment(lib, "Shlwapi.lib")
#include <benchmark/benchmark.h>


#include <string>
#include <functional>

#define ELC_TEST_COUNT_MEMORY_ALLOC
#include "../../files/elc/string"

template<class T>
struct TestAllocator {
	typedef T value_type;

	TestAllocator() = default;
	template<class U>
	constexpr TestAllocator(const TestAllocator<U>&) noexcept {}

	[[nodiscard]] T* allocate(std::size_t n) {
		if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length{};
		auto p = (T*)elc::APIs::alloc::aligned_alloc(alignof(T), n * sizeof(T));
		if(!p)
			throw std::bad_alloc{};
		return p;
	}

	void deallocate(T* p, std::size_t n) noexcept {
		elc::APIs::alloc::free((elc::defs::byte*)p, alignof(T));
	}
};
