//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF
string to_string(nothing){
	return "nothing"_elc_string;
}

namespace to_string_n{
	template<typename T>
	string num_base(T num,size_t radix,const string&radix_table){
		auto first_char_index=num%radix;
		if constexpr(::std::is_floating_point_v<T>)
			num-=first_char_index;
		num/=radix;
		if(first_char_index)
			return radix_table[first_char_index]+num_base(num,radix,radix_table);
		else
			return ""_elc_string;
	}
}
template<typename T> requires ::std::is_integral_v<T>
string to_string(T num,size_t radix=10,const string&radix_table="0123456789abcdefghigklmnopqrstuvwxyz"_elc_string){
	string aret;
	if constexpr(::std::is_floating_point_v<T>){
		if constexpr(::std::numeric_limits<T>::has_signaling_NaN){
			if(num==::std::numeric_limits<T>::signaling_NaN)
				return "signaling_NaN"_elc_string;
		}
		if constexpr(::std::numeric_limits<T>::has_quiet_NaN){
			if(num==::std::numeric_limits<T>::quiet_NaN)
				return "quiet_NaN"_elc_string;
		}
		if constexpr(::std::numeric_limits<T>::has_infinity){
			if(num==::std::numeric_limits<T>::infinity)
				return "infinity"_elc_string;
		}
		if(auto t=num%1){
			aret='.'+to_string::num_base(t,radix,radix_table)
			num-=t;
		}
	}
	aret=to_string::num_base(num,radix,radix_table)+aret;
	if constexpr(!::std::is_unsigned_v<T>)
		if(num < 0)
			aret='-'+aret;
	return aret;
}

//file_end

