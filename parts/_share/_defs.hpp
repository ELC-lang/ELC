//_defs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4099)//class与struct混用警告diss
#endif
//
/*#if defined(ELC_TEST_ON)
	#define DO_TEST(namespace_)\
	namespace_::test();\
	namespace_::test_log_out();\
	namespace_::test_end();
#endif
*/

// #define use_as_template template<bool=true>
// #define often_noexcept
#define noexcept_as(...) noexcept(noexcept(__VA_ARGS__))
#define constexpr_as(...) MAGIC//we can't do that ——by std c++.

// #define floop while(1)
#define declvalue(...) ::std::declval<__VA_ARGS__>()

#define template_error(reason) static_assert(template_error_helper<T>,reason)
#define enable_if(...) class enable_state= ::std::enable_if_t<__VA_ARGS__>
#define enabled_by_default class enable_state=void
#define disabled_by_default class enable_state= ::std::enable_if_t<false>
#define enable_if_not_ill_form(...) class enable_state= ::std::void_t<decltype(__VA_ARGS__)>
#define enable_flag class enable_state

#define was_an_ill_form(...) MAGIC//we can't do that ——by std c++.

#define MAGIC

// #define INTER_NAMESPACE(NAMESPACENAME) namespace NAMESPACENAME{
// #define BREAK_NAMESPACE }
