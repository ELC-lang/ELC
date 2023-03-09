//_test.hpp
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
inline void test(){
	ELC_TEST_EVENTNAME("string部分测试");
	{
		{
			string_t a=es"";
			stest_accert(a.size()==0);
			a=es"asd";
			stest_accert(a.size()==3);
			a=es"asd"+a;
			stest_accert(a.size()==6);
			a+=es"asd";
			stest_accert(es"asd"+a==es"asdasdasdasd");
			a[1]=ec('e');
			stest_accert(a.substr(0,3)==es"aed");
			stest_accert(a.substr(3).size()==6);
			stest_accert(a[2]==ec('d'));
			a.clear();
			stest_accert(a.size()==0);
			a.resize(3,ec('d'));
			stest_accert(a==es"ddd");
			for(const char_t&c:a)
				stest_accert(c==ec('d'));
			a=es"abc";
			stest_accert(a.begin()==a.cbegin());
			stest_accert(*a.begin()==ec('a'));
			stest_accert(a.begin()<=a.cend());
			stest_accert(*a.rbegin()==ec('c'));
			stest_accert(a.rbegin()<=a.rend());
			stest_accert(a.rbegin()==a.end()-1);
			a.push_back(es"as");
			a.push_front(ec('p'));
			stest_accert(a == es"pabcas");
			a.erase(1,3);
			a.erase(1);
			stest_accert(a == es"ps");
			stest_accert(a.pop_front() == ec('p'));
			stest_accert(a.pop_back() == ec('s'));
			stest_accert(a.size() == 0);
			a=es"abc";
			stest_accert(a.pop_back(2) == es"bc");
			stest_accert(a.size() == 1);
			a.push_front(es"wqer");
			stest_accert(a==es"wqera");
			a.insert(2,es"123");
			a.insert(0,es"123");
			a.insert(a.size(),es"123");
			stest_accert(a[2] == ec('3'));
			a[5]=ec('6');
			a[6]=ec('6');
			a[7]=ec('6');
			a[8]=ec('6');
			stest_accert(a == es"123wq6666ra123");
			for(char_t& c: a.view())
				c ='7';
			stest_accert(a == es"77777777777777");
		}
		{
			using namespace elc;
			using elc::defs::hash;
			auto a = es"ab"_elc_string;//constexpr_string_data_t
			stest_accert(hash(a) == hash(string_view(es"ab")));
			string b = es"ab";//comn_string_data_t
			stest_accert(hash(b) == hash(string_view(es"ab")));
			stest_accert(b.memory_cost() > a.memory_cost());
			a += b;//sum_string_data_t -> constexpr_string_data_t
				   //				   -> comn_string_data_t
			stest_accert(hash(a) == hash(string_view(es"abab")));
			a.insert(1, 'c');//inserted_string_data_t -> sum_string_data_t
			stest_accert(hash(a) == hash(string_view(es"acbab")));
			a.erase(2, 3);//erased_string_data_t -> inserted_string_data_t
			stest_accert(hash(a) == hash(string_view(es"ac")));
			a.push_back(es"123");//end_apply_string_data_t -> erased_string_data_t
			stest_accert(hash(a) == hash(string_view(es"ac123")));
			a.push_front(es"123");//head_apply_string_data_t -> end_apply_string_data_t
			stest_accert(hash(a) == hash(string_view(es"123ac123")));
		}
	}
	check_memory_lack();
}
inline void test_log_out(){
}
inline void test_end(){
}

//file_end

