//_body.hpp
//at namespace ::
/*
未完成的elc解释器template文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST)
	#error "this part cannot be tested."
#endif
//include APIs：elc中可能根据编译环境而修改的底层函数
#include "../../../../../_share/APIs/str_code_convert.hpp"
#include "../../../../../_share/APIs/library_load.hpp"
//include locale：自然语言相关的字符串
#include "../../../../../_share/locale/using.hpp"//strings
namespace elc::defs{
	#include "../_share/_defs.hpp"

	#include "lib_loader.hpp"
	#if defined(ELC_TEST_ON)
	namespace lib_loader_part_test{
		#include "_test.hpp"
	}
	#endif

	#include "../_share/_undefs.hpp"
}
namespace elc{
	#include "_export.hpp"
}

//file_end

