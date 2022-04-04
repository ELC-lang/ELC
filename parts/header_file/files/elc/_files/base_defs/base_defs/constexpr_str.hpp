//constexpr_str.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//仅是声明
namespace constexpr_str_n {
	template<typename char_T>
	struct constexpr_str_view_t;
	template<typename char_T>
	struct constexpr_str_t;
}
using constexpr_str_n::constexpr_str_view_t;
using constexpr_str_n::constexpr_str_t;
//重载range操作
//仅是声明以及size_t包装
namespace range_n {
	template<typename T>
	[[nodiscard]]constexpr T* in_range(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]size_t in_range_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = in_range(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]size_t in_range_but_reverse_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = in_range_but_reverse(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_first_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]size_t find_first_of_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = find_first_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]size_t find_last_of_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = find_last_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]size_t find_first_not_of_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = find_first_not_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range);
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of_size_t(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		auto result = find_last_not_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
}
//开始定义
namespace constexpr_str_n{
	//constexpr_str_t的退化类型，表明这是常量字符串，但没有可用的额外信息
	template<typename char_T>
	struct constexpr_str_view_t:array_like_view_t<const char_T>{
		static constexpr size_t npos=range_n::npos;
		typedef array_like_view_t<const char_T> base_t;
		using base_t::base_t;
		//成员函数
		[[nodiscard]]constexpr const char_T*str()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr const char_T*data()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr operator const char_T*()const noexcept{return str();}
		//substr
		[[nodiscard]]constexpr constexpr_str_view_t substr(size_t pos,size_t len)const noexcept{
			return constexpr_str_view_t(base_t::cbegin()+pos,len);
		}
		[[nodiscard]]constexpr constexpr_str_view_t substr(size_t pos)const noexcept{
			return constexpr_str_view_t(base_t::cbegin()+pos,base_t::size()-pos);
		}
		[[nodiscard]]constexpr constexpr_str_view_t substr(size_t pos,const char_T*end)const noexcept{
			return constexpr_str_view_t(base_t::cbegin()+pos,end-base_t::cbegin());
		}
		[[nodiscard]]constexpr constexpr_str_view_t substr(const char_T*begin,size_t len)const noexcept{
			return constexpr_str_view_t(begin,len);
		}
		[[nodiscard]]constexpr constexpr_str_view_t substr(const char_T*begin)const noexcept{
			return constexpr_str_view_t(begin,base_t::size());
		}
		[[nodiscard]]constexpr constexpr_str_view_t substr(const char_T*begin,const char_T*end)const noexcept{
			return constexpr_str_view_t(begin,end-begin);
		}
		//find
		[[nodiscard]]size_t find(const char_T ch)const{
			return in_range_size_t(ch, *this);
		}
		[[nodiscard]]size_t reverse_find(const char_T ch)const{
			return in_range_but_reverse_size_t(ch, *this);
		}
		[[nodiscard]]size_t find(base_t str)const{
			return in_range_size_t(str, *this);
		}
		[[nodiscard]]size_t reverse_find(base_t str)const{
			return in_range_but_reverse_size_t(str, *this);
		}
		[[nodiscard]]size_t find(const char_T*str,size_t len)const{
			return in_range_size_t(array_like_view_t<const char_T>(str,len), *this);
		}
		[[nodiscard]]size_t reverse_find(const char_T*str,size_t len)const{
			return in_range_but_reverse_size_t(array_like_view_t<const char_T>(str,len), *this);
		}
		[[nodiscard]]size_t find(const constexpr_str_t<char_T>&str)const{
			return in_range_size_t(str, *this);
		}
		[[nodiscard]]size_t reverse_find(const constexpr_str_t<char_T>&str)const{
			return in_range_but_reverse_size_t(str, *this);
		}
		[[nodiscard]]size_t find(const char_T*str)const{
			return find(array_end_by_zero_t<const char_T>(str), *this);
		}
		[[nodiscard]]size_t reverse_find(const char_T*str)const{
			return reverse_find(array_end_by_zero_t<const char_T>(str), *this);
		}
		//others by range_n::find_last_of_size_t etc
		[[nodiscard]]size_t find_first_of(const char_T ch)const{
			return range_n::find_first_of_size_t(ch, *this);
		}
		[[nodiscard]]size_t find_last_of(const char_T ch)const{
			return range_n::find_last_of_size_t(ch, *this);
		}
		[[nodiscard]]size_t find_first_not_of(const char_T ch)const{
			return range_n::find_first_not_of_size_t(ch, *this);
		}
		[[nodiscard]]size_t find_last_not_of(const char_T ch)const{
			return range_n::find_last_not_of_size_t(ch, *this);
		}
		[[nodiscard]]size_t find_first_of(base_t str)const{
			return range_n::find_first_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_last_of(base_t str)const{
			return range_n::find_last_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_first_not_of(base_t str)const{
			return range_n::find_first_not_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_last_not_of(base_t str)const{
			return range_n::find_last_not_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_first_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_first_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_last_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_last_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_first_not_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_first_not_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_last_not_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_last_not_of_size_t(str, *this);
		}
		[[nodiscard]]size_t find_first_of(const char_T* str) const {
			return find_first_of(array_end_by_zero_t<const char_T>(str), *this);
		}
		[[nodiscard]]size_t find_last_of(const char_T* str) const {
			return find_last_of(array_end_by_zero_t<const char_T>(str), *this);
		}
		[[nodiscard]]size_t find_first_not_of(const char_T* str) const {
			return find_first_not_of(array_end_by_zero_t<const char_T>(str), *this);
		}
		[[nodiscard]]size_t find_last_not_of(const char_T* str) const {
			return find_last_not_of(array_end_by_zero_t<const char_T>(str), *this);
		}
		[[nodiscard]]size_t find_first_of(const char_T* str, size_t len) const {
			return find_first_of(array_like_view_t<const char_T>(str, len), *this);
		}
		[[nodiscard]]size_t find_last_of(const char_T* str, size_t len) const {
			return find_last_of(array_like_view_t<const char_T>(str, len), *this);
		}
		[[nodiscard]]size_t find_first_not_of(const char_T* str, size_t len) const {
			returnfind_first_not_of(array_like_view_t<const char_T>(str, len), *this);
		}
		[[nodiscard]]size_t find_last_not_of(const char_T* str, size_t len) const {
			return find_last_not_of(array_like_view_t<const char_T>(str, len), *this);
		}
	};
	//告知其他elc库接口这是编译时常量字符串，并提供编译时计算好的内容，包括但不限于hash、bmh头、bitmarks等，用于加速库操作
	template<typename char_T>
	struct constexpr_str_t:constexpr_str_view_t<char_T>{
		typedef constexpr_str_view_t<char_T> base_t;
		hash_t hash_result;
		range_n::match_pattern<const char_T> match_pattern;
		range_n::reverse_match_pattern<const char_T> reverse_match_pattern;
		range_n::bitmark_for_finds<const char_T> bitmark_for_finds;
		bool is_bitmark_workable;
		constexpr constexpr_str_t(const char_T* str, size_t size):
			base_t(str, size),
			hash_result(defs::hash(str, size)),
			match_pattern(array_like_view_t<const char_T>{str, size}),
			reverse_match_pattern(array_like_view_t<const char_T>{str, size}) {
				is_bitmark_workable = bitmark_for_finds.mark(*this);
			}
		[[nodiscard]]constexpr hash_t hash()const noexcept{return hash_result;}
	};
	template<class char_T,size_t N>
	struct constexpr_str_t_literal_helper{
		const char_T* _array_begin;
		constexpr constexpr_str_t_literal_helper(const char_T(&str)[N])noexcept{
			_array_begin=str;
		}
		[[nodiscard]]constexpr auto get_constexpr_str_t()const noexcept{
			return constexpr_str_t{_array_begin,N-1};
		}
	};
	template<constexpr_str_t_literal_helper v>
	struct literal_result_provider {
		distinctive static constexpr auto value = v.get_constexpr_str_t();
	};
	//别问，问就是傻逼c++
	//告知其他elc库接口这是编译时常量字符串，并提供编译时计算好的内容，包括但不限于hash、bmh头、bitmarks等，用于加速库操作
	template<constexpr_str_t_literal_helper str_helper>
	[[nodiscard]]inline consteval auto& operator ""_constexpr_str()noexcept{
		return literal_result_provider<str_helper>::value;
	}
	//空字串常量且str或begin会指向一个0终止字符串（而不是nullptr），便于安全初始化
	template<class char_T>
	distinctive inline constexpr auto empty_constexpr_str_of = constexpr_str_t(&const_default_value_of<char_T>,0);
};
using constexpr_str_n::empty_constexpr_str_of;
using constexpr_str_n::operator ""_constexpr_str;

//重载range操作
namespace range_n {
	template<typename T>
	[[nodiscard]]constexpr T* in_range(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		return pattern.match_pattern.match(range);
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		return pattern.reverse_match_pattern.match(range);
	}
	//find_first_of
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmark_workable){
			return find_first_of_bitmark(pattern.bitmark_for_finds,range);
		}
		else{
			return find_first_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_last_of
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmark_workable){
			return find_last_of_bitmark(pattern.bitmark_for_finds,range);
		}
		else{
			return find_last_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_first_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmark_workable){
			return find_first_not_of_bitmark(pattern.bitmark_for_finds,range);
		}
		else{
			return find_first_not_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_last_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmark_workable){
			return find_last_not_of_bitmark(pattern.bitmark_for_finds,range);
		}
		else{
			return find_last_not_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
};

//file_end

