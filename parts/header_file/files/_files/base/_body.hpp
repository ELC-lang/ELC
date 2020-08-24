//_body.hpp
//at namespace ::
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST)
	#error "this part cannot be tested."
#endif
#include "../../../../_share/APIs/alloc.hpp"//using at memory/alloc
#include "../../../../_share/APIs/die.hpp"//using at memory/gc
#include "../../../../_share/locale/using.hpp"//strings
namespace elc::defs{
	#include "../_share/_defs.hpp"
	namespace base{
		#include "base_defs/_body.hpp"
		#include "tools/_body.hpp"
	}
	using namespace base;

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
	using namespace memory;

	#if defined(ELC_TEST_ON)
	namespace base_part_test{
		#include "_test.hpp"
	}
	#endif

	#include "../_share/_undefs.hpp"
}
namespace elc{
	#include "_export.hpp"
}

//file_end

