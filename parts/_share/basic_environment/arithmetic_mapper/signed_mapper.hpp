//signed_mapper.hpp
/*
未完成的elc解释器_share文件
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
TYPE_MAPPER(int8_t)
TYPE_MAPPER(int16_t)
TYPE_MAPPER(int32_t)
TYPE_MAPPER(int64_t)
#if defined(ELC_BASE_ENV_HAS_INT128)
	TYPE_MAPPER(int128_t)
#endif
TYPE_MAPPER(::std::intmax_t)
TYPE_MAPPER(ptrdiff_t)
TYPE_MAPPER(signed char)
TYPE_MAPPER(short)
TYPE_MAPPER(int)
TYPE_MAPPER(long)
TYPE_MAPPER(long long)

//file_end

