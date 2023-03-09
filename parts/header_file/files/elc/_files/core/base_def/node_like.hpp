//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
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
[[nodiscard]]inline ptr make_list()noexcept;
template<typename T,typename...Args>
[[nodiscard]]inline ptr make_list(T&&first,Args&&...rest);

no_vtable_struct node_like:type_info_t<node_like>::template_name
	with_common_attribute<abstract_base,weak_ref_able,replace_able,ref_able>{
public:
	typedef node_like this_t;

	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept=0;

	[[nodiscard]]virtual value be_eval();
	[[nodiscard]]virtual value be_call(ptr);
protected:
	[[nodiscard]]virtual logical_bool equal_with(const_ptr)const noexcept=0;
	[[nodiscard]]virtual logical_bool eq_with(const_ptr a)const noexcept{return a.get()==this;}//不是a==this：ptr的opertaor==将调用在下方定义的pointer_equal，这会通过eq间接调用eq_with
	[[nodiscard]]virtual constexpr size_t equal_level()const noexcept{return 36;}
	[[nodiscard]]virtual constexpr size_t eq_level()const noexcept{return 36;}
public:
	node_like()noexcept=default;
	node_like(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }
	virtual ~node_like()=default;

	[[nodiscard]]virtual value arec(const value)=0;

	[[nodiscard]]value operator[](const value index);
	[[nodiscard]]value operator[](auto&&index){
		return operator[](as_value(index));
	}

	virtual void clear()noexcept=0;

	[[nodiscard]]virtual explicit operator hash_t()const noexcept{return hash((void*)this);}
	void destroy()noexcept{
		this->be_replace_as(null_ptr);
	}
	virtual void be_replace_as(ptr a)noexcept{
		this->clear();
		replace_able<this_t>::be_replace_as(a.get());
	}

	[[nodiscard]]virtual explicit operator logical_bool()const noexcept{return true;}
	[[nodiscard]]explicit operator bool()const noexcept{return(bool)this->operator logical_bool();}

	template<typename...Args>
	inline value operator()(Args&&...rest){return this->be_call(make_list(forward<Args>(rest)...));}

	[[nodiscard]]logical_bool eq(const_ptr a)const noexcept{
		const auto this_eqlv = this->eq_level();
		const auto arg_eqlv	 = a->eq_level();
		if(this_eqlv _small_than_ arg_eqlv)
			return a->eq_with(this);
		elseif(this_eqlv _big_than_ arg_eqlv)
			return this->eq_with(a);
		else
			return this->eq_with(a)&&a->eq_with(this);
	}
	[[nodiscard]]logical_bool equal(const_ptr a)const noexcept{
		const auto this_equlv = this->equal_level();
		const auto arg_equlv  = a->equal_level();
		if(this_equlv _small_than_ arg_equlv)
			return a->equal_with(this);
		elseif(this_equlv _big_than_ arg_equlv)
			return this->equal_with(a);
		else
			return this->equal_with(a)&&a->equal_with(this);
	}

	[[nodiscard]]ptr operator&()noexcept{return this;}
	[[nodiscard]]const_ptr operator&()const noexcept{return this;}
	[[nodiscard]]logical_bool operator==(const this_t&a)const noexcept{
		return a.equal(this);
	}
protected:
	friend void the_waiting_for_destroy(node_like* a)noexcept;
	virtual void waiting_for_destroy()noexcept{clear();}
};

//base中的类型功能适应器
[[nodiscard]]inline constexpr node_like*the_get_null_ptr(const node_like*)noexcept;//{return&void;}
[[nodiscard]]inline logical_bool the_pointer_to_bool(const node_like*a)noexcept{
	return a->operator logical_bool();
}
[[nodiscard]]inline logical_bool the_pointer_equal(const node_like*a,const node_like*b)noexcept{
	return a->eq(b);
}
[[nodiscard]]inline hash_t the_pointer_hash(const node_like*a)noexcept{
	return a->operator hash_t();
}
//ptr中的类型功能适应器
inline void the_waiting_for_destroy(node_like*a)noexcept{
	a->waiting_for_destroy();
}
inline void the_destroy_and_free(node_like*a)noexcept{
	unget(a);
}

//gc中的类型功能适应器
inline void the_destroy(node_like*a)noexcept{
	a->destroy();
}

//file_end

