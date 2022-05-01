//_test.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace vector_n{
	//BLOCK:for debug
	inline void test(){
		ELC_TEST_EVENTNAME("vector部分测试");
		{
			vector_t<int>anvector;
			stest_accert(anvector.size()==0);
			anvector.resize(5);
			stest_accert(anvector.size()==5);
			anvector[2]=99;
			anvector.resize(3);
			stest_accert(anvector[2]==99);
			anvector=anvector;
			swap(anvector,anvector);
		}
		check_memory_lack();
	}
	inline void test_log_out(){
	}
	inline void test_end(){
	}
	//BLOCK_END
}

//file_end

