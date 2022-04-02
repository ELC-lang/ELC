//constexpr_str.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace constexpr_str_n{
	template<typename char_T>
	struct constexpr_str_view_t:array_like_view_t<const char_T>{
		typedef array_like_view_t<const char_T> base_t;
		using base_t::base_t;
		[[nodiscard]]constexpr const char_T*str()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr operator const char_T*()const noexcept{return str();}
	};
	template<typename char_T>
	struct constexpr_str_t:constexpr_str_view_t<char_T>{
		typedef constexpr_str_view_t<char_T> base_t;
		hash_t hash_result;
		range_n::match_pattern<const char_T> match_pattern;
		range_n::reverse_match_pattern<const char_T> reverse_match_pattern;
		range_n::bitmask_for_finds<const char_T> bitmask_for_finds;
		bool is_bitmask_workable;
		constexpr constexpr_str_t(const char_T* str, size_t size):
			base_t(str, size),
			hash_result(defs::hash(str, size)),
			match_pattern(array_like_view_t<const char_T>{str, size}),
			reverse_match_pattern(array_like_view_t<const char_T>{str, size}) {
				is_bitmask_workable = bitmask_for_finds.mask(*this);
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
	template<constexpr_str_t_literal_helper str_helper>
	[[nodiscard]]inline consteval auto& operator ""_constexpr_str()noexcept{
		return literal_result_provider<str_helper>::value;
	}
	template<class char_T>
	distinctive inline constexpr auto empty_constexpr_str_of = constexpr_str_t(&const_default_value_of<char_T>,0);
};
using constexpr_str_n::constexpr_str_view_t;
using constexpr_str_n::constexpr_str_t;
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
		if(pattern.is_bitmask_workable){
			return find_first_of_bitmask(pattern.bitmask_for_finds,range);
		}
		else{
			return find_first_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_last_of
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmask_workable){
			return find_last_of_bitmask(pattern.bitmask_for_finds,range);
		}
		else{
			return find_last_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_first_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmask_workable){
			return find_first_not_of_bitmask(pattern.bitmask_for_finds,range);
		}
		else{
			return find_first_not_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
	//find_last_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of(const constexpr_str_t<T>&pattern,array_like_view_t<T>range){
		if(pattern.is_bitmask_workable){
			return find_last_not_of_bitmask(pattern.bitmask_for_finds,range);
		}
		else{
			return find_last_not_of((const constexpr_str_view_t<T>&)pattern,range);
		}
	}
};

//file_end

