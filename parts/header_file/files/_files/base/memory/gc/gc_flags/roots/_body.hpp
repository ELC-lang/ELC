//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF:草稿
//#error
struct have_root:helper_struct{};

template<typename T>
class root_of;

template<typename T>
class roots_t{
	typedef roots_t<T>this_t;
	typedef root_of<T>root_t;
	typedef root_of<T>*value_t;
	typedef base_stack_t<value_t> base_t;

	base_stack_t<value_t> _roots;
public:
	constexpr roots_t()noexcept=default;
	~roots_t()noexcept=default;

	using_method_from_base_t(map);
	using_method_from_base_t(add);
	using_method_from_base_t(remove);
	using_method_from_base_t(for_each);

	void map_and_mark()noexcept_as(declvalue(value_t)->map_and_mark()){
		base_t::for_each(
			lambda(value_t a)noexcept_as(declvalue(value_t)->map_and_mark())
			{
				a->map_and_mark();
			}
		);
	}
};
template<typename T>
inline roots_t<T>roots_of;

template<typename T>
class root_of{
	typedef root_of<T>this_t;
public:
	root_of()noexcept_as(roots_of<T>.add(declvalue(this_t*))){
		roots_of<T>.add(this);
	}
	virtual T& get()noexcept=0;
	virtual void map_and_mark()noexcept=0;
};
template<typename T>
T& root_of<T>::get()noexcept{return const_default_value_of<T>;}
template<typename T>
void root_of<T>::map_and_mark()noexcept{}

//file_end

