//_defs.hpp
/*
未完成的elc解释器_share文件
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

//undefs at "_undefs.hpp"

//set ELC_WARNING_LEVEL default to 4(max)
#if !defined(ELC_WARNING_LEVEL)
	#define ELC_WARNING_LEVEL 4
#endif

#if defined(_MSC_VER)
	#pragma warning(push,ELC_WARNING_LEVEL)
	#pragma warning(disable:4099)//class与struct混用警告diss
	#pragma warning(disable:4584)//重复子类警告diss
	#pragma warning(disable:4250)//域控制继承警告diss
	#pragma warning(disable:26432)//不完全默认方法警告diss
	#pragma warning(disable:26435)//virtual override方法警告diss
	#pragma warning(disable:26481)//容器安利diss
	#pragma warning(disable:26446)//gsl::at安利diss
	#pragma warning(disable:26434)//方法覆盖警告diss
	#pragma warning(disable:26429)//gsl::not_null安利diss
	#pragma warning(disable:26471)//对void*进行static_cast安利diss
	#pragma warning(disable:26474)//隐式指针转换显式cast警告diss
	#pragma warning(disable:26473)//相同指针显式cast警告diss
	#pragma warning(disable:26456)//隐藏非虚运算符警告diss
	#pragma warning(disable:26485)//数组作指针警告diss
	#pragma warning(disable:26490)//reinterpret_cast警告diss
	#pragma warning(disable:26472)//static_cast窄幅转换警告diss
	#pragma warning(disable:26482)//非常数数组索引警告diss
	#pragma warning(disable:26493)//c_cast转换警告diss：虽然有用但是太烦了，爬
#endif
#if defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wall"//开启所有警告
	#pragma GCC diagnostic ignored "-Wmissing-template-keyword"//忽略模板警告
#endif
#if defined(_MSC_VER)
	//#define suppress_msvc_warning(...) __pragma(warning(suppress:__VA_ARGS__)) //不再使用因为其不确定的作用域
	#define disable_msvc_warning(...) __pragma(warning(disable:__VA_ARGS__))
	#define push_msvc_warning() __pragma(warning(push))
	#define pop_msvc_warning() __pragma(warning(pop))
	#define push_and_disable_msvc_warning(...) push_msvc_warning() disable_msvc_warning(__VA_ARGS__)
#else
	//#define suppress_msvc_warning(...)
	#define disable_msvc_warning(...)
	#define push_msvc_warning()
	#define pop_msvc_warning()
	#define push_and_disable_msvc_warning(...)
#endif
#if defined(_MSC_VER)
	//push pack settings
	#pragma pack(push)
	#pragma pack()
#endif
#if defined(_MSC_VER)
	//push and undef common macros
	#pragma push_macro("new")
	#undef new
	#pragma push_macro("delete")
	#undef delete
	#pragma push_macro("min")
	#undef min
	#pragma push_macro("max")
	#undef max
	#pragma push_macro("abs")
	#undef abs
	#pragma push_macro("export")
	#undef export
	#pragma push_macro("rand")
	#undef rand
	#pragma push_macro("sqrt")
	#undef sqrt
	#pragma push_macro("elc")
	#undef elc
#endif
//
#if defined(ELC_VOID_NAME)
	#undef void
#endif
//
#if defined(ELC_PUBLIC_ALL)
	#define protected public
	#define private public
#endif
//
#if defined(ELC_RESOLVE_GLOBAL_NAME_CONFLICTS)
	#undef rand
#endif

// #define often_noexcept
#define noexcept_as(...) noexcept(noexcept((__VA_ARGS__)))//dnmd C艹标准就是没有noexcept_as_auto
#define noexcept_as_auto MAGIC//哦现在有了
#define constexpr_as(...) MAGIC constexpr//( •̀ ω •́ )✌
#define constexpr_as_auto MAGIC MAGIC constexpr//✌( •̀ ω •́ )✌

#define using_method_from_base_t(name,...) \
template<class...Args> requires was_not_an_ill_form(declvalue(base_t).name(declvalue(Args)...))\
auto name(Args&&...rest)__VA_ARGS__ noexcept_as(base_t::name(declvalue(Args)...))\
{\
	return base_t::name(forward<Args>(rest)...);\
}\

#define using_method_from_value(name,value_name,...) \
template<class...Args> requires was_not_an_ill_form(re_declvalue(value_name).name(declvalue(Args)...))\
auto name(Args&&...rest)__VA_ARGS__ noexcept_as(re_declvalue(value_name).name(declvalue(Args)...))\
{\
	return value_name.name(forward<Args>(rest)...);\
}\

#define floop while(__builtin_is_my_dick_still_there())
#if defined(_MSC_VER) && _MSC_VER>=1935
	//https://developercommunity.visualstudio.com/t/10193638
	#define enable_adl(name) template<typename=void>void name()noexcept=delete
#else
	#define enable_adl(name) void name()noexcept=delete
#endif

#define re_declvalue(...) (declvalue(decltype(__VA_ARGS__)))
#define re_decltype(...) (decltype(declvalue(__VA_ARGS__)))

#define declvalue(...) (::std::declval<__VA_ARGS__>())

/*! 实例化到此将引发错误 */
#define template_error(reason) static_assert(template_error_helper<T>,reason)
/*! 实例化到此将引发警告 */
#define template_warning(reason) template_warning_helper<T>(reason)

