//equal.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//equal：值相等
template<typename T>
inline auto is_equal(T&&a,T&&b)noexcept_as(declvalue(T)==declvalue(T)){
	return a==b;
}
template<typename T>
inline auto is_not_equal(T&&a,T&&b)noexcept_as(declvalue(T)==declvalue(T)){
	return!is_equal(a,b);
}
//eq：同一对象
template<typename T>
inline auto is_eq(T&&a,T&&b)noexcept_as(&declvalue(T)==&declvalue(T)){
	return &a==&b;
}
template<typename T>
inline auto is_not_eq(T&&a,T&&b)noexcept_as(&declvalue(T)==&declvalue(T)){
	return!is_eq(a,b);
}

//file_end

