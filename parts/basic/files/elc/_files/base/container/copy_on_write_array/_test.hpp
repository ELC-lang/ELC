//_test.hpp
//at namespace elc::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace copy_on_write_array_n{
	//BLOCK:for debug
	inline void test(){
		stest_entryevent(L"copy_on_write_array部分测试");
		{
			copy_on_write_array_t<int>anarray;
			stest_accert(anarray.size()==0);
			anarray.resize(5);
			stest_accert(anarray.size()==5);
			anarray[2]=99;
			anarray.resize(3);
			stest_accert(anarray[2]==99);
			array_t<int>bnarray(5);
			swap(anarray,bnarray);
			stest_accert(bnarray[2]==99);
			stest_accert(anarray.size()==5);
			swap(bnarray,anarray);
			anarray=bnarray;
			bnarray=anarray;
			swap(bnarray,bnarray);
			bnarray=bnarray;
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
