//_body.hpp
//at namespace ::
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST)
	#error "this part cannot be tested."
#endif
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace core{
		#include "base_def/_body.hpp"
		#include "runtime_exception/_body.hpp"
		#include "special_values/_body.hpp"
		#include "nodes/_body.hpp"
	}

	#if defined(ELC_TEST_ON)
	namespace core_part_test{
		#include "_test.hpp"
	}
	#endif

	#include "../_share/_undefs.hpp"
}
namespace elc{
	#include "_export.hpp"
}
#if defined(ELC_STRING)
	#include "../_multi/string_arec_as_value.hpp"
#endif

//file_end
