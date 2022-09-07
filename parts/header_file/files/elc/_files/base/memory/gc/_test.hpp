//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if!defined(_MSC_VER)
struct gc_tester:type_info_t<gc_tester>::template_name
	with_common_attribute<can_map_all,can_shrink,count_able,mark_able_for_gc,never_in_array,ref_able>,
	have_root,build_by_get_only,force_use_default_null_ptr{
	distinctive static inline int shrink_time=0;
	distinctive static inline int destroy_time=0;
	~gc_tester()noexcept{destroy_time++;}
	void shrink(){
		shrink_time++;
	}
	static void reset_times(){
		shrink_time=destroy_time=0;
	}
};
static_assert(type_info<gc_tester>.base_on<can_map_all<gc_tester>>);
void map_and_mark_for_gc(gc_tester*a){
	attribute_ptr_cast<mark_able_for_gc>(a)->mark();
}
void destory_by_gc(gc_tester*a){
	a->~gc_tester();
}
#endif
inline void test(){
	#if!defined(_MSC_VER)
	ELC_TEST_EVENTNAME("gc部分测试");
	using ::std::time;
	using ::std::rand;
	using ::std::srand;
	srand((unsigned)time(nullptr));
	gc.add_gc_method(type_info<gc_tester>);
	for(int t=rand()%7;t--;)
	{
		int i;
		root_ptr_t<gc_tester> name=get<gc_tester>();
		for(i=rand()%72;i--;){
			#pragma warning(suppress: 4189)
			auto p=get<gc_tester>();
		}
		gc();
		stest_accert(gc_tester::shrink_time==i+1);
		stest_accert(gc_tester::destroy_time==i);
		stest_accert(get_size_of_get(&*name)==1);
	}
	check_memory_lack();
	#endif
}
inline void test_log_out(){

}
inline void test_end(){

}

//file_end

