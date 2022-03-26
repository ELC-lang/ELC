#include "pch.h"

static void Std_StringCreation(benchmark::State& state) {
	for(auto _: state)
		std::string empty_string;
}
BENCHMARK(Std_StringCreation);

static void ELC_StringCreation(benchmark::State& state) {
	for(auto _: state)
		elc::string empty_string;
}
BENCHMARK(ELC_StringCreation);

static void Std_StringCopy(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		std::string copy(x);
}
BENCHMARK(Std_StringCopy);

static void ELC_StringCopy(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::string copy(x);
}
BENCHMARK(ELC_StringCopy);

static void Std_StringApplyEnd(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x += "o";
}
BENCHMARK(Std_StringApplyEnd);

static void ELC_StringApplyEnd(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x += U"o";
}
BENCHMARK(ELC_StringApplyEnd);

static void Std_StringApplyHead(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x = "o" + x;
}
BENCHMARK(Std_StringApplyHead);

static void ELC_StringApplyHead(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.push_front(U"o");
}
BENCHMARK(ELC_StringApplyHead);

static void Std_StringEqual_Size5(benchmark::State& state) {
	std::string x1 = "hello";
	std::string x2 = "hello";
	for(auto _: state)
		x1 == x2;
}
BENCHMARK(Std_StringEqual_Size5);

static void ELC_StringEqual_Size5(benchmark::State& state) {
	elc::string x1 = U"hello";
	elc::string x2 = U"hello";
	for(auto _: state)
		x1 == x2;
}
BENCHMARK(ELC_StringEqual_Size5);

static void Std_StringEqual_Size20480(benchmark::State& state) {
	std::string x1 = "hello";
	std::string x2 = "hello";
	for(int i = 13; i; i--) {
		x1 += x1;
		x2 += x2;
	}
	for(auto _: state)
		x1 == x2;
}
BENCHMARK(Std_StringEqual_Size20480);

static void ELC_StringEqual_Size20480(benchmark::State& state) {
	elc::string x1 = U"hello";
	elc::string x2 = U"hello";
	for(int i = 13; i; i--) {
		x1 += x1;
		x2 += x2;
	}
	for(auto _: state)
		x1 == x2;
}
BENCHMARK(ELC_StringEqual_Size20480);

static void Std_StringHash_Size5(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		std::hash<std::string>{}(x);
}
BENCHMARK(Std_StringHash_Size5);

static void ELC_StringHash_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::defs::hash(x);
}
BENCHMARK(ELC_StringHash_Size5);

static void Std_StringHash_Size20480(benchmark::State& state) {
	std::string x = "hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		std::hash<std::string>{}(x);
}
BENCHMARK(Std_StringHash_Size20480);

static void ELC_StringHash_Size20480(benchmark::State& state) {
	elc::string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		elc::defs::hash(x);
}
BENCHMARK(ELC_StringHash_Size20480);

BENCHMARK_MAIN();
