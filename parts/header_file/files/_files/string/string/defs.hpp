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
struct string_t:container_struct{
private:
	copy_on_write_array<T>_data;
	size_t _size;

	typedef copy_on_write_array<T> base_t_w;
public:
	typedef string_t<T>this_t;
	typedef base_t_w::iterator iterator;
	typedef base_t_w::const_iterator const_iterator;
public:
	[[nodiscard]]T&operator[](size_t a)noexcept_as(declvalue(base_t_w)[declvalue(size_t)]){return _data[a];}
	[[nodiscard]]const T&operator[](size_t)const noexcept_as(declvalue(const base_t_w)[declvalue(size_t)]){return _data[a];}
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
	[[nodiscard]]bool operator==(const this_t&){
		
	}
	[[nodiscard]]bool operator==(const this_t&a){
		return !operator==(a);
	}
	operator=
	for_each
};

//file_end

