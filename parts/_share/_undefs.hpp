//_undefs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*#if defined(ELC_TEST_ON)
	#undef DO_TEST
#endif
*/

// #undef use_as_template

// #undef often_noexcept
#undef noexcept_as
#undef constexpr_as

// #undef floop
#undef declvalue

#undef template_error
#undef enable_if
#undef enabled_by_default
#undef disabled_by_default
#undef enable_flag
#undef enable_if_not_ill_form

#undef was_an_ill_form

#undef lambda
#undef lambda_with_catch

#define MAGIC

#define nothing

// #undef BREAK_NAMESPACE
// #undef INTER_NAMESPACE

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
//