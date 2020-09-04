//default_data_type.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc{
	/*ELC内部string统一采用char_t宽字符，使用setlocale(LC_ALL,"en_US.utf8")下的UTF编码。*/
	typedef char32_t char_t;//等有char64_t记得叫我
	/*ELC内部钦定int_t类型*/
	typedef int64_t int_t;
	/*ELC内部钦定uint_t类型*/
	typedef uint64_t uint_t;
}

//file_end

