//string.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
using string_t=array_t<T>;//本来是copy_on_write_t<array_t<T>>，但貌似没有必要（而且copy_on_write_t貌似越写越丑了）

typedef string_t<char_t>string;

//file_end

