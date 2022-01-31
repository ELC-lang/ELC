//constexpr_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct constexpr_string_data_t final:base_string_data_t<char_T>,instance_struct<constexpr_string_data_t<char_T>>{
	typedef constexpr_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;

	const char_T* _m;
	size_t _size;

	constexpr_string_data_t(string_view_t str){
		_m=str.begin();
		_size=str.size();
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)override final{
		return get<this_t>(string_view_t{_m+begin,size});
	}
	[[nodiscard]]virtual const char_T* get_data(){return _m;}
	[[nodiscard]]virtual const char_T* get_const_c_str(){
		if(!_m[_size])
			return _m;
		else
			return base_t::get_const_c_str();
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ copy_assign[size](note::form(_m+pos),note::to(to)); }
	[[nodiscard]]virtual char_T arec(size_t index)override final{ return _m[index]; }

	virtual void be_replace_as(ptr_t a)override final{base_t::be_replace_as(a);}
	virtual void arec_set(size_t index,char_T a,ptr_t&p)override final{base_t::arec_set(index,a,p);}

	[[nodiscard]]virtual double get_memory_cost()override final{
		return double(sizeof(*this))/get_ref_num((const base_t*)this);
	}
};

//file_end

