//other.hpp
//at namespace elc::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
using ::std::size_t;
/*
typedef int64_t elint;
typedef double elfloat;
typedef uint64_t eluint;
static_assert(sizeof(elfloat)==sizeof(elint),"size error");
*/

template<typename T>
inline T&assign(T&a,const T b)noexcept_as(declvalue(T&)=declvalue(const T)){//为绕过条件内赋值时の警告而使用
	a=b;
	return a;
}

template<typename T>
inline void discard(T&&){}//fucking nodiscard.

using ::std::forward;
using ::std::swap;
//using ::std::move;//新move定义于base/memory/lifetime

