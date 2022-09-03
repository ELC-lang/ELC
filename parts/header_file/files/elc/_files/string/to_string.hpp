//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace to_string_n{
	inline string to_string_rough(nothing)noexcept{
		return es"nothing"_elc_string;
	}
	template<typename T>
	inline string num_base(T num,size_t radix,const string radix_table)noexcept{
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
	inline string num_base_mantissa(T num,size_t radix,const string radix_table)noexcept{
		string aret;
		T	   epsilon = ::std::numeric_limits<T>::epsilon();
		while(num > epsilon){
			num*=radix;
			epsilon*=radix;
			T first_char_index;
			num=::std::modf(num,&first_char_index);
			aret+=radix_table[(size_t)first_char_index];
		}
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string_rough(T num,size_t radix=10,const string radix_table=es"0123456789abcdefghigklmnopqrstuvwxyz"_elc_string)noexcept{
		if constexpr(::std::is_floating_point_v<T>){//float特殊值检查
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
				if(::std::isnan(num)){
					string aret=es"NaN("_elc_string;
					data_view<T> view{&num};
					for(byte c: view) {
						aret += to_string_rough((unsigned char)c, radix, radix_table);
						aret += ec('.');
					}
					aret.back() = ec(')');
					return aret;
				}
			}
			if constexpr(::std::numeric_limits<T>::has_infinity){
				if(num==::std::numeric_limits<T>::infinity())
					return es"Infinity"_elc_string;
				if(num==-::std::numeric_limits<T>::infinity())
					return es"-Infinity"_elc_string;
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
			if(::std::signbit(num)){
				aret=ec('-');
				unum=UT(-num);
			}
		UT num_fractional{};
		if constexpr(::std::is_floating_point_v<T>)
			num_fractional=::std::modf(unum,&unum);
		aret+=num_base(unum,radix,radix_table);
		if constexpr(::std::is_floating_point_v<T>)
			if(num_fractional > ::std::numeric_limits<UT>::epsilon())
				aret+=ec('.')+num_base_mantissa(num_fractional,radix,radix_table);
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string_rough(T num,const string radix_table)noexcept{
		return to_string_rough(num,radix_table.size(),radix_table);
	}
}
using to_string_n::to_string_rough;

namespace from_string_get_n{
	template<typename T>
	inline T num_base(string str,size_t radix,const string radix_table)noexcept{
		T aret{};
		for(size_t i=0;i<str.size();i++){
			const size_t index=radix_table.find(str[i]);
			if(index==string::npos)
				return T();
			aret*=(T)radix;
			aret+=(T)index;
		}
		return aret;
	}
	template<typename T>
	inline T num_base_mantissa(string str,size_t radix,const string radix_table)noexcept{
		T aret{};
		size_t i=str.size();
		while(i){
			i--;
			const size_t index=radix_table.find(str[i]);
			if(index==string::npos)
				return T();
			aret+=(T)index;
			aret/=radix;
		};
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline T from_string_get(string str,size_t radix=10,string radix_table=es"0123456789abcdefghigklmnopqrstuvwxyz"_elc_string)noexcept{
		if constexpr(::std::is_floating_point_v<T>){//float特殊值检查
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN){
				if(str.starts_with(es"NaN("_elc_string)){
					str.pop_front(4);
					str.pop_back();
					data_block<T> block;
					size_t		  write_index = 0;
					while(1) {
						const size_t dot_pos = str.find(ec('.'));
						auto byte_str			 = str.substr(0, dot_pos);
						block[write_index++] = (byte)from_string_get<unsigned char>(byte_str, radix, radix_table);
						if(dot_pos == string::npos)
							break;
						str					 = str.substr(dot_pos+1);
					}
					return data_cast<T>(block);
				}
			}
			if constexpr(::std::numeric_limits<T>::has_infinity){
				if(str==es"Infinity"_constexpr_str)
					return ::std::numeric_limits<T>::infinity();
				if(str==es"-Infinity"_constexpr_str)
					return -::std::numeric_limits<T>::infinity();
			}
		}
		radix_table.resize(radix);
		typedef decltype(lambda{
			if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
				return T();
			else
				return::std::make_unsigned_t<T>();
		}()) UT;
		UT unum{};
		bool is_negative=false;
		if constexpr(!::std::is_unsigned_v<T>)
			if(str[0]==ec('-')){
				is_negative=true;
				str.pop_front();
			}
		if constexpr(::std::is_floating_point_v<T>) {
			const size_t dot_pos=str.find(ec('.'));
			if(dot_pos!=string::npos){
				auto mantissa_str=str.substr(dot_pos+1);
				str=str.substr(0,dot_pos);
				unum=num_base_mantissa<UT>(mantissa_str,radix,radix_table);
			}
		}
		unum+=num_base<UT>(str,radix,radix_table);
		if(is_negative)
			return T(T{}-unum);
		else
			return T(unum);
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline T from_string_get(string str,const string radix_table)noexcept{
		return from_string_get<T>(str,radix_table.size(),radix_table);
	}
}
using from_string_get_n::from_string_get;

namespace to_string_n{
	inline string to_string(nothing)noexcept{
		return es"nothing"_elc_string;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string(T num,size_t radix=10,const string radix_table=es"0123456789abcdefghigklmnopqrstuvwxyz"_elc_string)noexcept{
		auto aret=to_string_rough(num,radix,radix_table);
		if constexpr(::std::is_floating_point_v<T>){
			if constexpr(::std::numeric_limits<T>::has_signaling_NaN || ::std::numeric_limits<T>::has_quiet_NaN)
				if(aret.starts_with(es"NaN("_elc_string))
					return aret;
			T tmp;
			if(::std::modf(num,&tmp)){//如果有小数部分
				size_t dot_pos=aret.find(ec('.'));
				if(dot_pos == string::npos)
					return aret;//?

				//进位器
				auto rounding_up_char = lambda_with_catch(&) (string::arec_t char_arc)noexcept{
					char_t up_char = move(char_arc);
					size_t up_pos  = radix_table.find(up_char);
					up_pos++;
					if(up_pos == radix)
						up_pos = 0;
					move(char_arc) = radix_table[up_pos];
					return up_pos == 0;
				};
				auto rounding_up = lambda_with_catch(&) (string&str)noexcept{
					size_t i = str.size();
					while(i--){
						if(str[i] == ec('.'))
							i--;
						if(rounding_up_char(str[i]))
							continue;
						else
							break;
					}
				};
				{
					//二分法查找最合适的切割位点.
					size_t left_pos	 = dot_pos + 1;
					size_t right_pos = aret.size();
					string better_aret,better_aret_last;
					do {
						size_t step_pos = (left_pos + right_pos) / 2;
						better_aret		= aret.substr(0, step_pos);
						//判断是否需要进位.(当被截断内容在radix_table的后半部分.)
						char_t cut_char = aret[step_pos];
						size_t cut_num	= radix_table.find(cut_char);
						if(cut_num >= radix / 2)
							rounding_up(better_aret);
						//判断当前切割位点有效性.
						if(from_string_get<T>(better_aret, radix, radix_table) == num) {
							if(better_aret.back() == radix_table[0]) {
								auto end_pos = better_aret.find_last_not_of(radix_table[0]);
								better_aret.resize(end_pos + 1);
								if(better_aret.back() == ec('.'))
									better_aret.pop_back();
							}
							right_pos		 = better_aret.size();
							better_aret_last = better_aret;
						}
						elseif(left_pos != step_pos)
							left_pos = step_pos;
						else
							left_pos++;
					} while(left_pos < right_pos);
					if(better_aret_last){
						aret = better_aret_last;
						if(aret.ends_with(ec('.')))
							aret.pop_back();
					}
				}
			}
		}
		return aret;
	}
	template<typename T> requires ::std::is_arithmetic_v<T>
	inline string to_string(T num,const string radix_table)noexcept{
		return to_string(num,radix_table.size(),radix_table);
	}
}
using to_string_n::to_string;

//file_end

