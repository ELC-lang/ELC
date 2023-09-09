//_body.hpp
//at namespace ::
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
//include default_data_type：基础数据类型定义
#include "../../../../../_share/default_data_type.hpp"
namespace elc::defs{ //在elc::defs中定义内容
	#include "../_share/_defs.hpp"

	namespace base{}
	using namespace base;//导出内容：defs内的名称空间只起到大体名称分组的作用，不进行名称阻拦

	//内存泄露检测
	#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
	namespace memory{
		inline void check_memory_lack()noexcept;
		inline struct distinctive memory_lack_checker_t{
			~memory_lack_checker_t(){
				check_memory_lack();
			}
		}memory_lack_checker{};
	}
	#endif

	namespace base{
		//恭喜你找到elc内大部分语法糖所在地
		#include "template_helper.hpp"
		#include "using_std.hpp"
		#include "concept.hpp"
		#include "magic_number.hpp"

		#include "other.hpp"

		#include "cast.hpp"
		#include "special_flags.hpp"
		#include "non_Xable.hpp"
		#include "attribute.hpp"
		#include "special_attribute.hpp"
		#include "type_info.hpp"
		#include "arithmetic_type_info_prover_decl.hpp"
		#include "compare.hpp"
		#include "min_max.hpp"
		#include "math.hpp"

		#include "literal_support.hpp"
	}

	//由于vcruntime的type_info在全局定义,这里需要额外声明使用base::type_info.
	#if defined(_VCRUNTIME_H)
		using base::type_info;
	#endif

	#include "../_share/_undefs.hpp"
}
#include "../../../../../_share/basic_environment/_body.hpp"
namespace elc::defs{ //在elc::defs中定义内容
	#include "../_share/_defs.hpp"


	namespace base{
		#include "arithmetic_type_info_prover.hpp"
		#include "bit.hpp"
		#include "null_ptr.hpp"
		#include "note.hpp"
		#include "array_like.hpp"
		#include "data.hpp"
		#include "pointer.hpp"
		#include "zero.hpp"
	}

	namespace memory{
		#include "memory/lifetime/_body.hpp"
	}
	using namespace memory;

	namespace base{
		#include "hash.hpp"
		#include "range.hpp"
		#include "sort.hpp"
		#include "string_view.hpp"
		#include "type_name.hpp"
		#include "constexpr_str.hpp"
		#include "logical_bool.hpp"
		#include "count_able.hpp"
		#include "invoke.hpp"
		#include "maybe_fail_reference.hpp"
	}

	#include "../_share/_undefs.hpp"
}

//file_end

