//list.hpp
//at namespace elc::defs::base::list_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC

注意：
	本文件可能使用了cpp20的新支持或语义，而某些编译器可能并没有支持
	同时，此项目并没有完成
*/
//copy from old ELC
//TODO:move to base/container.
template<typename T>
class list_t;
template<typename T>
struct cons_t:non_copyable,non_moveable,attribute<T,cons_t<T>>,basic_struct,container_struct{
	typedef cons_t<T> this_t;
private:
	friend list_t<T>;
	typedef attribute<T,this_t> attribute;

	mutable this_t*_before;//为了在析构时修改前一项的next，勿删
	mutable this_t*_next;

	[[nodiscard]]this_t*get_before()noexcept{return _before;}
	[[nodiscard]]this_t*get_next()noexcept{return _next;}
	
	constexpr cons_t(this_t*a,this_t*b):_before(a),_next(b){}

	constexpr void set_before(this_t*a){
		_before=a;
		a->_next=this;
	}
	constexpr void set_next(this_t*a){
		_next=a;
		a->_before=this;
	}
	constexpr void insert_to_before(this_t*a){
		set_before(a->_before);
		set_next(a);
	}
	constexpr void bind_with(this_t*a)noexcept{
		set_next(a);
		set_before(a);
	}
	constexpr void remove_from_list()noexcept{
		_before->set_next(_next);
	}
	constexpr T*get_handle(){
		return attribute::get_handle();
	}
public:
	constexpr cons_t()=default;
	~cons_t(){
		remove_from_list();
	}
};
template<typename T>
struct list_t:container_struct{
	typedef list_t<T> this_t;
	typedef cons_t<T> cons;
	struct iterator{
		cons*value;
		constexpr iterator()noexcept=default;
		constexpr iterator(cons*a)noexcept:value(a){}
		constexpr bool operator==(cons*a)const noexcept{return value==a;}
		constexpr bool operator!=(cons*a)const noexcept{return!operator==(a);}
		constexpr iterator&operator=(cons*a)&noexcept{value=a;return*this;}
		constexpr iterator&operator++()&noexcept{return*this=value->get_next();}
		constexpr iterator&operator--()&noexcept{return*this=value->get_before();}
		constexpr iterator operator++(int)&noexcept{auto a=*this;operator++();return a;}
		constexpr iterator operator--(int)&noexcept{auto a=*this;operator--();return a;}
		constexpr T*operator->()noexcept{return value->get_handle();}
		constexpr T&operator*()noexcept{return*operator->();}
		constexpr operator T*()noexcept{return operator->();}
	};
private:
	cons _begin,_end;
public:
	constexpr list_t()noexcept{_begin.bind_with(_end);}
	constexpr inline void add(cons*a)noexcept{
		a->insert_to_before(&end_);
	}
	[[nodiscard]]constexpr iterator begin()noexcept{return head_.get_next();}//这叫头迭代器
	[[nodiscard]]constexpr iterator end()noexcept{return&end_;}//这叫超尾迭代器
	[[nodiscard]]constexpr iterator head()noexcept{return&head_;}//。。。。这个大概就是超头迭代器了😂
	[[nodiscard]]constexpr bool empty()noexcept{return head_.get_next()==&end_;}
};
