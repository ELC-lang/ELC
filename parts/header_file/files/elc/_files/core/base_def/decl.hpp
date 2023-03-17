//decl.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
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
struct node_like;
typedef comn_ptr_t<node_like>ptr;
typedef weak_ptr_t<node_like>weak_ptr;
typedef comn_ptr_t<const node_like>const_ptr;
typedef weak_ptr_t<const node_like>const_weak_ptr;
struct value;

BREAK_NAMESPACE
namespace string_n{
	template<typename char_T>
	struct string_t;
	typedef string_t<char_t> string;
	template<constexpr_str_n::constexpr_str_t_literal_helper str_helper>
	inline constexpr string operator""_elc_string()noexcept;
}
using string_n::string_t;
using string_n::string;
INTER_NAMESPACE(core)

template<typename T>
[[nodiscard]]inline ptr make_long_term_binary_node_from(T a)noexcept;

template<typename T>
constexpr bool as_ptr_nothrow_helper()noexcept{
	if constexpr(was_not_an_ill_form(ptr(&declvalue(T))))
		return noexcept(ptr(&declvalue(T)));
	elseif constexpr(was_not_an_ill_form(const_ptr(&declvalue(T))))
		return noexcept(const_ptr(&declvalue(T)));
	elseif constexpr(was_not_an_ill_form(static_cast<node_like&>(declvalue(T))))
		return noexcept(&static_cast<node_like&>(declvalue(T)));
	elseif constexpr(was_not_an_ill_form(static_cast<const node_like&>(declvalue(T))))
		return noexcept(&static_cast<const node_like&>(declvalue(T)));
	elseif constexpr(was_not_an_ill_form(static_cast<value&>(declvalue(T))))
		return noexcept(ptr(static_cast<value&>(declvalue(T))));
	elseif constexpr(was_not_an_ill_form(static_cast<const value&>(declvalue(T))))
		return noexcept(const_ptr(static_cast<value&>(declvalue(T))));
	elseif constexpr(was_not_an_ill_form(ptr(declvalue(T))))
		return noexcept(ptr(declvalue(T)));
	elseif constexpr(was_not_an_ill_form(const_ptr(declvalue(T))))
		return noexcept(const_ptr(declvalue(T)));
	elseif constexpr(type_info<remove_cvref<T>> == type_info<char_t>)
		return noexcept(make_long_term_binary_node_from<char_t>(declvalue(T)));
	elseif constexpr(basic_integer_type<remove_cvref<T>>){
		if constexpr(signed_type<remove_cvref<T>>)
			return noexcept(make_long_term_binary_node_from<int_t>(declvalue(T)));
		else
			return noexcept(make_long_term_binary_node_from<uint_t>(declvalue(T)));
	}
	elseif constexpr(basic_float_type<remove_cvref<T>>)
		return noexcept(make_long_term_binary_node_from<float_t>(declvalue(T)));
	elseif constexpr(construct<string>.able<T>){
		return noexcept(make_long_term_binary_node_from<string>(declvalue(T)));
	}
}
template<typename T>
decltype(auto) as_ptr(T&&a)noexcept(as_ptr_nothrow_helper<T>()){
	ELC_TEST_EVENTNAME("as_ptr转换");
	if constexpr(was_not_an_ill_form(ptr(&a)))
		return ptr(&a);
	elseif constexpr(was_not_an_ill_form(const_ptr(&a)))
		return const_ptr(&a);
	elseif constexpr(was_not_an_ill_form(static_cast<node_like&>(a)))
		return &static_cast<node_like&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<const node_like&>(a)))
		return &static_cast<const node_like&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<value&>(a)))
		return ptr(static_cast<value&>(a));
	elseif constexpr(was_not_an_ill_form(static_cast<const value&>(a)))
		return const_ptr(static_cast<value&>(a));
	elseif constexpr(was_not_an_ill_form(ptr(a)))
		return ptr(a);
	elseif constexpr(was_not_an_ill_form(const_ptr(a)))
		return const_ptr(a);
	elseif constexpr(type_info<remove_cvref<T>> == type_info<char_t>)
		return make_long_term_binary_node_from<char_t>(a);
	elseif constexpr(basic_integer_type<remove_cvref<T>>){
		if constexpr(signed_type<remove_cvref<T>>)
			return make_long_term_binary_node_from<int_t>(a);
		else
			return make_long_term_binary_node_from<uint_t>(a);
	}
	elseif constexpr(basic_float_type<remove_cvref<T>>)
		return make_long_term_binary_node_from<float_t>(a);
	elseif constexpr(construct<string>.able<T>){
		return make_long_term_binary_node_from<string>(a);
	}
}

template<typename T>
constexpr bool as_value_nothrow_helper()noexcept{
	if constexpr(was_not_an_ill_form(static_cast<value&>(declvalue(T))))
		return noexcept(static_cast<value&>(declvalue(T)));
	elseif constexpr(was_not_an_ill_form(static_cast<const value&>(declvalue(T))))
		return noexcept(static_cast<const value&>(declvalue(T)));
	elseif constexpr(was_not_an_ill_form(as_ptr(declvalue(T)))){
		if constexpr(type_info<decltype(as_ptr(declvalue(T)))> == type_info<ptr>)
			return noexcept(value(as_ptr(declvalue(T))));
		elseif constexpr(type_info<decltype(as_ptr(declvalue(T)))> == type_info<const_ptr>)
			return noexcept((const value)remove_const((const node_like*)as_ptr(declvalue(T))));
	}
}
template<typename T>
decltype(auto) as_value(T&& a)noexcept(as_value_nothrow_helper<T>()){
	ELC_TEST_EVENTNAME("as_value转换");
	if constexpr(was_not_an_ill_form(static_cast<value&>(a)))
		return static_cast<value&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<const value&>(a)))
		return static_cast<const value&>(a);
	elseif constexpr(was_not_an_ill_form(as_ptr(declvalue(T)))){
		if constexpr(type_info<decltype(as_ptr(declvalue(T)))> == type_info<ptr>)
			return value(as_ptr(forward<T>(a)));
		elseif constexpr(type_info<decltype(as_ptr(declvalue(T)))> == type_info<const_ptr>)
			return (const value)remove_const((const node_like*)as_ptr(forward<T>(a)));
	}
}

//file_end

