//can_map_all.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct can_map_all:cons_t<can_map_all<T>>,is_common_attribute(can_map_all){
	typedef can_map_all<T> this_t;
private:
	inline static list_t<this_t> type_list;

	#define expr declvalue(func_t)(declvalue(U*))
	template<class U,class func_t>
	friend inline void map_all_helper(func_t&&a);
	#undef expr
public:
	constexpr can_map_all(){
		type_list.add(this);
	}
};
template<class T,class func_t>
inline void map_all_helper(func_t&&a){
	auto tmp=can_map_all<T>::type_list.begin();
	auto end=can_map_all<T>::type_list.end();
	while(tmp!=end){
		a(get_handle(tmp++));
	}
}
#define expr declvalue(func_t)(declvalue(T*))
template<class T,class func_t,enable_if(was_not_an_ill_form(expr))>
inline void map_all(func_t&&a)noexcept_as(expr){
	map_all_helper<T,func_t>(forward<func_t>(a));
}
#undef expr

//file_end