//for type_info
#define type_info_of(...) base_type_info_t(typeid(__VA_ARGS__))
#define type_name_of(...) (type_info_of(__VA_ARGS__).get_name())

#define is_common_attribute(name) public attribute<T,name<T>>
#define is_special_attribute(name) public attribute<T,name>

#define has_attribute(...) template_name has_attribute_helper<__VA_ARGS__>()
#define not_has_attribute(...) template_name not_has_has_attribute_helper<__VA_ARGS__>()

//for float_size_t
#define float_size_of(...) (float_size_t{sizeof(__VA_ARGS__)})
//bitnum_of
#define bitnum_of(...) (sizeof(__VA_ARGS__)*CHAR_BIT)

//for attribute
#define def_common_attribute_with_nothing(name) \
template<typename T>\
class name{}\

#define def_special_attribute_with_nothing(name) \
class name{}\

#define common_attribute_t template<class>class
#define special_attribute_t class

/*!
若参数的布尔值为零，那么此模板不会实例化

为什么不用cpp20的constraints？
1.不能类内定义
2.不能作为模板参数
一句话总结：c艹标准会sb
*/
#define enable_if(...) class enable_state= ::std::enable_if_t<__VA_ARGS__>
/*! 默认参与模板候选 */
#define enabled_by_default class enable_state=void
/*! 默认不参与模板候选 */
#define disabled_by_default class enable_state= ::std::enable_if_t<false>
/*! 若参数为病式，那么此模板不会实例化 */
#define enable_if_not_ill_form(...) class enable_state= ::std::void_t<decltype(__VA_ARGS__)>
/*!用于模板声明 */
#define enable_flag class enable_state

#define was_an_ill_form(...) (!was_not_an_ill_form(__VA_ARGS__))
#define was_an_ill_form_with_parameter(...) (!was_not_an_ill_form_with_parameter(__VA_ARGS__))
#define was_not_an_ill_form(...) (bool(requires{__VA_ARGS__;}))
#define was_not_an_ill_form_and_noexcept(...) (bool(requires{__VA_ARGS__;{__VA_ARGS__}noexcept;}))
#define was_not_an_ill_form_with_parameter(...) (bool(requires __VA_ARGS__ ))

