//_test.hpp
//at namespace elc::defs::base_part_test
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void test(){
	ELC_TEST_EVENTNAME("base部分测试");
	memory::test();
	container::test();
	gc_n::test();
}
inline void test_log_out(){
	memory::test_log_out();
	container::test_log_out();
}
inline void test_end(){
	memory::test_end();
	container::test_end();
}

//file_end

