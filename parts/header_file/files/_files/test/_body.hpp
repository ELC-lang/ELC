//_body.hpp
//at namespace ::
/*
未完成的elc解释器test文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc{
	inline void test(){
		stest_entryevent(L"elc总测试");
		#define CALL_NAME test
		#include "map.hpp"
		#undef CALL_NAME
		stest_exitevent();
	}
	inline void test_log_out(){
		#define CALL_NAME test_log_out
		#include "map.hpp"
		#undef CALL_NAME
	}
	inline void test_end(){
		#define CALL_NAME test_end
		#include "map.hpp"
		#undef CALL_NAME
	}
}

//file_end

