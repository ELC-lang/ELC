//_body.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include"array/_body.hpp"
#include"copy_on_write_array/_body.hpp"

#include"stacks/_body.hpp"

#include"hash_table/_body.hpp"

#if defined(ELC_TEST_ON)
	#include"_test.hpp"
#endif
