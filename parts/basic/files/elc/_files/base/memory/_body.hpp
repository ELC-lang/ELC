//_body.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include"gc/_body.hpp"

#if !defined(_MSC_VER)//for syntax check
//https://developercommunity.visualstudio.com/content/problem/683248/a-bug-of-parameter-pack.html
#include"pool/_body.hpp"
#endif

#include"alloc/_body.hpp"
#include"lifetime/_body.hpp"
#include"get/_body.hpp"

#include"other/_body.hpp"
#include"ptr/_body.hpp"

#if defined(ELC_TEST_ON)
	#include"_test.hpp"
#endif
