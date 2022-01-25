//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace to_string_n{
	string to_string(nothing){
		return es"nothing"_elc_string;
	}
	template<typename T>
	string num_base(T num,size_t radix,const string radix_table){
		string aret;
		do{//do while，在num为0时也有返值
			T first_char_index{};
			if constexpr(::std::is_floating_point_v<T>)
				first_char_index= ::std::fmod(num,(T)radix);
			else
				first_char_index= num%radix;
			if constexpr(::std::is_floating_point_v<T>)
				num-=first_char_index;
			num/=(T)radix;
			aret.push_front(radix_table[(size_t)first_char_index]);
		}
		while(num);
		return aret;
	}
	template<typename T>
	string num_base_mantissa(T num,size_t radix,const string radix_table){
		string aret;
		while(num){
			num*=radix;
			T first_char_index;
			num=::std::modf(num,&first_char_index);
			aret+=radix_table[(size_t)first_char_index];
		}
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	string to_string(T num,size_t radix=10,const string radix_table=es"0123456789abcdefghigklmnopqrstuvwxyz"_elc_string){
		if constexpr(::std::is_floating_point_v<T>){//float特殊值检查
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN){
				if(num==::std::numeric_limits<T>::signaling_NaN())
					return es"signaling_NaN"_elc_string;
			}
			if constexpr(::std::numeric_limits<T>::has_quiet_NaN){
				if(num==::std::numeric_limits<T>::quiet_NaN())
					return es"quiet_NaN"_elc_string;
			}
			if constexpr(::std::numeric_limits<T>::has_infinity){
				if(num==::std::numeric_limits<T>::infinity())
					return es"infinity"_elc_string;
				if(num==-::std::numeric_limits<T>::infinity())
					return es"-infinity"_elc_string;
			}
		}
		typedef decltype(lambda{
			if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
				return T();
			else
				return::std::make_unsigned_t<T>();
		}()) UT;
		//符号转无符号而不是num=-num避免INT_MAX这种情况下的溢出
		UT unum=UT(num);
		string aret;
		if constexpr(!::std::is_unsigned_v<T>)
			if(num < 0){
				aret=ec('-');
				unum=UT(-num);
			}
		if constexpr(::std::is_floating_point_v<T>){
			UT num_fractional=::std::modf(unum,&unum);
			if(num_fractional){
				aret+=ec('.')+num_base_mantissa(num_fractional,radix,radix_table);
			}
		}
		aret.push_front(num_base(unum,radix,radix_table));
		return aret;
	}
}
using to_string_n::to_string;

//file_end

