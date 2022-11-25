// elc_rand_VS_std_BENCHMARK.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//elc::rand
#include <elc/base_defs>
//std::mt19937
#include <random>
//std::rand
#include <cstdlib>

//google benchmark
#define BENCHMARK_STATIC_DEFINE
#include <benchmark/benchmark.h>

void ELC_rand_8bit(benchmark::State& state) {
	using elc::defs::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint8_t>());
	}
	state.counters["state_size"] = sizeof(elc::defs::rand_seed);
}
BENCHMARK(ELC_rand_8bit);

void ELC_rand_32bit(benchmark::State& state) {
	using elc::defs::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint32_t>());
	}
	state.counters["state_size"] = sizeof(elc::defs::rand_seed);
}
BENCHMARK(ELC_rand_32bit);

void ELC_rand_64bit(benchmark::State& state) {
	using elc::defs::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint64_t>());
	}
	state.counters["state_size"] = sizeof(elc::defs::rand_seed);
}
BENCHMARK(ELC_rand_64bit);

void Std_mt19937_32bit(benchmark::State& state) {
	std::mt19937 mt;
	for(auto _: state) {
		benchmark::DoNotOptimize(mt());
	}
	state.counters["state_size"] = sizeof(mt);
}
BENCHMARK(Std_mt19937_32bit);

void Std_mt19937_64bit(benchmark::State& state) {
	std::mt19937_64 mt;
	for(auto _: state) {
		benchmark::DoNotOptimize(mt());
	}
	state.counters["state_size"] = sizeof(mt);
}
BENCHMARK(Std_mt19937_64bit);

void Std_rand(benchmark::State& state) {
	for(auto _: state) {
		benchmark::DoNotOptimize(std::rand());
	}
	state.counters["state_size"] = sizeof(int);
}
BENCHMARK(Std_rand);

BENCHMARK_MAIN();
