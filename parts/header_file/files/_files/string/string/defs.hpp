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
struct string_t:copy_on_write_t<vector_t<T>>{
private:
	typedef copy_on_write_t<vector_t<T>> base_t_w;
public:
	typedef string_t<T>this_t;
	typedef base_t_w::iterator iterator;
	typedef base_t_w::const_iterator const_iterator;
public:
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
	void for_each(){
		#error
	}
};

//file_end

