//binary_node_t.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct binary_node_base_t:node_like{
	typedef binary_node_base_t this_t;

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<this_t>;}
	[[nodiscard]]virtual base_type_info_t get_additional_type_info()const noexcept=0;
protected:
	[[noreturn]] virtual void throw_self_ptr()const=0;
	template<typename T>
	friend[[nodiscard]]inline maybe_fail_reference<T> maybe_fail_use_by_ref_as(ptr p)noexcept;
	template<typename T>
	friend[[nodiscard]]inline maybe_fail_reference<T> maybe_fail_use_by_ref_as(ptr p)noexcept;
	template<typename T>
	friend[[nodiscard]]inline const T& const_use_by_ref_as(const_ptr p)noexcept;
};
template<typename T>
struct binary_node_t:binary_node_base_t,instance_struct<binary_node_t<T>>{
	typedef binary_node_t<T> this_t;
	typedef binary_node_base_t base_t;

	T _m;

	binary_node_t(const T&a)noexcept:_m(a){}
	binary_node_t(T&&a)noexcept:_m(a){}

	[[nodiscard]]virtual base_type_info_t get_additional_type_info()const noexcept override{return type_info<this_t>;}
private:
	static constexpr bool hash_nothrow_helper=hash.able<T>?
											  hash.nothrow<T>:1;
public:
	[[nodiscard]]virtual explicit operator hash_t()const noexcept(hash_nothrow_helper)override{
		if constexpr(hash.able<T>)
			return hash(_m);
		else
			#if defined(_MSC_VER)
				[[gsl::suppress(f.6)]]
			#endif
			return node_like::operator hash_t();
	}
protected:
	[[noreturn]] virtual void throw_self_ptr()const override{throw&_m;}
	[[nodiscard]]virtual logical_bool eq_with(const_ptr a)const noexcept override{
		using defs::equal;//貌似msvc在这里有bug
		if constexpr(equal.able<T>){
			if(a->get_type_info() != this->get_type_info())
				return false;
			const base_t*base_p=down_cast<const base_t*>(a.get());
			if(base_p->get_type_info() != this->get_type_info())
				return false;
			const this_t*p=down_cast<const this_t*>(base_p);
			return _m==p->_m;
		}
		else
			return node_like::eq_with(a);
	}
	[[nodiscard]]virtual logical_bool equal_with(const_ptr a)const noexcept override{
		return eq_with(a);
	}
private:
	static constexpr bool arec_nothrow_helper=was_not_an_ill_form(arec_as_value(_m,declvalue(const value&)))?
											  was_not_an_ill_form_and_noexcept(arec_as_value(_m,declvalue(const value&))):1;
public:
	[[nodiscard]]virtual value arec(const value index)noexcept(arec_nothrow_helper)override{
		if constexpr(was_not_an_ill_form(arec_as_value(_m,index)))
			return arec_as_value(_m,index);
		else
			return as_value(this);
	}
private:
	static constexpr bool clear_nothrow_helper= was_not_an_ill_form(declvalue(T).clear())?
												was_not_an_ill_form_and_noexcept(declvalue(T).clear()):
												re_construct.nothrow<T>;
public:
	virtual void clear()noexcept(clear_nothrow_helper)override{
		if constexpr(was_not_an_ill_form(declvalue(T).clear()))
			_m.clear();
		else
			re_construct(&_m);
	}
};

template<typename T>
[[nodiscard]]inline bool was_an(const_ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	if(p->get_type_info() == type_info<binary_node_base_t>){
		auto base_p = down_cast<const binary_node_base_t*>(p.get());
		if(base_p->get_additional_type_info() == type_info<target_node_t>)
			return true;
		elseif constexpr(::std::is_class_v<T> && !::std::is_final_v<T>){
			try{
				base_p->throw_self_ptr();
			}
			catch(T*){
				return true;
			}
			catch(void*){}
		}
	}
	return false;
}
template<typename T>
[[nodiscard]]inline maybe_fail_reference<T> maybe_fail_use_by_ref_as(ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	if(p->get_type_info() == type_info<binary_node_base_t>){
		auto base_p = down_cast<binary_node_base_t*>(p.get());
		if(base_p->get_additional_type_info() == type_info<target_node_t>)
			return down_cast<target_node_t*>(base_p)->_m;
		elseif constexpr(::std::is_class_v<T> && !::std::is_final_v<T>){
			try{
				base_p->throw_self_ptr();
			}
			catch(T*aret){
				return*aret;
			}
			catch(void*){}
		}
	}
	return note::fail;
}
template<typename T>
[[nodiscard]]inline T& use_by_ref_as(ptr p)noexcept{
	return maybe_fail_use_by_ref_as<T>(p).get_ref();
}
template<typename T>
[[nodiscard]]inline const T& const_use_by_ref_as(const_ptr p)noexcept{
	typedef binary_node_t<T> target_node_t;
	if(p->get_type_info() == type_info<binary_node_base_t>){
		auto base_p = down_cast<const binary_node_base_t*>(p.get());
		if(base_p->get_additional_type_info() == type_info<target_node_t>)
			return down_cast<const target_node_t*>(base_p)->_m;
		elseif constexpr(::std::is_class_v<T> && !::std::is_final_v<T>){
			try{
				base_p->throw_self_ptr();
			}
			catch(const T*aret){
				return*aret;
			}
			catch(const void*){}
		}
	}
	return const_default_value_of<T>;
}
template<typename T>
[[nodiscard]]inline T use_as(const_ptr p)noexcept{
	return const_use_by_ref_as<T>(p);
}
template<typename T>
[[nodiscard]]inline const T& use_by_ref_as(const_ptr p)noexcept{
	return const_use_by_ref_as<T>(p);
}
template<typename T>
[[nodiscard]]inline const T& use_by_ref_as(auto&&a)noexcept{
	return use_by_ref_as<T>(as_ptr(a));
}
template<typename T>
[[nodiscard]]inline ptr make_binary_node_from(T a)noexcept{
	return get<binary_node_t<T>>(move(a));
}
template<typename T>
inline distinctive map_t<ptr,T> long_term_binary_node_storager;
template<typename T>
[[nodiscard]]inline ptr make_long_term_binary_node_from(T a)noexcept{
	ptr& to = long_term_binary_node_storager<T>[a];
	if(!bool(to) || const_use_by_ref_as<T>(to)!=a)
		to = make_binary_node_from<T>(move(a));
	return to;
}

//file_end

