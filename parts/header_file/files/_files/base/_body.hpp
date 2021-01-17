//_body.hpp
//at namespace ::
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST)//若已包含elc/test则报错：其应在此文件后被包含
	#error "this part cannot be tested."
#endif

//include APIs：elc中可能根据编译环境而修改的底层函数
#include "../../../../_share/APIs/alloc.hpp"//using at memory/alloc
#include "../../../../_share/APIs/die.hpp"//using at memory/gc
//include locale：自然语言相关的字符串
#include "../../../../_share/locale/using.hpp"//strings

namespace elc::defs{ //在elc::defs中定义内容
	#include "../_share/_defs.hpp"

	namespace base{
		#include "base_defs/_body.hpp"
		#include "tools/_body.hpp"
	}
	using namespace base;//导出内容

	namespace memory{
		#include "memory/_body.hpp"//依赖list
	}
	using namespace memory;

	namespace container{
		#include "container/_body.hpp"
	}
	using namespace container;

	namespace memory::gc_n{
		#include "memory/gc/_body.hpp"//依赖container
	}
	using namespace memory;//只是强迫症

	#if defined(ELC_TEST_ON)
	namespace base_part_test{
		#include "_test.hpp"
	}
	#endif

	#include "../_share/_undefs.hpp"
}
namespace elc{
	#include "_export.hpp"//导出base承诺于名称空间elc中提供的内容
}

//file_end

