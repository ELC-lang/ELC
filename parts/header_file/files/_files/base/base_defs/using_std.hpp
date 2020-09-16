//using_std.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
using ::std::byte;
using ::std::size_t;

using ::std::forward;
using ::std::addressof;
using ::std::swap;
/*
如何不用中间变量swap两个值？
方法一：
a=a+b
b=a-b
a=a-b
方法二：
a^=b^=a^=b
道理我都懂，可是还是用中间变量快些
XD
更多方法欢迎补充
*/

template<class T>
using remove_cvref= ::std::remove_cvref_t<T>;
template<class T>
using remove_cv= ::std::remove_cv_t<T>;
// using ::std::move;//新move定义于 "../memory/lifetime/defs.hpp"

// using ::std::max;
// using ::std::min;
//新min与max定义于 "min_max.hpp"

//file_end

