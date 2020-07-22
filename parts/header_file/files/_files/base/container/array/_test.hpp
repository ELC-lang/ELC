//_test.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_n{
	//BLOCK:for debug
	inline void test(){
		stest_entryevent(L"array部分测试");
		{
			array_t<int>anarray;
			stest_accert(anarray.size()==0);
			anarray.resize(5);
			stest_accert(anarray.size()==5);
			anarray[2]=99;
			anarray.resize(3);
			stest_accert(anarray[2]==99);
			anarray=anarray;
			swap(anarray,anarray);
		}
		check_memory_lack();
		stest_exitevent();
	}
	inline void test_log_out(){
	}
	inline void test_end(){
	}
	//BLOCK_END
}

//file_end

