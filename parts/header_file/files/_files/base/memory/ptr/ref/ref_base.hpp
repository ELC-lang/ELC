//ref_base.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
void waiting_for_destroy(T*)noexcept{
	template_error("this function should not be instantiated,please overload the function waiting_for_destroy in the namespace where this type is defined");
}
template<typename T>
void destroy_and_free(T*a)noexcept{
	if constexpr(type_info<T>.has_attribute(build_by_get_only)&&type_info<T>.has_attribute(never_in_array))
		unget(a);
	else
		template_error("please overload the function destroy_and_free in the namespace where this type is defined.");
}
//
template<typename T>
class ref_able;
template<typename T>
class weak_ref_able;

template<typename T>
constexpr bool was_ref_able=type_info<T>.has_attribute(ref_able);
template<typename T>
constexpr bool was_weak_ref_able=type_info<T>.has_attribute(weak_ref_able);

template<typename T>
[[nodiscard]]link_num_t get_ref_num(const T*a)noexcept{
	if constexpr(!was_ref_able<T>)
		template_error("hey.");
	return attribute_ptr_cast<const ref_able>(a)->link_num();
}
template<typename T>
[[nodiscard]]link_num_t get_weak_ref_num(const T*a)noexcept{
	if constexpr(!was_weak_ref_able<T>)
		template_error("hey.");
	return attribute_ptr_cast<const weak_ref_able>(a)->link_num();
}

//file_end

