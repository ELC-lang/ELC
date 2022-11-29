//type_name_test_output.hpp
//at namespace ::
//multi when ELC_STRING & ELC_BASE_STREAM
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if !defined(ELC_multi_type_name_test_output)//pls skip define check as this part is muti
#define ELC_multi_type_name_test_output
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace stream_n{
		//typename output only for text_ostream<char_t>
		template<class stream_T> requires(type_info<stream_T>.base_on<text_ostream<char_t>>)
		decltype(auto)operator<<(stream_T&&stream,const type_name_t&name)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			return stream << to_char_t_str(name);
		}
	}

	#include "../_share/_undefs.hpp"
}
#endif

//file_end

