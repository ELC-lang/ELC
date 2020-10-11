//_undefs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif
//
#if defined(ELC_VOID_NAME)
	#define void the_void
#endif

#undef lazy_instantiation
#undef lazy_instantiation_name
#undef LIS_ID_t
#undef LIS_name
#undef LIS_IS_flag
#undef LIS_default_value

// #undef often_noexcept
#undef noexcept_as
#undef noexcept_as_auto
#undef constexpr_as
#undef constexpr_as_auto

#undef using_method_from_base_t
#undef using_method_from_value

// #undef floop
#undef re_declvalue
#undef re_decltype

#undef declvalue

#undef template_error
#undef template_warning

#undef has_attribute
#undef not_has_attribute

#undef def_common_attribute_with_nothing
#undef def_special_attribute_with_nothing
#undef common_attribute_t


#undef enable_if
#undef enabled_by_default
#undef disabled_by_default
#undef enable_if_not_ill_from
#undef enable_flag

#undef was_an_ill_form

#undef lambda
#undef lambda_with_catch

#undef MAGIC

#undef nothing

#undef elseif
#undef _big_than_
#undef _small_than_

#if defined(_MSC_VER)
	#undef not
	#undef and
	#undef or
#endif

#undef INTER_NAMESPACE
#undef BREAK_NAMESPACE

/*防写错宏*/
#undef szie
/*防写错宏*/
#undef form

//file_end

