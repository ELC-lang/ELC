//cast.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
inline const T add_const(T a){return a;}
template<typename T>
inline T remove_const(const T a){return a;}
template<typename T>
inline const T*add_const(T*a){return a;}
template<typename T>
inline T*remove_const(const T*a){return const_cast<T*>(a);}

//file_end

