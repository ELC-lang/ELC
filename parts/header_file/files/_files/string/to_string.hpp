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

template<typename T> requires ::std::is_integral_v<T>
string to_string(T num,size_t radix=10,string radix_table="0123456789abcdefghigklmnopqrstuvwxyz"_elc_string){
	string aret;
	while(num){
		aret+=radix_table[num%radix];
		num/=radix;
	}
	if constexpr(!::std::is_unsigned_v<T>)
		if(num < 0)
			aret='-'+aret;
	return aret;
}

//file_end

