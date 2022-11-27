//arithmetic_text_io.hpp
//at namespace ::
//multi when ELC_NUMERICAL_REPRESENTATION & ELC_BASE_STREAM
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace stream_n{
		//arithmetic output only for text_ostream<char_t>
		template<typename T,class stream_T> requires(::std::is_arithmetic_v<remove_cvref<T>> &&
													 type_info<stream_T>.base_on<text_ostream<char_t>> &&
													 type_info<remove_cvref<T>>!=type_info<char_t>)
		decltype(auto)operator<<(stream_T&stream,T&&data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			return stream << to_string(::std::forward<T>(data));
		}

		//T* output only for text_ostream<char_t>
		template<typename T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream<char_t>>)
		decltype(auto)operator<<(stream_T&stream,T*data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			//output name of type at first
			stream << type_info<T>.get_name();
			if constexpr(::std::is_polymorphic_v<T>)//RTTI
				if(type_info_of(*data)!=type_info<T>)
					stream << ec("(") << type_name_of(*data) << ec(")");
			//output address
			stream << ec("@") << to_string((size_t)data,numerical_representation_n::hexadecimal);
			return stream;
		}
	}

	#include "../_share/_undefs.hpp"
}

//file_end