/*! 让lambda递归更加美观 */
#define recursive_lambda(...) lambda(auto&&lambda_RLSRRS,__VA_ARGS__)
/*! 让lambda递归更加美观 */
#define get_recursive_lambda_caller(name,...) \
exlambda(auto&&...Args)__VA_ARGS__ lambda_force_inline{\
	return name(name,Args...);\
}
/*! 让lambda定义更加美观 */
#define lambda []
/*! 让lambda定义更加美观 */
#define exlambda [&]
/*! 让lambda定义更加美观 */
#define lambda_with_catch(...) [__VA_ARGS__]
/*! 让lambda定义更加美观 */
#define self_recursion(...) lambda_RLSRRS(lambda_RLSRRS,__VA_ARGS__)
//recursive_lambda_self_referential_reserved_symbolname
#define lambda_RLSRRS _my_jb_super_sb_name_

#if defined(_MSC_VER) && !defined(__clang__)
	#define lambda_inline [[msvc::forceinline]]
	#define lambda_force_inline [[msvc::forceinline]]
#elif
	#define lambda_inline [[gnu::always_inline]]
	#define lambda_force_inline [[gnu::always_inline]]
#else
	#define lambda_inline
	#define lambda_force_inline
#endif

#define MAGIC//ahh,ko no tenno da!

/*! 装饰性语法糖 */
#define nothing

/*! aya风格语法糖 */
#define elseif else if
/*! aya风格语法糖 */
#define _big_than_ >
/*! aya风格语法糖 */
#define _small_than_ <

#if defined(_MSC_VER)
	#define not !
	#define and &&
	#define or ||
#endif

/*! 进入名称空间，并不是很美观 */
#define INTER_NAMESPACE(NAMESPACENAME) namespace NAMESPACENAME{
/*! 退出名称空间，并不是很美观 */
#define BREAK_NAMESPACE }

/*! template说明符 */
#define template_name template
/*! template说明符 */
#define type_name class

#if defined(DEBUG) || defined(_DEBUG)
	#define not_in_debug 0
	#define in_debug 1
#else
	#define not_in_debug 1
	#define in_debug 0
#endif

#define es U""
#define ec(ch) U ## ch

#if defined(ELC_TEST_ON)||defined(ELC_TEST_CHECK_MEMORY_LACK)
	#define ELC_TEST_EVENTNAME(name) stest_eventer _the_stest_eventer__(L""name)
#else
	#define ELC_TEST_EVENTNAME(name)
#endif

#define override_instance_struct \
protected:\
	virtual void abstract_method_unget_this()noexcept_as(instance_struct<this_t>::abstract_method_unget_this())override{instance_struct<this_t>::abstract_method_unget_this();}\
	virtual void* _abstract_method_copy_get_this()noexcept_as(instance_struct<this_t>::_abstract_method_copy_get_this())override{return instance_struct<this_t>::_abstract_method_copy_get_this();}\
	virtual void* _abstract_method_get_resize_this(size_t size)noexcept_as(instance_struct<this_t>::instance_struct<this_t>::_abstract_method_get_resize_this(size))override{return instance_struct<this_t>::_abstract_method_get_resize_this(size);}\
	virtual void* _abstract_method_get_forward_resize_this(size_t size)noexcept_as(instance_struct<this_t>::_abstract_method_get_forward_resize_this(size))override{return instance_struct<this_t>::_abstract_method_get_forward_resize_this(size);}\
	virtual size_t abstract_method_get_size_of_get_for_this()noexcept override{return instance_struct<this_t>::abstract_method_get_size_of_get_for_this();}

#if defined(_MSC_VER) && !defined(__clang__)
	#define distinctive __declspec(dllexport)
#else
	#define distinctive
#endif

#if defined(_MSC_VER) && !defined(__clang__)
	#define force_inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
	#define force_inline __attribute__((always_inline)) inline
#else
	#define force_inline inline
#endif

#if defined(_MSC_VER) && !defined(__clang__)
	#define with_no_vtable __declspec(novtable)
#else
	#define with_no_vtable
#endif

#define in_consteval (::std::is_constant_evaluated())

#define no_vtable_struct struct with_no_vtable
#define no_vtable_class class with_no_vtable

//#define get_elc_defs using namespace ::elc::defs

//file_end

