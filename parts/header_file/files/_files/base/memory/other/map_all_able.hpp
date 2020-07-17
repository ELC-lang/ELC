//map_all_able.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
struct map_all_able:non_copyable,non_moveable,cons_t<map_all_able<T>>,attribute<T,map_all_able<T>>{
	typedef map_all_able<T> this_t;
private:
	typedef attribute<T,this_t> attribute;

	inline static list_t<this_t> type_list;

	constexpr T*get_handle(){
		return attribute::get_handle();
	}
public:
	constexpr map_all_able(){
		type_list.add(this);
	}
	#define tmp_expr declvalue(T_)(declvalue(T*))
	template<class T_,enable_if_not_ill_form(tmp_expr)>
	static void for_each(T_&a)noexcept_as(tmp_expr){
		auto tmp=type_list.begin();
		auto end=type_list.end();
		while(tmp!=end){
			a((tmp++)->get_handle());
		}
	}
	#undef tmp_expr
};
