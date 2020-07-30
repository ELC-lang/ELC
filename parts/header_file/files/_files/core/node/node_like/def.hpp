//def.hpp
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
setter eval(){return eval_of(this)(this);}

struct node_like:ref_able,weak_ref_able,replace_able{
protected:
	virtual const base_type_info_t& get_type_info()const=0;
	virtual function_t<setter()> get_eval_of_this()const{return lambda_with_catch(this){return this;};}
	virtual function_t<setter(ptr)> get_call_of_this()const{return lambda_with_catch(this){return this;};}
public:
	virtual ~node_like()=default;

	virtual setter operator[](ptr)=0;

	virtual void clear()=0;

	virtual explicit operator logic_bool()const=0;
	explicit operator bool()const{return operator logic_bool();}

	template<typename...Args>
	inline setter operator()(Args&&... rest){return this->get_call_of_this()(node_list(forward<Args>(rest)...));}

	virtual logic_bool eq(ptr a){return a==this->operator&();}
	virtual logic_bool equal(ptr a){return this->get_type_info()->arec(equal)->list_call(this,a);}

	virtual void destroy(){
		this->clear();
		this->be_replace_as(null_ptr);
	}
	virtual void be_replace_as(ptr a){
		replace_able::be_replace_as(a);
	}
	ptr operator&(){return this;}
};

//file_end

