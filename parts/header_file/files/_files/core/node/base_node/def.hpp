//def.hpp
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//草稿警告
/*
node:
	method table:
		call
		eval
		operator bool

	arecer
*/
//UF:草稿代码警告

struct base_node:gc_able,ref_able,weak_ref_able,replace_able{
private:
	template<typename...Args>
	inline setter list_call(Args&&... rest){return this->call(list(forward<Args>(rest)...));}
public:
	virtual setter operator[](ptr)=0;
	virtual setter operator[](eluint a){this->operator[](make_node_by<eluint>(a));}

	template<typename T>
	inline setter arec(T a){return this->operator[](a);}

	virtual void clear()=0;

	virtual setter type()=0;

	virtual explicit operator bool(){return get_bool_of(this)->list_call(this);}
	virtual setter eval(){return eval_of(this)->list_call(this);}
	virtual setter call(ptr args){return call_of(this)->call(cons(this,args));}
	template<typename...Args>
	inline setter operator()(Args&&... rest){return list_call(forward<Args>(rest)...);}

	virtual logic_bool eq(ptr a){return a==this->operator&();}
	virtual logic_bool equal(ptr a){return this->type()->arec(equal)->list_call(this,a);}



	virtual void destroy(){
		this->clear();
		this->replace(null_ptr);
	}
	void replace(ptr a){
		replace_able::replace(a);
	}
	ptr operator&(){return this;}
};

//file_end

