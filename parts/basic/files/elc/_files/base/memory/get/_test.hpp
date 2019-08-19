//_test.hpp
//at namespace elc::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace get_n{
	//BLOCK:for debug
	inline void test(){
		stest_entryevent(L"get部分测试");
		ste::tester::setzero();
		tester*p=get<tester>[50]();
		stest_accert(tester::getbuildtime()==50);
		get_resize(p,60);
		//destroytime:50(+50) cause move(move_construct&destruct).
		stest_accert(tester::getdestroytime()==50);
		stest_accert(tester::getbuildtime()==60);
		int i=0;
		do
			p[i]();//写入测试
		while(++i!=60);
		stest_accert(tester::getcalltime()==60);
		unget(p);
		//destroytime:110(+60) cause destruct.
		stest_accert(tester::getdestroytime()==110);
		stest_exitevent();
	}
	inline void test_log_out(){
		alloc_n::test_log_out();
	}
	inline void test_end(){
		alloc_n::test_end();
	}
	//BLOCK_END
}
