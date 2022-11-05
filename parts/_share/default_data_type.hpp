//default_data_type.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc{
	#include "./_defs.hpp"

	/*ELC内部string统一采用char_t宽字符，使用setlocale(LC_ALL,"en_US.utf8")下的UTF编码。*/
	typedef char32_t char_t;
	/*_end_by_zero注释的char_t指针来表示c_string*/
	typedef _end_by_zero char_t* string_ptr_t;
	/*_end_by_zero注释的char_t指针来表示c_string*/
	typedef _end_by_zero const char_t* const_string_ptr_t;
	/*ELC内部钦定int_t类型*/
	typedef int64_t int_t;
	/*ELC内部钦定uint_t类型*/
	typedef uint64_t uint_t;
	/*ELC内部钦定float_t类型*/
	typedef double float_t;
	/*ELC内部钦定float_size_t类型，用以描述内存大小的浮点状况*/
	typedef long double float_size_t;

	#include "./_undefs.hpp"
}

//file_end

