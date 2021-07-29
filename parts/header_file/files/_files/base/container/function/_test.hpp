//_test.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	//BLOCK:for debug
	inline void test(){
		stest_entryevent(L"function部分测试");
		{
			int tester=0;
			#if !defined(_MSC_VER)
				function_t
			#else
				function_t<void()noexcept>
			#endif
			a(lambda_with_catch (&tester)() noexcept { tester = 1; });
			static_assert(type_info<decltype(a)> == type_info<function_t<void()noexcept>>);
			stest_accert(tester==0);
			a();
			stest_accert(tester==1);
			#if !defined(_MSC_VER)
				function_t
			#else
				function_t<void()>
			#endif
			b=test;
			static_assert(type_info<decltype(b)> == type_info<function_t<void()>>);
			b=a;
			a=null_ptr;
			tester=0;
			a();
			stest_accert(tester==0);
			b();
			stest_accert(tester==1);
			swap(a,a);
			stest_accert(a!=b);
			b=a;
			stest_accert(a==b);
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

