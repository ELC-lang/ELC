//list.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//copy from old ELC
template<typename T>
class list_t;
/*
common attribute,表明这是对于list_t可加入的cons.
*/
template<typename T>
struct cons_t:non_copyable,non_moveable,is_common_attribute(cons_t){
	typedef cons_t<T> this_t;
private:
	friend list_t<T>;
	friend iterator_t<T,this_t*>;
	typedef attribute<T,this_t> attribute_t;

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
/*
T类型的有序双向list.
方法有:
constexpr void add(cons_t<T>*a)noexcept;
constexpr iterator begin()noexcept;
constexpr iterator end()noexcept;
constexpr iterator head()noexcept;
constexpr bool empty()const noexcept;
*/
template<typename T>
struct list_t{
	typedef list_t<T> this_t;
	typedef cons_t<T> cons;
	typedef iterator_t<T,cons*>iterator;
private:
	cons _begin,_end;
public:
	constexpr list_t()noexcept{_begin.bind_with(_end);}
	inline constexpr void add(cons*a)noexcept{
		a->insert_to_before(&_end);
	}
	[[nodiscard]]constexpr iterator begin()noexcept{return _begin.get_next();}//这叫头迭代器
	[[nodiscard]]constexpr iterator end()noexcept{return&_end;}//这叫超尾迭代器
	[[nodiscard]]constexpr iterator head()noexcept{return&_begin;}//。。。。这个大概就是超头迭代器了😂
	[[nodiscard]]constexpr bool empty()const noexcept{return _begin.get_next()==&_end;}
};

//file_end

