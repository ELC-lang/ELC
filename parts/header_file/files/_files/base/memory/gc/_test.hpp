//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF:没想好如何测试
#error
struct gc_tester:gc_able<gc_tester>,can_shrink<gc_tester>,count_able<gc_tester>,can_map_all<gc_tester>,mark_able_for_gc<gc_tester>{
	
	void shrink(){
		//?
	}
}
inline void test(){
	stest_entryevent(L"gc部分测试");
	
}
inline void test_log_out(){
	
}
inline void test_end(){
	
}
