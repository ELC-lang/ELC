//null_ptr.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace null_ptr_n{
	/*
	提醒子类定义者重载get_null_ptr.
	*/
	special_attribute_t can_t_use_default_null_ptr{};

	/*
	force_use_default_null_ptr :
	可不重载get_null_ptr
	在此类ref_able或weak_ref_able时，意味着使用者**保证**所有指向此类的实例的ptr都不可为null_ptr
	否则将内存访问错误
	*/
	special_attribute_t force_use_default_null_ptr:can_t_use_default_null_ptr{};

	void the_get_null_ptr() = delete;
	template<typename T>
	[[nodiscard]]constexpr auto get_null_ptr()noexcept{
		if constexpr(was_not_an_ill_form(the_get_null_ptr((T*)nullptr)))
			return the_get_null_ptr((T*)nullptr);
		elseif constexpr(type_info<T>.has_attribute(can_t_use_default_null_ptr)&&type_info<T>.not_has_attribute(force_use_default_null_ptr)){
			template_error("please overload the function the_get_null_ptr in the namespace where this type is defined.");
			return (T*)nullptr;
		}
		else
			return (T*)nullptr;
	}


	/*
	字面量null_ptr，如同nullptr使用即可.
	*/
	constexpr struct null_ptr_t{
		template<typename T>
		[[nodiscard]]constexpr_as(get_null_ptr<remove_cvref<T>>())auto base_get()const noexcept{return get_null_ptr<remove_cvref<T>>();}
		template<typename T>
		[[nodiscard]]constexpr_as(base_get<T>())operator T*()const noexcept{return down_cast<T*>(base_get<T>());}
		//constexpr operator decltype(nullptr)()const noexcept{return nullptr;}//提醒接口设计者注意null_ptr的重载版本.
	}null_ptr{};

	template<typename T,typename U=decltype(*null_ptr.base_get<T>())>
	[[nodiscard]]auto operator==(T*a,null_ptr_t)noexcept{
		return null_ptr.base_get<T>()==static_cast<const remove_ref<U>*>(add_const(a));
	}
}
using null_ptr_n::can_t_use_default_null_ptr;
using null_ptr_n::force_use_default_null_ptr;
using null_ptr_n::null_ptr;
typedef decltype(null_ptr) null_ptr_t;
typedef decltype(nullptr) nullptr_t;

//file_end

