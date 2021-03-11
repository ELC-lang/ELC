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
	template<class U,class func_t,enable_flag>
	friend inline void map_all(func_t&a)noexcept_as(expr);
	#undef expr
public:
	constexpr can_map_all(){
		type_list.add(this);
	}
};
#define expr declvalue(func_t)(declvalue(T*))
template<class T,class func_t> requires(was_not_an_ill_form(expr) && type_info<T>.has_attribute(can_map_all))
inline void map_all(func_t&a)noexcept_as(expr){
	auto tmp=can_map_all<T>::type_list.begin();
	auto end=can_map_all<T>::type_list.end();
	while(tmp!=end){
		a(get_handle(tmp++));
	}
}
#undef expr

//file_end

