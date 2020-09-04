//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace default_gc_for_type{
	template<class T,enable_if(type_info<T>.has_attribute(count_able))>
	inline size_t count_for_success_identify;

	template<class T,enable_if(
								(
									type_info<T>.has_attribute(mark_able_for_gc) &&
									type_info<T>.has_attribute(have_root)
								)&&(
										info.has_attribute(can_map_all)
									&&(
											type_info<T>.has_attribute(can_shrink)
										||(
											type_info<T>.has_attribute(mark_able_for_gc) &&
											type_info<T>.has_attribute(have_root)
										)
									)
								)
							)>
	inline void default_gc_method()noexcept{
		constexpr auto&info=type_info<T>;
		//
		if constexpr(info.has_attribute(count_able))
			count_for_success_identify<T> = the_number_of(info);
		//
		if constexpr(info.has_attribute(can_map_all)){
			if constexpr(info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root))
				root_of<T>.map_and_mark();
			if constexpr(info.has_attribute(can_shrink) || (info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root)))
				map_all<T>(
					lambda(T&a)noexcept{
						if constexpr(info.has_attribute(can_shrink))
							a.shrink();
						if constexpr(info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root)){
							auto&b=static_cast<mark_able_for_gc<T>&>(a)
							if(b.not_mark())
								destory(a);
							else
								b.unmark();
						}
					}
				);
		}
	}
	//
	template<class T,enable_if(type_info<T>.has_attribute(count_able))>
	[[nodiscard]]inline bool default_gc_success_identifier()noexcept{
		constexpr auto&info=type_info<T>;
		if constexpr(info.has_attribute(count_able)){//重复判定，勿删，方便日后扩展
			return count_for_success_identify<T> _big_than_ the_number_of(info);
		}
	}
	template<class T>
	auto have_default_gc_success_identifier_helper(int) -> decltype(
		default_gc_success_identifier<T>(),
		::std::true_type{});
	template<class>
	auto have_default_gc_success_identifier_helper(...) -> ::std::false_type;

	template<class T>
	inline constexpr bool have_default_gc_success_identifier = decltype(have_default_gc_success_identifier_helper<T>(0))::value;
}
using default_gc_for_type::have_default_gc_success_identifier;
template<class T,enable_if_not_ill_form(default_gc_for_type::default_gc_method<T>())>
inline void gc_method_of()noexcept{
	default_gc_for_type::default_gc_method<T>()
}
template<class T,enable_if_not_ill_form(default_gc_for_type::default_gc_success_identifier<T>())>
[[nodiscard]]inline bool gc_success_identifier_of()noexcept{
	return default_gc_for_type::default_gc_success_identifier<T>();
}

//file_end

