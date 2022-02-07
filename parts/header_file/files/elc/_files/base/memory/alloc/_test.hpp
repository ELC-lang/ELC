//_test.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void check_memory_lack()noexcept{
	stest_mapeventlogwith(lambda(const void*p,steventlist*a)noexcept{
		stest_putsf(L"内存泄露于%p,分配自:",p);
		stest_printeventlist(stdout,stest_copyeventlist(a));
		//alloc_n::base_free(remove_const(p));
		//stest_puts(L"已释放");
		stest_wait();
	});
}
#if defined(ELC_TEST_ON)
	namespace alloc_n{
		//BLOCK:for debug
		inline void test(){
			ELC_TEST_EVENTNAME("alloc部分测试");
			using ::std::time;
			using ::std::rand;
			using ::std::srand;
			srand((unsigned int)time(nullptr));
			for(int i=rand()%100+40;i--;){
				int*p=alloc<int>(50);
				p[0]=72;
				stest_accert(get_size_of_alloc(p)==50);
				p[49]=rand();
				p[19]=666;
				realloc(p,20);
				stest_accert(get_size_of_alloc(p)==20);
				stest_accert(p[19]==666);
				stest_accert(p[0]==72);
				free(p);
			}
			check_memory_lack();
		}
		inline void test_log_out(){
		}
		inline void test_end(){
		}
		//BLOCK_END
	}
#endif

//file_end

