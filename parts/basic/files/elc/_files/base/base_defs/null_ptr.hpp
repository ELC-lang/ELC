//null_ptr.hpp
//at namespace elc::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace null_ptr_n{
	struct can_t_use_default_null_ptr{};
	template<typename T>
	[[nodiscard]]constexpr T*get_null_ptr(type_pack_t<T>)noexcept{
		if constexpr(::std::is_convertible_v<T*,can_t_use_default_null_ptr*>)
			template_error("please overload the function get_null_ptr in the namespace where this type is defined.");
		return nullptr;
	}
	constexpr struct{
		template<typename T>
		operator T*()const noexcept{return get_null_ptr(type_pack<T>);}
		//constexpr operator decltype(nullptr)(){return nullptr;}//防止重载多重路线
	}null_ptr{};
}
using null_ptr_n::can_t_use_default_null_ptr;
using null_ptr_n::null_ptr;
typedef decltype(null_ptr) null_ptr_t;
typedef decltype(nullptr) nullptr_t;
