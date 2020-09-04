//_body.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include "alloc/_body.hpp"
#include "lifetime/_body.hpp"
#include "get/_body.hpp"

#include "pool/_body.hpp"//依赖list

#include "other/_body.hpp"//依赖list
#include "ptr/_body.hpp"

//#include "gc/_body.hpp"//NOTE:gc不在此处定义，因为它依赖container

#if defined(ELC_TEST_ON)
	#include "_test.hpp"
#endif

//file_end

