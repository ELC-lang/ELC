#include "pch.h"
typedef elc::string::char_type																			   test_char_type;
typedef std::basic_string<test_char_type, std::char_traits<test_char_type>, TestAllocator<test_char_type>> std_string;

static_assert(sizeof(std_string) >= sizeof(elc::string));
#pragma optimize("", off)

void claer_memory_count()noexcept {
	elc::APIs::alloc::count_info::clear();
}

template<class T>
T rand() {
	using namespace elc::defs;
	T			 aret;
	data_view<T> ret_data_view{&aret};
	for(auto& i: ret_data_view)
		i = byte(::std::rand() % 256);
	return aret;
}

//*

static void Std_StringCreation(benchmark::State& state) {
	claer_memory_count();
	for(auto _: state)
		std_string tmp((size_t)state.range(0), rand<std_string::value_type>());
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringCreation)->Range(8, 1 << 16);

static void ELC_StringCreation(benchmark::State& state) {
	claer_memory_count();
	for(auto _: state)
		elc::string tmp{(size_t)state.range(0), rand<std_string::value_type>()};
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(std_string::value_type));
}
BENCHMARK(ELC_StringCreation)->Range(8, 1 << 16);

static void ELC_StringCreation_FromConstExprStr(benchmark::State& state) {
	using namespace elc;
	claer_memory_count();
	for(auto _: state)
		elc::string string = U"hello"_constexpr_str;
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCreation_FromConstExprStr);


static void Std_StringArec(benchmark::State& state) {
	std_string			   x = U"hello";
	std_string::value_type c;
	claer_memory_count();
	for(auto _: state)
		c = x[rand<size_t>() % 5];
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringArec);

static void ELC_StringArec(benchmark::State& state) {
	elc::string			   x = U"hello";
	elc::string::char_type c;
	claer_memory_count();
	for(auto _: state)
		c = x[rand<size_t>() % 5];
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringArec);

static void Std_StringArecSet(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x[rand<size_t>() % 5] = rand<std_string::value_type>();
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringArecSet);

static void ELC_StringArecSet(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x[rand<size_t>() % 5] = rand<elc::string::char_type>();
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringArecSet);

static void Std_StringCopy(benchmark::State& state) {
	std_string x((size_t)state.range(0), rand<std_string::value_type>());
	claer_memory_count();
	for(auto _: state)
		std_string copy(x);
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringCopy)->Range(8, 1 << 16);

static void ELC_StringCopy(benchmark::State& state) {
	elc::string x{(size_t)state.range(0), rand<elc::string::char_type>()};
	claer_memory_count();
	for(auto _: state)
		elc::string copy(x);
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCopy)->Range(8, 1 << 16);

static void ELC_StringCopy_FromConstExprStr(benchmark::State& state) {
	using namespace elc;
	elc::string x = U"hello"_constexpr_str;
	claer_memory_count();
	for(auto _: state)
		elc::string copy(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringCopy_FromConstExprStr);

static void Std_StringApplyEnd(benchmark::State& state) {
	std_string x;
	claer_memory_count();
	for(auto _: state)
		x += rand<std_string::value_type>();
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringApplyEnd);

static void ELC_StringApplyEnd(benchmark::State& state) {
	elc::string x;
	claer_memory_count();
	for(auto _: state)
		x += rand<elc::string::char_type>();
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringApplyEnd);

static void Std_StringApplyHead(benchmark::State& state) {
	std_string x;
	claer_memory_count();
	for(auto _: state)
		x = rand<std_string::value_type>() + x;
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringApplyHead);

static void ELC_StringApplyHead(benchmark::State& state) {
	elc::string x;
	claer_memory_count();
	for(auto _: state)
		x.push_front(rand<elc::string::char_type>());
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringApplyHead);

static void Std_StringEqual(benchmark::State& state) {
	auto	   mychar = rand<std_string::value_type>();
	std_string x((size_t)state.range(0), mychar);
	std_string y((size_t)state.range(0), mychar);
	claer_memory_count();
	for(auto _: state)
		x == y;
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringEqual)->Range(8, 1 << 16);

static void ELC_StringEqual(benchmark::State& state) {
	auto	   mychar = rand<elc::string::char_type>();
	elc::string x{(size_t)state.range(0), mychar};
	elc::string y{(size_t)state.range(0), mychar};
	claer_memory_count();
	for(auto _: state)
		x == y;
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringEqual)->Range(8, 1 << 16);

static void Std_StringHash(benchmark::State& state) {
	std_string x((size_t)state.range(0), rand<std_string::value_type>());
	claer_memory_count();
	for(auto _: state)
		std::hash<std_string>()(x);
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringHash)->Range(8, 1 << 16);

static void ELC_StringHash(benchmark::State& state) {
	elc::string x{(size_t)state.range(0), rand<elc::string::char_type>()};
	claer_memory_count();
	for(auto _: state)
		elc::defs::hash(x);
	state.SetBytesProcessed(state.iterations() * (size_t)state.range(0) * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringHash)->Range(8, 1 << 16);

static void ELC_StringHash_Size5_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	claer_memory_count();
	for(auto _: state)
		hash(x);
	state.SetBytesProcessed(state.iterations() * 5 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringHash_Size5_MarkAsConstExprStr);

/*

static void Std_StringSize(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(Std_StringSize);

static void ELC_StringSize(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringSize);

static void ELC_StringSize_MarkAsConstExprStr(benchmark::State& state) {
	using namespace elc::defs;
	elc::string x = U"hello"_constexpr_str;
	claer_memory_count();
	for(auto _: state)
		x.size();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringSize_MarkAsConstExprStr);

static void Std_StringClear(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.clear();
	state.SetBytesProcessed(0);
}
BENCHMARK(Std_StringClear);

static void ELC_StringClear(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.clear();
	state.SetBytesProcessed(0);
}
BENCHMARK(ELC_StringClear);

static void Std_StringInsert_ToPos1(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.insert(1, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringInsert_ToPos1);

static void ELC_StringInsert_ToPos1(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.insert(1, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringInsert_ToPos1);

static void Std_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.insert(x.size()-2, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringInsert_ToPos_1UnitForwardAtTheEnd);

static void ELC_StringInsert_ToPos_1UnitForwardAtTheEnd(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.insert(x.size()-2, U"e");
	state.SetBytesProcessed(state.iterations() * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringInsert_ToPos_1UnitForwardAtTheEnd);

static void Std_StringFindFirstOf_Size5(benchmark::State& state) {
	std_string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.find_first_of(U"e");
	state.SetBytesProcessed(state.iterations() * 2 * sizeof(std_string::value_type));
}
BENCHMARK(Std_StringFindFirstOf_Size5);

static void ELC_StringFindFirstOf_Size5(benchmark::State& state) {
	elc::string x = U"hello";
	claer_memory_count();
	for(auto _: state)
		x.find_first_of(U"e");
	state.SetBytesProcessed(state.iterations() * 2 * sizeof(elc::string::char_type));
}
BENCHMARK(ELC_StringFindFirstOf_Size5);

//*/

#pragma optimize("", on)
