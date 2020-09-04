//_test.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace ptr_n{
	struct tester:ref_able<tester>,weak_ref_able<tester>,build_by_get_only{};
	inline void test(){
		stest_entryevent(L"ptr部分测试");
		{
			comn_ptr_t a=get<tester>();
			weak_ptr_t b=a;
		}
		check_memory_lack();
		stest_exitevent();
	}
	inline void test_log_out(){
	}
	inline void test_end(){
	}
}

//file_end

