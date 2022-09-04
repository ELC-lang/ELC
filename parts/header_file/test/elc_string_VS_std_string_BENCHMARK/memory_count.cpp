#include "pch.h"
#include <new>

class ElcMemoryManager: public benchmark::MemoryManager {
	void Start() noexcept override {
		elc::APIs::alloc::count_info::clear();
	}
	void Stop(Result* result) noexcept override {
		// The number of allocations made in total between Start and Stop.
		result->num_allocs = elc::APIs::alloc::count_info::alloc_count;

		// The peak memory use between Start and Stop.
		result->max_bytes_used = elc::APIs::alloc::count_info::memory_using_max;

		// The total memory allocated, in bytes, between Start and Stop.
		// Init'ed to TombstoneValue if metric not available.
		result->total_allocated_bytes = elc::APIs::alloc::count_info::alloc_size;

		// The net changes in memory, in bytes, between Start and Stop.
		// ie., total_allocated_bytes - total_deallocated_bytes.
		// Init'ed to TombstoneValue if metric not available.
		result->net_heap_growth = elc::APIs::alloc::count_info::alloc_size - elc::APIs::alloc::count_info::free_size;
	}
};

int main(int argc, char** argv) {
	std::unique_ptr<benchmark::MemoryManager> mm(new ElcMemoryManager());
	::benchmark::Initialize(&argc, argv);
	benchmark::RegisterMemoryManager(mm.get());
	if(::benchmark::ReportUnrecognizedArguments(argc, argv))
		return 1;
	::benchmark::RunSpecifiedBenchmarks();
	::benchmark::Shutdown();
	return 0;
}
