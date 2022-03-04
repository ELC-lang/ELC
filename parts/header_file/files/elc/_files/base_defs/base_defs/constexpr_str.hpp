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
	struct constexpr_str_t:array_like_view_t<const char_T>{
		typedef array_like_view_t<const char_T> base_t;
		using base_t::base_t;
		[[nodiscard]]constexpr const char_T*str()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr operator const char_T*()const noexcept{return str();}
	};
	template<class char_T,size_t N>
	struct constexpr_str_t_literal_helper{
		const char_T* _array_begin;
		constexpr constexpr_str_t_literal_helper(const char_T(&str)[N])noexcept{
			_array_begin=str;
		}
		[[nodiscard]]constexpr auto get_constexpr_str_t()const noexcept{
			return constexpr_str_t<char_T>{_array_begin,N};
		}
	};
	//别问，问就是傻逼c++
	template<constexpr_str_t_literal_helper str_helper>
	[[nodiscard]]inline constexpr auto operator ""_constexpr_str()noexcept{
		return str_helper.get_constexpr_str_t();
	}
};
using constexpr_str_n::constexpr_str_t;
using constexpr_str_n::operator ""_constexpr_str;

//file_end

