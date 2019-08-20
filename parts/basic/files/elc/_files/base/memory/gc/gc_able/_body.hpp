//_body.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct gc_able{};
/*
template<typename T,enable_if(::std::is_base_of_v<gc_able<T>,T>)>
struct gc_t{};
template<typename T,enable_if(::std::is_base_of_v<gc_able<T>,T>)>
inline gc_t<T> gc{};
*/
//i don't konw.
