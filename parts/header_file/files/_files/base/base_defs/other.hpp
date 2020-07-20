//other.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class empty_type:elc_struct{};

template<typename T>
inline T&assign(T&a,const T b)noexcept_as(declvalue(T&)=declvalue(const T)){//为绕过条件内赋值时の警告而使用
	a=b;
	return a;
}

template<typename T>
inline void discard(T&&){}//fucking nodiscard.
template<typename T>
inline void just_a_use(T&&){}

template<typename T>
inline T const_default_value_of{};//?

template<typename T>
class maybe_fail_reference{
	T*_ref_to;
public:
	constexpr maybe_fail_reference(T&a):_ref_to(&a){}
	constexpr maybe_fail_reference(maybe_fail_reference&)=default;
	constexpr maybe_fail_reference(note::fail_t):_ref_to(nullptr){}

	[[nodiscard]]bool not_fail()noexcept{return _ref_to;}
	[[nodiscard]]bool fail()noexcept{return!not_fail();}
	T&get_ref()noexcept{return*_ref_to;}
};

template<typename T>
static void destroy(T*a)noexcept_as(declvalue(T).replace(null_ptr)){//default destroy
	if constexpr(type_info<T>.has_attribute<replace_able>){
		//(replace_able<T>*)(a)->replace(null_ptr);//×
		a->replace(null_ptr);//允许覆写replace方法√
	}elseif(type_info<T>.has_attribute<build_by_get_only>&&type_info<T>.has_attribute<never_in_array>){
		unget(a);
	}else{
		template_error("Please overload the function special_destroy in the namespace where this type is defined.");
	}
}

/*
typedef int64_t elint;
typedef double elfloat;
typedef uint64_t eluint;
static_assert(sizeof(elfloat)==sizeof(elint),"size error");
*/
