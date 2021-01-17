//_body.hpp
//at namespace ::
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(ELC_TEST) //查重 对于是否已加载过该hpp文件的判断
	#error "this part cannot be tested." //中止并提示错误信息
#endif
#include "../../../../_share/APIs/alloc.hpp"//using at memory/alloc
#include "../../../../_share/APIs/die.hpp"//using at memory/gc
#include "../../../../_share/locale/using.hpp"//strings
namespace elc::defs{ //在elc::defs中定义内容
	#include "../_share/_defs.hpp"
	namespace base{ //elc::defs::base
		#include "base_defs/_body.hpp"
		#include "tools/_body.hpp"
	}
	using namespace base; //将定义完的base引入 下同理

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

	#if defined(ELC_TEST_ON) //通过检索ELC_TEST_ON是否被def 来定义附加项
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

