//base_ptr_t.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
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

	void swap_with(same_ref_p_t&a)noexcept{swap(_to,a._to);}

	static constexpr bool cut_nothrow=noexcept(declvalue(ref_type).cut_ref());
protected:
	static void cut_ref(T*a)noexcept(cut_nothrow){attribute_ptr_cast<ref_type>(a)->cut_ref();}
	static void add_ref(T*a)noexcept{attribute_ptr_cast<ref_type>(a)->add_ref();}

	void cut_ref()const noexcept_as(cut_ref(nullptr)){cut_ref(_to);}
	void add_ref()const noexcept{add_ref(_to);}
};

template<class T,typename ref_type>
inline void swap(same_ref_p_t<T,ref_type>&a,same_ref_p_t<T,ref_type>&b)noexcept{
	a.swap_with(b);
}

template<class T,typename ref_type,bool replace_check>
struct ptr_t:same_ref_p_t<T,ref_type>{
	typedef ptr_t<T,ref_type,replace_check>this_t;

	typedef same_ref_p_t<T,ref_type>base_t;
	typedef same_ref_p_t<T,ref_type>same_ref;
	typedef same_ptr_p_t<T>same_ptr;
	using base_t::cut_nothrow;
	using same_ref::add_ref;
	using same_ref::cut_ref;
	using same_ref::swap_with;
	using same_ptr::_to;

	explicit ptr_t(T*a,special_init_t)noexcept:same_ref(a){}
	ptr_t(T*a)noexcept:same_ref(a){add_ref();}
	ptr_t(const same_ptr&a)noexcept:same_ref(a){add_ref();}
	ptr_t(ptr_t&a)noexcept:ptr_t((same_ptr&)a){}
	ptr_t(ptr_t&&a)noexcept:ptr_t((same_ptr&)a){}
	ptr_t(nullptr_t=nullptr)noexcept:ptr_t((T*)(null_ptr)){}
	~ptr_t()noexcept(cut_nothrow){cut_ref();}

	static constexpr bool reset_nothrow=cut_nothrow;
	void reset(T*a)const noexcept(reset_nothrow){auto tmp=_to;add_ref(_to=a);cut_ref(tmp);}
	void reset(nullptr_t=nullptr)const noexcept(reset_nothrow){reset(null_ptr);}
public:
	static constexpr bool check_nothrow=(type_info<T>.not_has_attribute(replace_able))||reset_nothrow;
protected:
	inline void check()const noexcept(check_nothrow){
		if constexpr(replace_check&&type_info<T>.has_attribute(replace_able))
			if((replace_able<T>*)(_to)->replaced())
				reset((replace_able<T>*)(_to)->get_ptr());
	}
public:
	static constexpr bool get_nothrow=check_nothrow;
	[[nodiscard]]T*get()const noexcept(get_nothrow){
		check();
		return base_t::get();
	}
	[[nodiscard]]bool unique()const noexcept{return attribute_ptr_cast<ref_able>(get())->link_num()==1;}
	[[nodiscard]]explicit constexpr operator 
	::std::conditional_t<(replace_check&&type_info<T>.has_attribute(replace_able)),
						  unstable_hash_t,hash_t>()noexcept_as(hash(declvalue(this_t).get())){//注意：当T可replace时，同一ptr的hash可能变动
		return hash(get());
	}
};

template <class T,typename ref_type,bool replace_check>
[[nodiscard]]inline auto operator==(const ptr_t<T,ref_type,replace_check>&a,const T*b)
noexcept_as(pointer_equal(declvalue(const ptr_t<T,ref_type,replace_check>&).get(),declvalue(const T*))){
	return pointer_equal(a.get(),b);
}
template <class T,typename ref_type,bool replace_check>
[[nodiscard]]inline auto operator==(const T*a,const ptr_t<T,ref_type,replace_check>&b)
noexcept_as(pointer_equal(declvalue(const T*),declvalue(const ptr_t<T,ref_type,replace_check>&).get())){
	return pointer_equal(a,b.get());
}
template <class T,typename ref_type_1,bool replace_check_1,
				  typename ref_type_2,bool replace_check_2>
[[nodiscard]]inline auto operator==(const ptr_t<T,ref_type_1,replace_check_1>&a,
							 const ptr_t<T,ref_type_2,replace_check_2>&b)
noexcept_as(pointer_equal(
		declvalue(const ptr_t<T,ref_type_1,replace_check_1>&).get(),
		declvalue(const ptr_t<T,ref_type_2,replace_check_2>&).get())
){
	return pointer_equal(a.get(),b.get());
}

template<class T,typename ref_type,bool replace_check>
struct base_ptr_t:ptr_t<T,ref_type,replace_check>{
	static_assert(type_info<T>.base_on<ref_type>);
	typedef ptr_t<T,ref_type,replace_check>base_t;
	typedef base_ptr_t<T,ref_type,replace_check>this_t;
	using typename base_t::same_ref;
	using typename base_t::same_ptr;
	using base_t::reset;
	using base_t::get;
	using same_ref::swap_with;
	using same_ptr::_to;

	using base_t::get_nothrow;
	using base_t::reset_nothrow;

	using base_t::base_t;

	base_ptr_t(base_ptr_t&a)noexcept:base_t(a){}
	base_ptr_t(base_ptr_t&&a)noexcept:base_t(move(a)){}

	[[nodiscard]]T*operator->()const noexcept(get_nothrow){return get();}
	[[nodiscard]]T&operator*()const noexcept(get_nothrow){return*get();}
	[[nodiscard]]explicit operator bool()const noexcept(get_nothrow){return pointer_to_bool(get());}
	[[nodiscard]]logical_bool operator!()const noexcept(get_nothrow){return!pointer_to_bool(get());}
	[[nodiscard]]explicit operator T*()const noexcept(get_nothrow){return get();}

	base_ptr_t&operator=(T*a)&noexcept(reset_nothrow){reset(a);return*this;}
	base_ptr_t&operator=(const same_ptr&a)&noexcept(reset_nothrow&&get_nothrow){reset(a.get());return*this;}
	base_ptr_t&operator=(base_ptr_t&a)&noexcept(reset_nothrow&&get_nothrow){reset(a.get());return*this;}
	base_ptr_t&operator=(same_ref&&a)&noexcept{swap_with(a);return*this;}
	base_ptr_t&operator=(nullptr_t)&noexcept(reset_nothrow){reset(null_ptr);return*this;}

private:
	static constexpr class for_delete_t{
		T*_m;
		friend class this_t;
		for_delete_t*operator()(T*a)noexcept{
			_m=a;
			return this;
		}
	public:
		static void operator delete(void*a)noexcept_as(destroy(declvalue(T*))){
			destroy(reinterpret_cast<for_delete_t*>(a)->_m);
		}
	}for_delete{};
public:
	[[nodiscard]]operator for_delete_t*()noexcept(get_nothrow){return for_delete(get());}
	template<typename...Args,enable_if(invoke<T>.able<Args...>)>
	inline auto operator()(Args&&... rest)noexcept(invoke<T>.nothrow<Args...>){return(operator*())(forward<Args>(rest)...);}
};

//file_end

