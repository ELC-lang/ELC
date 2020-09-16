//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class gc_t{
	typedef gc_t this_t;
	typedef function_t<void()noexcept>gc_method_t;
	typedef function_t<bool()noexcept>success_identifier_t;

	base_stack_t<gc_method_t>_gc_methods;
	base_stack_t<success_identifier_t>_gc_success_identifiers;
	mutable flag _gc_running=flag(not_set);
public:
	inline void operator()()const noexcept{
		if(_gc_running)
			die_with(locale::str::gc_fail);
		_gc_running.set();
		if(_gc_methods.empty())
			die_with(locale::str::empty_gc_method);
		_gc_methods.for_each(invoke<const gc_method_t>._as(nothing));
		_gc_running.unset();
	}
	[[nodiscard]]inline bool success()const noexcept{
		bool success=_gc_success_identifiers.empty();
		_gc_success_identifiers.for_each(
			lambda_with_catch(&success)(const success_identifier_t&a)noexcept{
				if(not success)
					success=a();
			}
		);
		return success;
	}

	inline void add_gc_method(gc_method_t a)noexcept{
		_gc_methods.add(a);
	}
	inline void remove_gc_method(gc_method_t a)noexcept{
		_gc_methods.remove(a);
	}

	inline void add_gc_success_identifier(success_identifier_t a)noexcept{
		_gc_success_identifiers.add(a);
	}
	inline void remove_gc_success_identifier(success_identifier_t a)noexcept{
		_gc_success_identifiers.remove(a);
	}

	template<class T>
	inline void add_gc_method(type_info_t<T>)noexcept{
		add_gc_method(gc_method_of<T>);
		add_gc_success_identifier(gc_success_identifier_of<T>);
	}
	template<class T>
	inline void remove_gc_method(type_info_t<T>)noexcept{
		remove_gc_method(gc_method_of<T>);
		remove_gc_success_identifier(gc_success_identifier_of<T>);
	}
};

//file_end

