//_test.hpp
//at namespace elc::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void test(){
	stest_entryevent(L"memory部分测试");
	alloc_n::test();
	lifetime_n::test();
	get_n::test();
	pool_n::test();
	stest_exitevent();
}
inline void test_log_out(){
	alloc_n::test_log_out();
	lifetime_n::test_log_out();
	get_n::test_log_out();
	pool_n::test_log_out();
}
inline void test_end(){
	alloc_n::test_end();
	lifetime_n::test_end();
	get_n::test_end();
	pool_n::test_end();
}

