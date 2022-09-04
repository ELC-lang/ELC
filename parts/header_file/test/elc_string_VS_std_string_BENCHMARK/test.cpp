#include "pch.h"
typedef elc::string::char_type test_char_type;
typedef std::basic_string<test_char_type,std::char_traits<test_char_type>,TestAllocator<test_char_type>> std_string;

static_assert(sizeof(std_string)>=sizeof(elc::string));
#pragma optimize("", off)

//*

static void Std_StringCreation_Size0(benchmark::State& state) {
	for(auto _: state)
		std_string empty_string;
	state.SetBytesProcessed(0);
}
BENCHMARK(Std_StringCreation_Size0);

static void ELC_StringCreation_Size0(benchmark::State& state) {
	for(auto _: state)
		elc::string empty_string;
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringCreation_Size0);

static void Std_StringCreation_Size5(benchmark::State& state) {
	for(auto _: state)
		std_string string = U"hello";
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringCreation_Size5);

static void ELC_StringCreation_Size5(benchmark::State& state) {
	for(auto _: state)
		elc::string string = U"hello";
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCreation_Size5);

static void ELC_StringCreation_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc;
	for(auto _: state)
		elc::string string = U"hello"_constexpr_str;
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCreation_Size5_MarkAsConstExprStr);


static void Std_StringArec(benchmark::State& state) {
	std_string x = U"hello";
	std_string::value_type c;
	for(auto _: state)
		c = x[0];
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringArec);

static void ELC_StringArec(benchmark::State& state) {
	elc::string x = U"hello";
	elc::string::char_type c;
	for(auto _: state)
		c = x[0];
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringArec);

static void Std_StringArecSet(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x[0] = 'e';
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringArecSet);

static void ELC_StringArecSet(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x[0] = U'e';
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringArecSet);

static void Std_StringCopy_Size5(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		std_string copy(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringCopy_Size5);

static void ELC_StringCopy_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::string copy(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCopy_Size5);

static void ELC_StringCopy_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		elc::string copy(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCopy_Size5_MarkAsConstExprStr);

static void Std_StringCopy_Size20480(benchmark::State& state) {
	std_string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		std_string copy(x);
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringCopy_Size20480);

static void ELC_StringCopy_Size20480(benchmark::State& state) {
	elc::string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		elc::string copy(x);
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCopy_Size20480);

static void Std_StringApplyEnd(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x += U"o";
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringApplyEnd);

static void ELC_StringApplyEnd(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x += U"o";
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringApplyEnd);

static void Std_StringApplyHead(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x = U"o" + x;
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringApplyHead);

static void ELC_StringApplyHead(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.push_front(U"o");
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringApplyHead);

static void Std_StringEqual_Size5(benchmark::State& state) {
	std_string x1 = U"hello";
	std_string x2 = U"hello";
	for(auto _: state)
		x1 == x2;
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringEqual_Size5);

static void ELC_StringEqual_Size5(benchmark::State& state) {
	elc::string x1 = U"hello";
	elc::string x2 = U"hello";
	for(auto _: state)
		x1 == x2;
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringEqual_Size5);

static void Std_StringEqual_Size20480(benchmark::State& state) {
	std_string x1 = U"hello";
	std_string x2 = U"hello";
	for(int i = 13; i; i--) {
		x1 += x1;
		x2 += x2;
	}
	for(auto _: state)
		x1 == x2;
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(std_string::value_type));
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
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringEqual_Size20480);

static void Std_StringHash_Size5(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		std::hash<std_string>()(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringHash_Size5);

static void ELC_StringHash_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		elc::defs::hash(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringHash_Size5);

static void ELC_StringHash_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		hash(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringHash_Size5_MarkAsConstExprStr);

static void Std_StringHash_Size20480(benchmark::State& state) {
	std_string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		std::hash<std_string>()(x);
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringHash_Size20480);

static void ELC_StringHash_Size20480(benchmark::State& state) {
	elc::string x = U"hello";
	for(int i = 13; i; i--)
		x += x;
	for(auto _: state)
		elc::defs::hash(x);
	state.SetBytesProcessed(state.iterations() * 20480 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringHash_Size20480);

static void Std_StringSize(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(Std_StringSize);

static void ELC_StringSize(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringSize);

static void ELC_StringSize_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringSize_MarkAsConstExprStr);

static void Std_StringClear(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x.clear();
	state.SetBytesProcessed(0);
}
BENCHMARK(Std_StringClear);

static void ELC_StringClear(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.clear();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringClear);

static void Std_StringInsert_ToPos1(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x.insert(1, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringInsert_ToPos1);

static void ELC_StringInsert_ToPos1(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.insert(1, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringInsert_ToPos1);

static void Std_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x.insert(x.size()-2, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringInsert_ToPos_1UnitForwardAtTheEnd);

static void ELC_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.insert(x.size()-2, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringInsert_ToPos_1UnitForwardAtTheEnd);

static void Std_StringFindFirstOf_Size5(benchmark::State& state) {
	std_string x = U"hello";
	for(auto _: state)
		x.find_first_of(U"e");
	state.SetBytesProcessed(state.iterations() * 2 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringFindFirstOf_Size5);

static void ELC_StringFindFirstOf_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	for(auto _: state)
		x.find_first_of(U"e");
	state.SetBytesProcessed(state.iterations() * 2 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringFindFirstOf_Size5);

#pragma optimize("", on)

//*/
