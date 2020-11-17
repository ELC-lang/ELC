//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
lazy_instantiation struct LIS_name(probability);
lazy_instantiation struct LIS_name(setter);

lazy_instantiation struct LIS_name(node_like):type_info_t<LIS_ID_t(node_like)>::template
with_common_attribute<abstract_base,ref_able,weak_ref_able,replace_able>{
public:
	typedef comn_ptr_t<LIS_ID_t(node_like)>ptr;
	using_LIS_name(setter)setter;
	using_LIS_name(probability)probability;
	typedef LIS_ID_t(node_like)this_t;

	[[nodiscard]]virtual const base_type_info_t& get_type_info()const noexcept=0;
protected:
	[[nodiscard]]virtual function_t<probability()> get_eval_of_this()const{
		return lambda_with_catch(this)()noexcept{return this;};
	}
	[[nodiscard]]virtual function_t<probability(ptr)> get_call_of_this()const{
		return lambda_with_catch(this)(ptr)noexcept{return this;};
	}

	[[nodiscard]]virtual logical_bool equal_with(ptr)const=0;
	[[nodiscard]]virtual logical_bool eq_with(ptr a)const{return a.get()==this;}//不是a==this：ptr的opertaor==将调用在下方定义的pointer_equal，这会通过eq间接调用eq_with
	[[nodiscard]]virtual constexpr size_t equal_level()const{return 0;}
	[[nodiscard]]virtual constexpr size_t eq_level()const{return 0;}
public:
	constexpr LIS_name(node_like)()noexcept=default;
	constexpr LIS_name(node_like)(never_ref_num_zero_t)noexcept:ref_able<this_t>(never_ref_num_zero){}
	constexpr virtual ~LIS_name(node_like)()=default;

	[[nodiscard]]virtual setter arec(const probability)=0;

	[[nodiscard]]probability operator[](const probability){
		
	}
	[[nodiscard]]const probability operator[](const probability)const=0;

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
	[[nodiscard]]explicit operator bool()const{return this->operator logical_bool();}

	template<typename...Args>
	inline setter operator()(Args&&...rest);//{return this->get_call_of_this()(make_arg_list(forward<Args>(rest)...));}

	[[nodiscard]]logical_bool eq(ptr a)const{
		if(this->eq_level()==a->eq_level())
			return this->eq_with(a)&&a->eq_with(this);
		elseif(this->eq_level() _small_than_ a->eq_level())
			return a->eq_with(this);
		elseif(this->eq_level() _big_than_ a->eq_level())
			return this->eq_with(a);
	}
	[[nodiscard]]logical_bool equal(ptr a)const{
		if(this->equal_level()==a->equal_level())
			return this->equal_with(a)&&a->equal_with(this);
		elseif(this->equal_level() _small_than_ a->equal_level())
			return a->equal_with(this);
		elseif(this->equal_level() _big_than_ a->equal_level())
			return this->equal_with(a);
	}

	[[nodiscard]]ptr operator&(){return this;}
	[[nodiscard]]const_ptr operator&()const{return this;}
	[[nodiscard]]logical_bool operator==(const this_t&a)const{
		return a.equal(*this);
	}
};
lazy_instantiation_name(node_like);

//base中的类型功能适应器
[[nodiscard]]logical_bool pointer_to_bool(const node_like*a)noexcept{
	return a->operator logical_bool();
}
[[nodiscard]]logical_bool pointer_equal(const node_like*a,const node_like*b)noexcept{
	return a->eq(b);
}
[[nodiscard]]constexpr node_like*get_null_ptr(type_info_t<node_like>)noexcept;//{return&nil;}
[[nodiscard]]hash_t pointer_hash(const node_like*a)noexcept{
	return a->operator hash_t();
}

//file_end

