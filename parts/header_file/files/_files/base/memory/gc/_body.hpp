//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include "gc_flags/_body.hpp"

#include "gc_t/_body.hpp"

#include "defs.hpp"

#include "gc_able/_body.hpp"
#include "gc_for_alloc/_body.hpp"//for alloc (def in "../alloc/defs.hpp#L10" )

#if defined(ELC_TEST_ON)
	#include "_test.hpp"
#endif

//file_end

