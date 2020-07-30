//using_std.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
using ::std::size_t;

using ::std::forward;
using ::std::swap;
template<class T>
using remove_cvref= ::std::remove_cvref_t<T>;
template<class T>
using remove_cv= ::std::remove_cv_t<T>;
// using ::std::move;//新move定义于 "../memory/lifetime/defs.hpp"

// using ::std::max;
// using ::std::min;
//新min与max定义于base/base_defs/min_max

//file_end

