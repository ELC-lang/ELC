//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
namespace default_gc_for_type{
	template<class T,enable_if(type_info<T>.has_attribute(count_able))>
	distinctive inline size_t count_for_success_identify;

	template<class T>
	constexpr bool use_default_gc_able =
	(
		type_info<T>.has_attribute(mark_able_for_gc) &&
		type_info<T>.has_attribute(have_root)
	)&&(
			type_info<T>.has_attribute(can_map_all)
		&&(
				type_info<T>.has_attribute(can_shrink)
			||(
				type_info<T>.has_attribute(mark_able_for_gc) &&
				type_info<T>.has_attribute(have_root)
			)
		)
	);

	enable_adl(destory_by_gc);
	template<class T>
	void destory_by_gc(T*){
		template_error("this function should not be instantiated,please overload the function destory_by_gc in the namespace where this type is defined.");
	}

	template<class T> requires use_default_gc_able<T>
	inline void default_gc_method()noexcept{
		constexpr auto&info=type_info<T>;
		//
		if constexpr(info.has_attribute(count_able))
			count_for_success_identify<T> = the_number_of(info);
		//
		if constexpr(info.has_attribute(can_map_all)){
			if constexpr(info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root))
				roots_of<T>.map_and_mark();
			if constexpr(info.has_attribute(can_shrink) || (info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root)))
				map_all<T>(
					lambda(T*a)noexcept{
						if constexpr(info.has_attribute(can_shrink))
							a->shrink();
						if constexpr(info.has_attribute(mark_able_for_gc)&&info.has_attribute(have_root)){
							auto& b=attribute_cast<mark_able_for_gc>(*a);
							if(b.was_marked())
								b.unmark();
							else
								destory_by_gc(a);
						}
					}
				);
		}
	}
	//
	template<class T>
	constexpr bool use_default_gc_success_identifier_able = type_info<T>.has_attribute(count_able);

	template<class T> requires use_default_gc_success_identifier_able<T>
	[[nodiscard]]inline bool default_gc_success_identifier()noexcept{
		constexpr auto&info=type_info<T>;
		if constexpr(info.has_attribute(count_able)){//重复判定，勿删，方便日后扩展
			return count_for_success_identify<T> _big_than_ the_number_of(info);
		}
	}
}
template<class T,enable_if(default_gc_for_type::use_default_gc_able<T>)>
inline void gc_method_of()noexcept{
	default_gc_for_type::default_gc_method<T>();
}
template<class T,enable_if(default_gc_for_type::use_default_gc_success_identifier_able<T>)>
[[nodiscard]]inline bool gc_success_identifier_of()noexcept{
	return default_gc_for_type::default_gc_success_identifier<T>();
}

//file_end

