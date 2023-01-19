//arithmetic_text_io.hpp
//at namespace ::
//multi when ELC_NUMERICAL_REPRESENTATION & ELC_BASE_STREAM
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if !defined(ELC_multi_arithmetic_text_io)//pls skip define check as this part is muti
#define ELC_multi_arithmetic_text_io
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace stream_n{
		//arithmetic output only for text_ostream<char_t>
		template<arithmetic_type T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream<char_t>> &&
															type_info<remove_cvref<T>> != type_info<char_t> &&
															type_info<remove_cvref<T>> != type_info<bool>)
		decltype(auto)operator<<(stream_T&&stream,T&&data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			return stream << to_string(forward<T>(data));
		}

		template<typename T,class stream_T> requires(!::std::is_arithmetic_v<remove_cvref<T>> &&
													 to_arithmetic.able<remove_cvref<T>> &&
													 type_info<stream_T>.base_on<text_ostream<char_t>>)
		decltype(auto)operator<<(stream_T&& stream, T&& data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>> && to_arithmetic.nothrow<T>) {
			return stream << to_arithmetic(forward<T>(data));
		}

		template<typename T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream<char_t>> &&
													 type_info<remove_cvref<T>> == type_info<bool>)
		decltype(auto)operator<<(stream_T&&stream,T&&data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			if(data==true)
				stream << es"true"_constexpr_str;
			elseif(data==false)
				stream << es"false"_constexpr_str;
			else
				stream << es"others("_constexpr_str << union_cast<unsigned_specific_size_t<sizeof(bool)>>(data) << ec(')');
			return stream;
		}

		//T* output only for text_ostream<char_t>
		template<typename T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream<char_t>>)
		decltype(auto)operator<<(stream_T&&stream,T*data)noexcept(type_info<stream_T>.base_on<noexcept_text_ostream<char_t>>){
			//output name of type at first
			stream << type_info<T>.get_name();
			if constexpr(::std::is_polymorphic_v<T> && !::std::is_final_v<T>){//RTTI
				const auto typeinfo = type_info_of(*data);
				if(typeinfo != type_info<T>)
					stream << ec("(") << typeinfo.get_name() << ec(")");
			}
			//output address
			stream << ec("@") << to_string((size_t)data,numerical_representation_n::hexadecimal);
			return stream;
		}
	}

	#include "../_share/_undefs.hpp"
}
#endif

//file_end

