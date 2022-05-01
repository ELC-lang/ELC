//_test.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace forward_vector_n{
	//BLOCK:for debug
	inline void test(){
		ELC_TEST_EVENTNAME("forward_vector部分测试");
		{
			forward_vector_t<int>anforward_vector;
			stest_accert(anforward_vector.size()==0);
			anforward_vector.resize(5);
			stest_accert(anforward_vector.size()==5);
			anforward_vector[2]=99;
			anforward_vector.resize(3);
			stest_accert(anforward_vector[2]==99);
			anforward_vector=anforward_vector;
			swap(anforward_vector,anforward_vector);
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

