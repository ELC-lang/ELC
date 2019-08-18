//_test.hpp
//at namespace elc::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void test(){
	stest_entryevent(L"container部分测试");
	array_n::test();
	copy_on_write_array_n::test();
	stest_exitevent();
}
inline void test_log_out(){
	array_n::test_log_out();
	copy_on_write_array_n::test_log_out();
}
inline void test_end(){
	array_n::test_end();
	copy_on_write_array_n::test_end();
}
