//defs.hpp
//at namespace elc::defs::memory::gc
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T,enable_flag>
inline void gc_method_of()noexcept;
template<class T,enable_flag>
[[nodiscard]]inline bool gc_success_identifier_of()noexcept;

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
		//
		success=gc_success_identifiers.empty();
		gc_success_identifiers.for_each(
			lambda_with_catch(&success)(success_identifier_t&a)noexcept{
				if(not success)
					success=a();
			}
		);
	}
	[[nodiscard]]inline bool success()noexcept{
		return success;
	}

	inline void add_gc_method(gc_method_t a)noexcept{
		gc_methods.add(a);
	}
	inline void remove_gc_method(gc_method_t a)noexcept{
		gc_methods.remove(a);
	}

	inline void add_gc_success_identifier(success_identifier_t a)noexcept{
		gc_success_identifiers.add(a);
	}
	inline void remove_gc_success_identifier(success_identifier_t a)noexcept{
		gc_success_identifiers.remove(a);
	}

	template<class T>
	inline void add_gc_method(type_info_t<T>)noexcept{
		add_gc_method(gc_method_of<T>);
		if constexpr(is_function(gc_success_identifier_of<T>))
			add_gc_success_identifier(gc_success_identifier_of<T>);
	}
	template<class T>
	inline void remove_gc_method(type_info_t<T>)noexcept{
		remove_gc_method(gc_method_of<T>);
		if constexpr(is_function(gc_success_identifier_of<T>))
			remove_gc_success_identifier(gc_success_identifier_of<T>);
	}
}
//默认行为
namespace default_gc_for_type{
	template<class T,enable_if(type_info<T>.has_attribute<count_able>)>
	inline size_t count_for_success_identify;

	template<class T,enable_if(
								(
									type_info<T>.has_attribute<mark_able_for_gc> &&
									type_info<T>.has_attribute<have_root_for_gc>
								)||(
									(
										info.has_attribute<can_map_all>
									)&&(
										(
											type_info<T>.has_attribute<can_shrink>
										)||(
											type_info<T>.has_attribute<mark_able_for_gc> &&
											type_info<T>.has_attribute<have_root_for_gc>
										)
									)
								)
							)>
	inline void default_gc_method()noexcept{
		constexpr auto&info=type_info<T>;
		//
		if constexpr(info.has_attribute<count_able>)
			count_for_success_identify<T> = the_number_of(info);
		//
		if constexpr(info.has_attribute<can_map_all>){
			if constexpr(info.has_attribute<can_shrink> || (info.has_attribute<mark_able_for_gc>&&info.has_attribute<have_root_for_gc>))
				map_all<T>(lambda(T&a)noexcept{
					if constexpr(info.has_attribute<can_shrink>)
						a.shrink();
					if constexpr(info.has_attribute<mark_able_for_gc>&&info.has_attribute<have_root_for_gc>){
						auto&b=static_cast<mark_able_for_gc<T>&>(a)
						if(b.not_mark())
							destory(a);
						else
							b.unmark();
					}
				});
			elseif constexpr(info.has_attribute<mark_able_for_gc>&&info.has_attribute<have_root_for_gc>)
				root_of<T>.map_and_mark();
		}
	}
	//
	template<class T,enable_if(type_info<T>.has_attribute<count_able>)>
	[[nodiscard]]inline bool default_gc_success_identifier()noexcept{
		constexpr auto&info=type_info<T>;
		if constexpr(info.has_attribute<count_able>){
			return count_for_success_identify<T> _big_than_ the_number_of(info);
		}
	}
}
//默认行为定义
template<class T,enable_if_not_ill_form(default_gc_for_type::default_gc_method<T>())>
inline void gc_method_of()noexcept{
	default_gc_for_type::default_gc_method<T>()
}
template<class T,enable_if_not_ill_form(default_gc_for_type::default_gc_success_identifier<T>())>
[[nodiscard]]inline bool gc_success_identifier_of()noexcept{
	return default_gc_for_type::default_gc_success_identifier<T>();
}

inline gc_t gc;
