// elc_rand_VS_std_BENCHMARK.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//elc::rand
#include <elc/random>
//std::mt19937
#include <random>
//std::rand
#include <cstdlib>

//google benchmark
#define BENCHMARK_STATIC_DEFINE
#include <benchmark/benchmark.h>

void Std_rand(benchmark::State& state) {
	for(auto _: state) {
		benchmark::DoNotOptimize(std::rand());
	}
	state.counters["state_size"] = sizeof(int);
}
BENCHMARK(Std_rand);

void ELC_rand_8bit(benchmark::State& state) {
	using elc::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint8_t>());
	}
	state.counters["state_size"] = sizeof(elc::rand_seed);
}
BENCHMARK(ELC_rand_8bit);

void ELC_rand_32bit(benchmark::State& state) {
	using elc::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint32_t>());
	}
	state.counters["state_size"] = sizeof(elc::rand_seed);
}
BENCHMARK(ELC_rand_32bit);

void ELC_rand_64bit(benchmark::State& state) {
	using elc::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<uint64_t>());
	}
	state.counters["state_size"] = sizeof(elc::rand_seed);
}
BENCHMARK(ELC_rand_64bit);

struct a256bit_t {
	unsigned char _data[256 / CHAR_BIT];
};

void ELC_rand_256bit(benchmark::State& state) {
	using elc::rand;
	for(auto _: state) {
		benchmark::DoNotOptimize(rand<a256bit_t>());
	}
	state.counters["state_size"] = sizeof(elc::rand_seed);
}
BENCHMARK(ELC_rand_256bit);

void Std_mt19937_rand_32bit(benchmark::State& state) {
	std::mt19937 mt;
	for(auto _: state) {
		benchmark::DoNotOptimize(mt());
	}
	state.counters["state_size"] = sizeof(mt);
}
BENCHMARK(Std_mt19937_rand_32bit);

void Std_mt19937_rand_64bit(benchmark::State& state) {
	std::mt19937_64 mt;
	for(auto _: state) {
		benchmark::DoNotOptimize(mt());
	}
	state.counters["state_size"] = sizeof(mt);
}
BENCHMARK(Std_mt19937_rand_64bit);

BENCHMARK_MAIN();
