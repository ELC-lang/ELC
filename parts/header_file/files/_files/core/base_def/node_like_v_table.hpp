struct node_like:{
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
	virtual ~node_like()=default;

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

	[[nodiscard]]virtual explicit operator logical_bool()const{return true;}
};