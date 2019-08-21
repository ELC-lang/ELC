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
	if constexpr(!::std::is_convertible_v<T*,build_by_get_only*>)
		template_error("please overload the function special_destroy in the namespace where this type is defined.");
	unget(a);
}
//
template<typename T>
class ref_able;
template<typename T>
class weak_ref_able;

template<typename T>
constexpr bool base_on_ref_able=::std::is_convertible_v<T*,ref_able<T>*>;
template<typename T>
constexpr bool base_on_weak_ref_able=::std::is_convertible_v<T*,weak_ref_able<T>*>;

template<typename T>
[[nodiscard]]link_num_t get_ref_num(const T*a)noexcept{
	if constexpr(!base_on_ref_able<T>)
		template_error("hey.");
	return static_cast<const ref_able<T>*>(a)->link_num();
}
template<typename T>
[[nodiscard]]link_num_t get_weak_ref_num(const T*a)noexcept{
	if constexpr(!base_on_weak_ref_able<T>)
		template_error("hey.");
	return static_cast<const weak_ref_able<T>*>(a)->link_num();
}
