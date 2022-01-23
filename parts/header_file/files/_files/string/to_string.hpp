//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF
namespace to_string_n{
	string to_string(nothing){
		return "nothing"_elc_string;
	}
	template<typename T>
	string num_base(T num,size_t radix,const string&radix_table){
		if constexpr(::std::is_floating_point_v<T>)
			num=num%1;
		string aret;
		do{//do while，在num为0时也有返值
			auto first_char_index=num%radix;
			if constexpr(::std::is_floating_point_v<T>)
				num-=first_char_index;
			num/=radix;
			aret.push_front(radix_table[first_char_index]);
		}
		while(num);
		return aret;
	}
	template<typename T>
	string num_base_mantissa(T num,size_t radix,const string&radix_table){
		string aret;
		while(num){
			auto first_char_index=num%1;
			num-=first_char_index;
			num*=radix;
			aret+=radix_table[first_char_index];
		}
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	string to_string(T num,size_t radix=10,const string&radix_table="0123456789abcdefghigklmnopqrstuvwxyz"_elc_string){
		if constexpr(::std::is_floating_point_v<T>){//float特殊值检查
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
				if(num==-::std::numeric_limits<T>::infinity)
					return "-infinity"_elc_string;
			}
		}
		string aret;
		if constexpr(!::std::is_unsigned_v<T>)
			if(num < 0)
				aret='-';
		aret+=num_base(num,radix,radix_table);
		if constexpr(::std::is_floating_point_v<T>){
			if(auto t=num%1){
				aret='.'+num_base_mantissa(t,radix,radix_table)
			}
		}
		return aret;
	}
}

//file_end

