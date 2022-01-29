//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename...Args>
[[nodiscard]]inline ptr make_list(Args&&...rest)noexcept;

struct node_like:type_info_t<node_like>::template_name
with_common_attribute<abstract_base,weak_ref_able,replace_able,ref_able>{
public:
	typedef node_like this_t;

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept=0;
protected:
	[[nodiscard]]virtual function_t<setter()> get_eval_of_this();
	[[nodiscard]]virtual function_t<setter(ptr)> get_call_of_this();

	[[nodiscard]]virtual logical_bool equal_with(const_ptr)const=0;
	[[nodiscard]]virtual logical_bool eq_with(const_ptr a)const{return a.get()==this;}//不是a==this：ptr的opertaor==将调用在下方定义的pointer_equal，这会通过eq间接调用eq_with
	[[nodiscard]]virtual constexpr size_t equal_level()const{return 36;}
	[[nodiscard]]virtual constexpr size_t eq_level()const{return 36;}
public:
	node_like()noexcept=default;
	node_like(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }
	virtual ~node_like()=default;

	[[nodiscard]]virtual setter arec(const setter)=0;

	[[nodiscard]]setter operator[](const setter index);
	template<typename T>
	[[nodiscard]]setter operator[](T&&index){
		return operator[](as_setter(forward<T>(index)));
	}

	virtual void clear()=0;

	[[nodiscard]]virtual explicit operator hash_t()const=0;
	virtual void destroy(){
		this->clear();
		this->be_replace_as(null_ptr);
	}
	virtual void be_replace_as(ptr a){
		replace_able<this_t>::be_replace_as(a.get());
	}

	[[nodiscard]]virtual explicit operator logical_bool()const{return true;}
	[[nodiscard]]explicit operator bool()const{return(bool)this->operator logical_bool();}

	template<typename...Args>
	inline setter operator()(Args&&...rest){return this->get_call_of_this()(make_list(forward<Args>(rest)...));}

	[[nodiscard]]logical_bool eq(const_ptr a)const{
		auto this_eqlv=this->eq_level();
		auto arg_eqlv=a->eq_level();
		if(this_eqlv _small_than_ arg_eqlv)
			return a->eq_with(this);
		elseif(this_eqlv _big_than_ arg_eqlv)
			return this->eq_with(a);
		else
			return this->eq_with(a)&&a->eq_with(this);
	}
	[[nodiscard]]logical_bool equal(const_ptr a)const{
		auto this_equlv=this->equal_level();
		auto arg_equlv=a->equal_level();
		if(this_equlv _small_than_ arg_equlv)
			return a->equal_with(this);
		elseif(this_equlv _big_than_ arg_equlv)
			return this->equal_with(a);
		else
			return this->equal_with(a)&&a->equal_with(this);
	}

	[[nodiscard]]ptr operator&(){return this;}
	[[nodiscard]]const_ptr operator&()const{return this;}
	[[nodiscard]]logical_bool operator==(const this_t&a)const{
		return a.equal(this);
	}
protected:
	friend void the_waiting_for_destroy(node_like* a)noexcept;
	virtual void waiting_for_destroy()noexcept=0;
};

//base中的类型功能适应器
[[nodiscard]]constexpr node_like*the_get_null_ptr(const node_like*)noexcept;//{return&void;}
[[nodiscard]]logical_bool the_pointer_to_bool(const node_like*a)noexcept{
	return a->operator logical_bool();
}
[[nodiscard]]logical_bool the_pointer_equal(const node_like*a,const node_like*b)noexcept{
	return a->eq(b);
}
[[nodiscard]]hash_t the_pointer_hash(const node_like*a)noexcept{
	return a->operator hash_t();
}
//ptr中的类型功能适应器
void the_waiting_for_destroy(node_like*a)noexcept{
	a->waiting_for_destroy();
}
void the_destroy_and_free(node_like*a)noexcept{
	unget(a);
}

//gc中的类型功能适应器
void the_destroy(node_like*a)noexcept{
	a->destroy();
}

//file_end

