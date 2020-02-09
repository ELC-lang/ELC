//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF:草稿.
class gc_t{
	typedef function_t<void()noexcept>gc_method_t;
	typedef function_t<bool()noexcept>success_identifier_t;

	base_stack_t<gc_method_t>gc_methods;
	base_stack_t<success_identifier_t>gc_success_identifiers;
	bool success;

	inline void operator()()noexcept{
		if(gc_methods.empty())
			die_with(locale::str::empty_gc_method);
		gc_methods.for_each(invoke<gc_method_t>._as(nothing));
		
		bool success=gc_success_identifiers.empty();
		gc_success_identifiers.for_each(
			lambda_with_catch(&success)(success_identifier_t&a)noexcept{
				if(not success)
					success=a();
			}
		);
	}
	inline bool success()noexcept{
		return success;
	}

	inline void add_gc_method(gc_method_t a)noexcept{
		gc_methods.add(a);
	}
	inline void remove_gc_method(gc_method_t a)noexcept{
		gc_methods.remove(a);
	}
}
template<class T>
inline size_t count_for_success_identify;
template<class T>
inline void default_gc_method()noexcept{
	constexpr auto&info=type_info<T>;
	
	if constexpr(info.has_attribute<count_able>)
		count_for_success_identify<T> = the_number_of(info);
	
	if constexpr(info.has_attribute<can_map_all>&&info.has_attribute<can_shrink>){
		map_all<T>(lambda(T&a)noexcept{
			a.shrink();
		});
	}
	
	if constexpr(info.has_attribute<mark_able>&&info.has_attribute<can_map_all>&&info.has_attribute<have_root>){
		root_of<T>.map_and_mark();
		map_all<T>(lambda(T&a)noexcept{
			auto&b=static_cast<mark_able<T>&>(a)
			if(b.not_mark())
				destory(a);
			else
				b.unmark();
		});
	}
}
