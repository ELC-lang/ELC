//_test.hpp
/*
未完成的elc解释器base_exception文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
bool moha=0;
inline void test(){
	stest_entryevent("base_exception部分测试");
	try{
		throw elc::exception();
	}catch(elc::exception){
		moha=1;
	}
	stest_exitevent();
}
inline void test_log_out(){
	stest_assert(moha);
}
inline void test_end{
	moha=0;
}
