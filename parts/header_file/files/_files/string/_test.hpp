//_test.hpp
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline void test(){
	ELC_TEST_EVENTNAME("string部分测试");
	{
		string_t a=L"";
		stest_accert(a.size()==0);
		a=L"asd";
		stest_accert(a.size()==3);
		a=L"asd"+a;
		stest_accert(a.size()==6);
		a+=L"asd";
		stest_accert(L"asd"+a==L"asdasdasdasd");
		a[1]=L'e';
		stest_accert(a.substr(0,3)==L"aed");
		stest_accert(a.substr(3).size()==6);
		stest_accert(a[2]==L'd');
		a.clear();
		stest_accert(a.size()==0);
		a.resize(3,L'd');
		stest_accert(a==L"ddd");
		for(const wchar_t&c:a)
			stest_accert(c==L'd');
		a=L"abc";
		stest_accert(a.begin()==a.cbegin());
		stest_accert(*a.begin()==L'a');
		stest_accert(a.begin()<=a.cend());
		stest_accert(*a.rbegin()==L'c');
		stest_accert(a.rbegin()<=a.rend());
		stest_accert(a.rbegin()==a.end()-1);
		a.push_back(L"as");
		a.push_front(L'p');
		stest_accert(a == L"pabcas");
		a.erase(1,3);
		a.erase(1);
		stest_accert(a == L"ps");
		stest_accert(a.pop_front() == L'p');
		stest_accert(a.pop_back() == L's');
		stest_accert(a.size() == 0);
		a=L"abc";
		stest_accert(a.pop_back(2) == L"bc");
		stest_accert(a.size() == 1);
		a.push_front(L"wqer");
		stest_accert(a==L"wqera");
		a.insert(2,L"123");
		a.insert(0,L"123");
		a.insert(a.size(),L"123");
		stest_accert(a[2] == L'3');
		a[5]=L'6';
		a[6]=L'6';
		a[7]=L'6';
		a[8]=L'6';
		stest_accert(a == L"123wq6666ra123");
		for(wchar_t& c: a)
			c ='7';
		stest_accert(a == L"77777777777777");
	}
	check_memory_lack();
}
inline void test_log_out(){
}
inline void test_end{
}

//file_end

