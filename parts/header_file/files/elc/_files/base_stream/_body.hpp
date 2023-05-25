//_body.hpp
//at namespace ::
/*
未完成的elc解释器base_stream文件
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
#if defined(ELC_TEST)
	#error "this part cannot be tested."
#endif
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace stream_n{
		#include "base_streams/_body.hpp"
		#include "operators/_body.hpp"

		#include "unreadable_wrap/_body.hpp"
		#include "std_wrap/_body.hpp"
	}
	using namespace stream_n;

	#if defined(ELC_TEST_ON)
	namespace base_stream_part_test{
		#include "_test.hpp"
	}
	#endif

	#include "../_share/_undefs.hpp"
}
namespace elc{
	#include "_export.hpp"
}
#if defined(ELC_STRING)
	#include "../_multi/type_or_function_name_test_output.hpp"
#endif
#if defined(ELC_NUMERICAL_REPRESENTATION)
	#include "../_multi/arithmetic_text_io.hpp"
#endif

//file_end

