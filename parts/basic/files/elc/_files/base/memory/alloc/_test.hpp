//_test.hpp
//at namespace elc::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace alloc_n{
	//BLOCK:for debug
	inline void test(){
		using ::std::rand;
		stest_entryevent(L"alloc部分测试");
		for(int i=rand()%100+40;i--;){
			int*p=alloc<int>(50);
			p[49]=rand();
			p[19]=666;
			realloc(p,20);
			stest_accert(p[19]==666);
			free(p);
		}
		stest_exitevent();
	}
	inline void test_log_out(){
		stest_mapeventlogwith([](const void*p,steventlist*a){
			stest_putsf(L"内存泄露于%p,分配自:",p);
			stest_printeventlist(stdout,stest_copyeventlist(a));
			free(const_cast<void*>(p));
			stest_putsf(L"已释放");
			stest_wait();
		});
	}
	inline void test_end(){
		//stest_eventlogclear();
	}
	//BLOCK_END
}
