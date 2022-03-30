#include "pch.h"

static_assert(sizeof(std::string)>=sizeof(elc::string));

static void Std_StringCreation_Size0(benchmark::State& state) {
	for(auto _: state)
		std::string empty_string;
}
BENCHMARK(Std_StringCreation_Size0);

static void ELC_StringCreation_Size0(benchmark::State& state) {
	for(auto _: state)
		elc::string empty_string;
}
BENCHMARK(ELC_StringCreation_Size0);

static void Std_StringCreation_Size5(benchmark::State& state) {
	for(auto _: state)
		std::string string = "hello";
}
BENCHMARK(Std_StringCreation_Size5);

static void ELC_StringCreation_Size5(benchmark::State& state) {
	for(auto _: state)
		elc::string string = U"hello";
}
BENCHMARK(ELC_StringCreation_Size5);

static void ELC_StringCreation_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	for(auto _: state)
		elc::string string = U"hello"_constexpr_str;
}
BENCHMARK(ELC_StringCreation_Size5_MarkAsConstExprStr);

static void Std_StringCopy_Size5(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		std::string copy(x);
}
BENCHMARK(Std_StringCopy_Size5);

static void ELC_StringCopy_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::string copy(x);
}
BENCHMARK(ELC_StringCopy_Size5);

static void ELC_StringCopy_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		elc::string copy(x);
}
BENCHMARK(ELC_StringCopy_Size5_MarkAsConstExprStr);

static void Std_StringCopy_Size20480(benchmark::State& state) {
	std::string x = "hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		std::string copy(x);
}
BENCHMARK(Std_StringCopy_Size20480);

static void ELC_StringCopy_Size20480(benchmark::State& state) {
	elc::string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		elc::string copy(x);
}
BENCHMARK(ELC_StringCopy_Size20480);

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
		std::hash<std::string>()(x);
}
BENCHMARK(Std_StringHash_Size5);

static void ELC_StringHash_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::defs::hash(x);
}
BENCHMARK(ELC_StringHash_Size5);

static void ELC_StringHash_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		hash(x);
}
BENCHMARK(ELC_StringHash_Size5_MarkAsConstExprStr);

static void Std_StringHash_Size20480(benchmark::State& state) {
	std::string x = "hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		std::hash<std::string>()(x);
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

static void Std_StringSize(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x.size();
}
BENCHMARK(Std_StringSize);

static void ELC_StringSize(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.size();
}
BENCHMARK(ELC_StringSize);

static void ELC_StringSize_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		x.size();
}
BENCHMARK(ELC_StringSize_MarkAsConstExprStr);

static void Std_StringClear(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x.clear();
}
BENCHMARK(Std_StringClear);

static void ELC_StringClear(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.clear();
}
BENCHMARK(ELC_StringClear);

static void Std_StringInsert_ToPos1(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x.insert(1, "e");
}
BENCHMARK(Std_StringInsert_ToPos1);

static void ELC_StringInsert_ToPos1(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.insert(1, U"e");
}
BENCHMARK(ELC_StringInsert_ToPos1);

static void Std_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	std::string x = "hello";
	for(auto _: state)
		x.insert(x.size()-2, "e");
}
BENCHMARK(Std_StringInsert_ToPos_1UnitForwardAtTheEnd);

static void ELC_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.insert(x.size()-2, U"e");
}
BENCHMARK(ELC_StringInsert_ToPos_1UnitForwardAtTheEnd);

BENCHMARK_MAIN();
