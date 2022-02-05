//decl.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct node_like;
typedef comn_ptr_t<node_like>ptr;
typedef weak_ptr_t<node_like>weak_ptr;
typedef comn_ptr_t<const node_like>const_ptr;
typedef weak_ptr_t<const node_like>const_weak_ptr;
struct value;

template<typename T>
[[nodiscard]]inline ptr make_binary_node_from(T a)noexcept;

template<typename T>
auto as_value(T&& a) {
	ELC_TEST_EVENTNAME("as_value转换");
	if constexpr(was_not_an_ill_form(static_cast<value&>(a)))
		return static_cast<value&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<node_like&>(a)))
		return value(&static_cast<node_like&>(a));
	elseif constexpr(was_not_an_ill_form(static_cast<const value&>(a)))
		return static_cast<const value&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<const node_like&>(a)))
		return (const value)remove_const((const node_like*)&static_cast<const node_like&>(a));
	elseif constexpr(was_not_an_ill_form(ptr(&a)))
		return value(ptr(&a));
	elseif constexpr(was_not_an_ill_form(ptr(a)))
		return value(ptr(a));
	elseif constexpr(was_not_an_ill_form(const_ptr(&a)))
		return (const value)(const_ptr(&a));
	elseif constexpr(was_not_an_ill_form(const_ptr(a)))
		return (const value)remove_const((const node_like*)const_ptr(a));
	elseif constexpr(::std::is_integral_v<remove_cvref<T>>)
		return value(make_binary_node_from<int_t>(a));
	elseif constexpr(::std::is_floating_point_v<remove_cvref<T>>)
		return value(make_binary_node_from<float_t>(a));
}

template<typename T>
auto as_ptr(T&&a){
	ELC_TEST_EVENTNAME("as_ptr转换");
	if constexpr(was_not_an_ill_form(ptr(&a)))
		return ptr(&a);
	elseif constexpr(was_not_an_ill_form(static_cast<node_like&>(a)))
		return &static_cast<node_like&>(a);
	elseif constexpr(was_not_an_ill_form(static_cast<value&>(a)))
		return ptr(static_cast<value&>(a));
	elseif constexpr(was_not_an_ill_form(ptr(a)))
		return ptr(a);
	elseif constexpr(was_not_an_ill_form(const_ptr(a)))
		return const_ptr(a);
	elseif constexpr(::std::is_integral_v<remove_cvref<T>>)
		return make_binary_node_from<int_t>(a);
	elseif constexpr(::std::is_floating_point_v<remove_cvref<T>>)
		return make_binary_node_from<float_t>(a);
}

//file_end

