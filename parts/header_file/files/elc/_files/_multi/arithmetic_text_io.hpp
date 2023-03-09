//arithmetic_text_io.hpp
//at namespace ::
//multi when ELC_NUMERICAL_REPRESENTATION & ELC_BASE_STREAM
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
#if !defined(ELC_multi_arithmetic_text_io)//pls skip define check as this part is muti
#define ELC_multi_arithmetic_text_io
namespace elc::defs{
	#include "../_share/_defs.hpp"

	namespace stream_n{
		using namespace numerical_representation_n;

		const auto&default_numerical_representation=decimal;
		const auto&default_bool_representation=common_bool_representation;

		template<text_stream stream_T>
		decltype(auto)get_numerical_representation_of(stream_T&&stream)noexcept{
			if constexpr was_not_an_ill_form(stream.get_numerical_representation())
				return stream.get_numerical_representation();
			else
				return default_numerical_representation;
		}

		template<text_stream stream_T>
		decltype(auto)get_bool_representation_of(stream_T&&stream)noexcept{
			if constexpr was_not_an_ill_form(stream.get_bool_representation())
				return stream.get_bool_representation();
			else
				return default_bool_representation;
		}

		//arithmetic output only for text_ostream
		template<arithmetic_type T,text_ostream stream_T> requires(type_info<remove_cvref<T>> != type_info<char_t> &&
																   type_info<remove_cvref<T>> != type_info<bool>)
		decltype(auto)operator<<(stream_T&&stream,T&&data)noexcept(noexcept_text_ostream<stream_T>){
			return stream << to_string(forward<T>(data),get_numerical_representation_of(stream));
		}

		template<typename T,text_ostream stream_T> requires(!::std::is_arithmetic_v<remove_cvref<T>> &&
															to_arithmetic.able<remove_cvref<T>>)
		decltype(auto)operator<<(stream_T&&stream,T&&data)noexcept(noexcept_text_ostream<stream_T> && to_arithmetic.nothrow<T>) {
			return stream << to_arithmetic(forward<T>(data));
		}

		template<typename T,text_ostream stream_T> requires(type_info<T> == type_info<bool>)
		decltype(auto)operator<<(stream_T&&stream,T data)noexcept(noexcept_text_ostream<stream_T>){
			const auto&representation=get_bool_representation_of(stream);
			if(data==true)
				stream << representation.get_true();
			elseif(data==false)
				stream << representation.get_false();
			else
				stream << representation.get_other_prefix() << union_cast<unsigned_specific_size_t<sizeof(bool)>>(data) << representation.get_other_suffix();
			return stream;
		}

		//arithmetic input only for text_istream
		template<arithmetic_type T,text_istream stream_T> requires(type_info<remove_cvref<T>> != type_info<char_t> &&
																   type_info<remove_cvref<T>> != type_info<bool>)
		decltype(auto)operator>>(stream_T&&stream,T&data)noexcept(noexcept_text_istream<stream_T>){
			string temp;
			stream >> temp;
			data=from_string_get<T>(temp,get_numerical_representation_of(stream));
			return stream;
		}
		template<typename T,text_istream stream_T> requires(type_info<T> == type_info<bool>)
		decltype(auto)operator>>(stream_T&&stream,T&data)noexcept(noexcept_text_istream<stream_T>){
			string temp;
			stream >> temp;
			const auto&representation=get_bool_representation_of(stream);
			if(temp==representation.get_true())
				data=true;
			elseif(temp==representation.get_false())
				data=false;
			else{
				if(temp.starts_with(representation.get_other_prefix())){
					temp.remove_front(representation.get_other_prefix().size());
					if(temp.ends_with(representation.get_other_suffix())){
						temp.remove_back(representation.get_other_suffix().size());
						data=union_cast<bool>(from_string_get<unsigned_specific_size_t<sizeof(bool)>>(temp,get_numerical_representation_of(stream)));
					}
				}
			}
			return stream;
		}

		//T* output only for text_ostream
		template<typename T,text_ostream stream_T>
		decltype(auto)operator<<(stream_T&&stream,T*data)noexcept(noexcept_text_ostream<stream_T>){
			//output name of type at first
			stream << type_info<T>.get_name();
			if constexpr(::std::is_polymorphic_v<T> && !::std::is_final_v<T>){//RTTI
				const auto typeinfo = type_info_of(*data);
				if(typeinfo != type_info<T>)
					stream << ec("(") << typeinfo.get_name() << ec(")");
			}
			//output address
			stream << ec("@") << to_string((size_t)data,hexadecimal);
			return stream;
		}
	}

	#include "../_share/_undefs.hpp"
}
#endif

//file_end

