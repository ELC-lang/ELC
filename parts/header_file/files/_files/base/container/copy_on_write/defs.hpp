//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
class copy_on_write_t{
	typedef T base_t_w;
	struct data_t:base_t_w,ref_able<data_t>,build_by_get_only,never_in_array<data_t>,force_use_default_null_ptr{
		typedef base_t_w base_t;
		using base_t::base_t;

		data_t(const data_t&a)noexcept(construct<base_t>.nothrow<const base_t&>):base_t(a){}
	};
	typedef comn_ptr_t<data_t>ptr_t;
	typedef copy_on_write_t<T>this_t;
protected:
	ptr_t _m;

	static constexpr bool check_nothrow=noexcept(declvalue(ptr_t&)=get<data_t>(*declvalue(ptr_t)));
	void copy_check()noexcept(check_nothrow){
		if(!_m.unique())
			_m=get<data_t>(*_m);
	}
	base_t_w&get()const noexcept{
		return static_cast<base_t_w&>(*_m);
	}
public:
	copy_on_write_t()noexcept:_m(get<data_t>()){}
	template<class...Args,enable_if(get<base_t_w>.able<Args...>)>
	copy_on_write_t(Args&&...rest)noexcept(get<base_t_w>.nothrow<Args...>){
		_m=get<T>(forward<Args>(rest)...);
	}
	~copy_on_write_t()noexcept_as(declvalue(ptr_t).~ptr_t())=default;

	copy_on_write_t(const this_t&)noexcept=default;
	copy_on_write_t(this_t&&)noexcept=default;
	this_t&operator=(const this_t&)noexcept(ptr_t::reset_nothrow)=default;
	this_t&operator=(this_t&&)noexcept(ptr_t::reset_nothrow)=default;

	void swap_with(this_t&a)noexcept{_m.swap_with(a._m);}
	void swap_with(base_t_w&a)noexcept(check_nothrow){
		copy_check();
		a.swap_with(*_m);
	}
	template<class assign_t,enable_if_not_ill_form(declvalue(base_t_w)=declvalue(assign_t))>
	this_t&operator=(assign_t&&a)&noexcept(check_nothrow&noexcept(declvalue(base_t_w)=declvalue(assign_t))){
		copy_check();
		get()=a;
		return*this;
	}
	operator base_t_w&()noexcept(check_nothrow){
		copy_check();
		return*_m;
	}
	operator const base_t_w&()const noexcept{
		return*_m;
	}
	[[nodiscard]]explicit operator hash_t()const noexcept{return hash(*_m);}
	[[nodiscard]]bool operator==(const this_t&a)const noexcept(unget.nothrow<T>){
		if(get()==a.get()){
			const_cast<this_t>(this)->_data=a._data;//合并数据块以减少空间损耗
			return true;
		}else
			return false;
	}
}

//file_end

