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
#pragma warning(disable:26812)//enum class安利diss.
#pragma warning(disable:4584)//重复子类警告diss
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
#define noexcept_as(...) noexcept(noexcept(__VA_ARGS__))//dnmd C艹标准就是没有noexcept_as_auto
#define constexpr_as(...) MAGIC//we can't do that ——by std c++.
#define is_function(...) ::std::is_function_v<__VA_ARGS__>

#define using_method_from_base_t(name) \
template<class...Args,enable_if_not_ill_form(declvalue(base_t).name(declvalue(Args)...))>\
auto name(Args&&...rest)noexcept_as(base_t::name(declvalue(Args)...))\
{\
	return base_t::name(forward<Args>(rest)...);\
}\


// #define floop while(1)
#define declvalue(...) (::std::declval<__VA_ARGS__>())

#define template_error(reason) static_assert(template_error_helper<T>,reason)
#define enable_if(...) class enable_state= ::std::enable_if_t<__VA_ARGS__>
#define enabled_by_default class enable_state=void
#define disabled_by_default class enable_state= ::std::enable_if_t<false>
#define enable_if_not_ill_form(...) class enable_state= ::std::void_t<decltype(__VA_ARGS__)>
#define enable_flag class enable_state

#define was_an_ill_form(...) MAGIC//we can't do that ——by std c++.

#define lambda []
#define lambda_with_catch(...) [__VA_ARGS__]

#define MAGIC

#define nothing

#define elseif else if
#define _big_than_  >
#define _small_than_  <

// #define INTER_NAMESPACE(NAMESPACENAME) namespace NAMESPACENAME{
// #define BREAK_NAMESPACE }

//file_end

