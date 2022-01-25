//_body.hpp
//at namespace ::
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs{ //在elc::defs中定义内容
	#include "../_share/_defs.hpp"

	namespace base{}
	using namespace base;//导出内容：defs内的名称空间只起到大体名称分组的作用，不进行名称阻拦

	namespace base{
		#include "base_defs/_body.hpp"
	}

	#include "../_share/_undefs.hpp"
}

//file_end

