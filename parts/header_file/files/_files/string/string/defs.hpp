//_body.hpp
//at namespace elc
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF:未完草稿代码
template<typename T>
struct string_t:copy_on_write_array<T>,can_shrink<string_t<T>>{
	/*
	1.按倍数扩大，减少分配上的时间损耗
	2.copy_on_write
	*/
private:
	size_t _size;
private:
	typedef copy_on_write_array<T> base_t_w;
	T*get_data_begin()const noexcept{
		return const_cast<T*>(addressof((*this)[zero]));
	}
	using base_t_w::copy_check;
	void size_grow_to(size_t a){
		size_t dsize=base_t_w::size()；
		if(dsize == 0)
			dsize = a;
		while(a > dsize)
			dsize*=magic_number::gold_of_resize;
		if(dsize == base_t_w::size())
			resize(dsize);
		_size=a;
	}
public:
	void shrink()const{
		const_cast<array_t<T>&>((const array_t<T>&)(*this)).resize(_size);//通过const_cast绕过copy_on_write，否则会引起不必要的时空浪费
	}
public:
	typedef string_t<T>this_t;
	using base_t_w::iterator;
	using base_t_w::const_iterator;
public:
	[[nodiscard]]size_t size()const noexcept{return _size;}
	[[nodiscard]]constexpr iterator get_iterator_at(size_t a)noexcept{
		return _data.get_iterator_at(a);
	}
	[[nodiscard]]constexpr iterator begin()noexcept{
		return iteratorget_iterator_at(zero);
	}
	[[nodiscard]]iterator end()noexcept{
		return iteratorget_iterator_at(size());
	}
	[[nodiscard]]const_iterator cbegin()const noexcept{
		return const_cast<this_t*>(this)->begin();
	}
	[[nodiscard]]const_iterator cend()const noexcept{
		return const_cast<this_t*>(this)->end();
	}
	[[nodiscard]]bool empty()const noexcept{
		return size();
	}
	[[nodiscard]]bool operator==(const this_t&a)const noexcept(destruct.nothrow<T>){
		if(_data.get_copy_on_write_id() == a._data.get_copy_on_write_id())//同一数据块
			return true;
		if(_size != a._size)//长度不等
			return false;
		if(){
			::std::memcmp()
		}else{
			const_iterator i=cbegin(),ai=a.cbegin();
			auto s=size();
			while(s--){
				if(not equal(*(i++),*(ai++)))return false;
			}
		}
		return true;
	}
	[[nodiscard]]bool operator==(const this_t&a){
		return !operator==(a);
	}
	this_t&operator=(const this_t&)&noexcept(destruct.nothrow<T>)=default;

	using_method_from_value(for_each,_data);
};

//file_end

