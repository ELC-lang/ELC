// elc-header-file-dev_string.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include "../../files/base"
#include "../../files/base_exception"


#include "../../files/_files/_share/_defs.hpp"
namespace elc::defs{
	template<typename char_T>
	struct base_string_data_t,ref_able<base_string_data_t<char_T>>{
		typedef base_string_data_t<char_T> this_t;
		typedef comn_ptr_t<this_t> ptr_t;

		virtual size_t get_c_str(ptr_t&)=0;
		virtual size_t get_size()=0;
		virtual ptr_t get_substr_data(size_t begin,size_t end)=0;
		virtual ptr_t get_data_after_apply_str_to_begin(char_T*str,size_t size)=0;
		virtual ptr_t get_data_after_apply_str_to_end(char_T*str,size_t size)=0;

		virtual ptr_t get_data_after_insert(size_t pos,char_T*str,size_t size)=0;
		virtual ptr_t get_data_after_insert(size_t pos,ptr_t str)=0;
		virtual ptr_t get_data_after_erase(size_t pos,char_T*str,size_t size)=0;
		virtual ptr_t get_data_after_erase(size_t pos,ptr_t str)=0;
	};
}
namespace elc{}
#include "../../files/_files/_share/_undefs.hpp"

int main()
{

}
