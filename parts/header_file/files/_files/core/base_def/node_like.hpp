//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
lazy_instantiation struct LIS_name(setter);
lazy_instantiation struct LIS_name(node_like);
lazy_instantiation_name(node_like);
template<typename...Args>
node_like*node_list(Args&&...);

lazy_instantiation struct LIS_name(node_like):
ref_able<LIS_ID_t(node_like)>,weak_ref_able<LIS_ID_t(node_like)>,replace_able<LIS_ID_t(node_like)>{
public:
	typedef comn_ptr<LIS_ID_t(node_like)>ptr;
	typedef LIS_ID_t(setter)setter;
	typedef LIS_ID_t(node_like)this_t;
protected:
	[[nodiscard]]virtual const base_type_info_t& get_type_info()const noexcept=0;

	[[nodiscard]]virtual function_t<setter()> get_eval_of_this()const{
		return lambda_with_catch(this)()noexcept{return setter(this);};
	}
	[[nodiscard]]virtual function_t<setter(ptr)> get_call_of_this()const{
		return lambda_with_catch(this)(ptr)noexcept{return setter(this);};
	}

	[[nodiscard]]virtual logical_bool equal_with(ptr)const=0;
	[[nodiscard]]virtual logical_bool eq_with(ptr a)const{return a==this;}
	[[nodiscard]]virtual constexpr size_t equal_level()const{return 0;}
	[[nodiscard]]virtual constexpr size_t eq_level()const{return 0;}
public:
	virtual ~LIS_name(node_like)()=default;

	[[nodiscard]]virtual setter operator[](ptr)=0;

	virtual void clear()=0;

	[[nodiscard]]virtual explicit operator hash_t()const=0;
	virtual void destroy(){
		this->clear();
		this->be_replace_as(null_ptr);
	}
	virtual void be_replace_as(ptr a){
		replace_able::be_replace_as(a.get());
	}

	[[nodiscard]]virtual explicit operator logical_bool()const=0;
	[[nodiscard]]explicit operator bool()const{return this->operator logical_bool();}

	template<typename...Args>
	inline setter operator()(Args&&...rest){return this->get_call_of_this()(node_list(forward<Args>(rest)...));}

	[[nodiscard]]logical_bool eq(ptr a){
		if(this->eq_level()==a->eq_level())
			return this->eq_with(a)&&a->eq_with(this);
		elseif(this->eq_level() _small_than_ a->eq_level())
			return a->eq_with(this);
		elseif(this->eq_level() _big_than_ a->eq_level())
			return this->eq_with(a);
	}
	[[nodiscard]]logical_bool equal(ptr a){
		if(this->equal_level()==a->equal_level())
			return this->equal_with(a)&&a->equal_with(this);
		elseif(this->equal_level() _small_than_ a->equal_level())
			return a->equal_with(this);
		elseif(this->equal_level() _big_than_ a->equal_level())
			return this->equal_with(a);
	}

	[[nodiscard]]ptr operator&(){return this;}
};
lazy_instantiation_name(node_like);

//base中的类型功能适应器
[[nodiscard]]logical_bool pointer_to_bool(node_like*a)noexcept{
	return a->operator logical_bool();
}
[[nodiscard]]logical_bool pointer_equal(node_like*a,node_like*b)noexcept{
	return a->eq(b);
}
[[nodiscard]]constexpr node_like*get_null_ptr(type_info_t<node_like>)noexcept;//{return&nil;}
[[nodiscard]]hash_t pointer_hash(node_like*a)noexcept{
	return a->operator hash_t();
}

//file_end

