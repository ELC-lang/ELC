//_body.hpp
//at namespace ::
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST)
	#error "this part cannot be tested."
#endif
namespace elc::defs{
	#include"../_defs.hpp"
	
	#include"base_streams/_body.hpp"

	#if defined(ELC_TEST_ON)
	namespace base_stream_part_test{
		#include"_test.hpp"
	}
	#endif

	#include"../_undefs.hpp"
}
namespace elc{
	#include"_export.hpp"
}
