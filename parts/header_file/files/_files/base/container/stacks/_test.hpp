//_test.hpp
//at namespace elc::defs::container::stack_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void test(){
	stest_entryevent(L"stack部分测试");
	{
		base_stack_t<int>a;
		stest_accert(a.size()==0);
		a.add(1);
		a.add(2);
		a.add(3);
		stest_accert(a.size()==3);
		a.remove(2);
		stest_accert(a.size()==2);
		int add_num=0;
		a.for_each(lambda_with_catch(&add_num)(int a){add_num+=a;});
		stest_accert(add_num==4);
		auto_stack_t<int>b=a;
		stest_accert(b.not_in_stack(2));
		b.remove(3);
		stest_accert(a.in_stack(3));
		a.clear();
		b=a;
		a=b;
		stest_accert(b.size()==0);
	}
	check_memory_lack();
	stest_exitevent();
}
inline void test_log_out(){
}
inline void test_end(){
}

//file_end

