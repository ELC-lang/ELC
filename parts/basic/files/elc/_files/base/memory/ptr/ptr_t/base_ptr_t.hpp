//_body.hpp
//at namespace elc::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//base ptr
//copy from old ELC & adepted.
template<class T>
struct same_ptr_p_t{
protected:
	mutable T*_to;
	explicit same_ptr_p_t(T*a)noexcept:_to(a){}
public:
	same_ptr_p_t(const same_ptr_p_t&)noexcept=default;
	[[nodiscard]]T*get()const noexcept{return _to;}
};
template<class T,typename ref_type>
struct same_ref_p_t:same_ptr_p_t<T>{
	using same_ptr_p_t<T>::_to;
	explicit same_ref_p_t(T*a)noexcept:same_ptr_p_t<T>(a){}
	explicit same_ref_p_t(const same_ptr_p_t<T>&a)noexcept:same_ptr_p_t<T>(a){}

	void swap(same_ref_p_t&a)noexcept{using std::swap;swap(_to,a._to);}

protected:
	static void cut_ref(T*a)noexcept{static_cast<ref_type*>(a)->cut_ref();}
	static void add_ref(T*a)noexcept{static_cast<ref_type*>(a)->add_ref();}

	void cut_ref()const noexcept{cut_ref(_to);}
	void add_ref()const noexcept{add_ref(_to);}
};

template<class T,typename ref_type>
struct ptr_t:same_ref_p_t<T,ref_type>{
	typedef same_ref_p_t<T,ref_type>base_t;
	typedef same_ref_p_t<T,ref_type>same_ref;
	typedef same_ptr_p_t<T>same_ptr;
	using same_ref::add_ref;
	using same_ref::cut_ref;
	using same_ref::swap;
	using same_ptr::_to;

	explicit ptr_t(T*a,special_init_t)noexcept:same_ref(a){}
	ptr_t(T*a)noexcept:same_ref(a){add_ref();}
	ptr_t(const same_ptr&a)noexcept:same_ref(a){add_ref();}
	ptr_t(ptr_t&a)noexcept:ptr_t((same_ptr&)a){}
	ptr_t(ptr_t&&a)noexcept:ptr_t((same_ptr&)a){}
	ptr_t(nullptr_t=nullptr)noexcept:ptr_t((T*)(null_ptr)){}
	~ptr_t()noexcept{cut_ref();}

	void reset(T*a)const noexcept{auto tmp=_to;add_ref(_to=a);cut_ref(tmp);}
	void reset(nullptr_t=nullptr)const noexcept{reset(null_ptr);}
protected:
	void check()const noexcept{
		if constexpr(::std::is_base_of_v<replace_able<T>,T>)
			if((replace_able<T>*)(_to)->replaced())
				reset((replace_able<T>*)(_to)->get_ptr());
	}
public:
	[[nodiscard]]T*get()const noexcept{
		check();
		return base_t::get();
	}
};

template<class T>
using convert_interface_t=ptr_t<::std::remove_cvref_t<T>,ref_able<::std::remove_cvref_t<T>>>;

namespace compare_n{
	template<class T,class T_>
	class compare_interface_t;
}
using compare_n::compare_interface_t;

template<class T,typename ref_type>
struct base_p_t:ptr_t<T,ref_type>,compare_interface_t<T,base_p_t<T,ref_type>>{
	typedef ptr_t<T,ref_type>base_t;
	typedef convert_interface_t<T>convert_interface;
	template<class T_>
	using compare_interface=compare_interface_t<T,T_>;
	using typename base_t::same_ref;
	using typename base_t::same_ptr;
	using base_t::reset;
	using base_t::get;
	using same_ref::swap;
	using same_ptr::_to;
	using base_t::base_t;

	base_p_t(base_p_t&a)noexcept:base_t(a){}
	base_p_t(base_p_t&&a)noexcept:base_t(move(a)){}

	T*operator->()const noexcept{return get();}
	[[nodiscard]]T&operator*()const noexcept{return*get();}
	[[nodiscard]]explicit operator bool()const noexcept{return pointer_to_bool(get());}
	[[nodiscard]]logical_bool operator!()const noexcept{return!pointer_to_bool(get());}
	[[nodiscard]]explicit operator T*()const noexcept{return get();}

	base_p_t&operator=(T*a)&noexcept{reset(a);return*this;}
	base_p_t&operator=(const same_ptr&a)&noexcept{reset(a.get());return*this;}
	base_p_t&operator=(base_p_t&a)&noexcept{reset(a.get());return*this;}
	base_p_t&operator=(same_ref&&a)&noexcept{swap(a);return*this;}
	base_p_t&operator=(nullptr_t)&noexcept{reset(null_ptr);return*this;}

	template<class T_,enable_if(::std::is_convertible_v<T_,convert_interface>)>
	base_p_t&operator=(T_&&a)&noexcept(::std::is_nothrow_convertible_v<T_,convert_interface>){
		reset(static_cast<convert_interface>(forward<T_>(a))._to);
		return*this;
	}


	template<class T_,enable_if(::std::is_convertible_v<T_,convert_interface>)>
	base_p_t(T_&&a)noexcept(::std::is_nothrow_convertible_v<T_,convert_interface>):base_p_t(static_cast<convert_interface>(forward<T_>(a))._to){}

private:
	static void special_destroy(T*a){//default destroy
		if constexpr(! ::std::is_convertible_v<T*,replace_able<T>*>)
			template_error("Please overload the function special_destroy in the namespace where this type is defined.");
		//(replace_able<T>*)(a)->replace(null_ptr);
		a->replace(null_ptr);//允许覆写replace方法
	}
	static constexpr class for_delete_t{
		T*_m;
	public:
		static void operator delete(void*a){
			special_destroy(reinterpret_cast<for_delete_t*>(a)->_m);
		}
		for_delete_t*operator()(T*a)noexcept{
			_m=a;
			return this;
		}
	}for_delete{};
public:
	operator for_delete_t*()noexcept{return for_delete(get());}
};

template<class T,typename ref_type>
inline void swap(base_p_t<T,ref_type>&a,base_p_t<T,ref_type>&b)noexcept{
	a.swap(b);
}

namespace ptr_n::compare_n{
	template<class T,class T_>
	class compare_interface_t:attribute<T_,compare_interface_t<T,T_>>{
		typedef compare_interface_t<T,T_> this_t;
		typedef convert_interface_t<T> convert_interface;
		typedef attribute<T_,this_t> attribute;

		static constexpr bool nothrow=::std::is_nothrow_convertible_v<T_,convert_interface>;

		[[nodiscard]]T*get()const noexcept(nothrow){
			return static_cast<const convert_interface>(*attribute::get_handle()).get();
		}
	};

	template<class T,class T_>
	T*get_p(const compare_interface_t<T,T_>&a)noexcept(compare_interface_t<T,T_>::nothrow)
	{return a.get();}
	template<class T>
	T*get_p(T*a)noexcept{return a;}
	null_ptr_t&get_p(nullptr_t)noexcept{return null_ptr;}

	#define tmp_expr pointer_equal(get_p(declvalue(const T)),get_p(declvalue(const T_)))
	template<class T,class T_,enable_if_not_ill_form(tmp_expr)>
	logical_bool operator==(const T&a,const T_&b)noexcept_as(tmp_expr){
		return pointer_equal(get_p(a),get_p(b));
	}
	template<class T,class T_,enable_if_not_ill_form(tmp_expr)>
	logical_bool operator!=(const T&a,const T_&b)noexcept_as(tmp_expr){
		return!pointer_equal(get_p(a),get_p(b));
	}
	#undef tmp_expr
}
