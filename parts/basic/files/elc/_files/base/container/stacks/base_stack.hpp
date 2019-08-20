//defs.hpp
//at namespace elc::defs::container::stack_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
class base_stack_t:non_copyable{//基础容器，可用于hashtable
	typedef base_stack_t<T>this_t;
protected:
	struct data_t{
		T _data;
		data_t*_next;
	}*_m;
	size_t _size;
public:
	constexpr base_stack_t():_m(null_ptr),_size(0){}
	this_t&operator=(this_t&&a)noexcept{
		swap(_m,a._m);
		swap(_size,a._size);
		return*this;
	}
	base_stack_t(this_t&&a)noexcept:base_stack_t(){
		operator=(a);
	}
	~base_stack_t()noexcept(unget.nothrow<data_t>){
		data_t*tmp;
		while(_m!=null_ptr){
			tmp=_m;
			_m=_m->_next;
			unget(tmp);
		}
	}
	void clear()noexcept(destruct.nothrow<this_t>&&construct<this_t>.nothrow<>){
		destruct(this);
		construct<this_t>[this]();
	}
	[[nodiscard]]bool empty()noexcept{
		
	}
	template<typename T_>
	maybe_fail_reference<T>find(T_&&a)noexcept_as(declvalue(T_)==declvalue(T&)){
		data_t*tmp=_m;
		while(tmp!=null_ptr){
			if(a==tmp->_data)
				return tmp->_data;
			tmp=tmp->_next;
		}
		return note::fail;
	}
	bool in_stack(const T&a)noexcept_as(declvalue(const T&)==declvalue(T&)){
		return find(a).not_fail();
	}
	[[nodiscard]]size_t size()noexcept{
		return _size;
	}
	static constexpr bool add_nothrow=get<data_t>.nothrow<decltype({declvalue(const T&),declvalue(data_t*)})>;
	size_t add(const T&a)noexcept(add_nothrow){
		_m=get<data_t>({a,_m});
		_size++;
		return size();
	}
	size_t remove(const T&a)noexcept(unget.nothrow<data_t>){
		data_t*tmp=_m,**tmp_=&_m;
		size_t size=0;
		while(tmp!=null_ptr){
			size++;
			if(a==tmp->_data){
				*tmp_=tmp->_next;
				unget(tmp);
				return size;
			}
			tmp_=&tmp->_next;
			tmp=*tmp_;
		}
		return 0;
	}
private:
	void add(data_t*a){
		a->_next=_m;
		_m=a;
		_size++;
	}
public:
	[[nodiscard]]hash_t get_top_hash()noexcept_as(hash(declvalue(T&))){
		if(_m)
			return hash(_m->_data);
	}
	bool move_top_to(this_t&a)noexcept{
		if(_m){
			size--;
			auto tmp=_m;
			_m=_m->_next;
			a.add(tmp);
			return true;
		}else
			return false;
	}
};
