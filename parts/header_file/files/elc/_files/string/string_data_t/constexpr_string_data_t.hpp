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

	using base_t::copy_assign_nothrow;
	using base_t::copy_construct_nothrow;
	using base_t::move_construct_nothrow;
	using base_t::construct_nothrow;
	using base_t::destruct_nothrow;
	using base_t::clear_nothrow;
	using base_t::ptr_reset_nothrow;
	using base_t::hash_nothrow;
	using base_t::get_data_nothrow;
	using base_t::apply_data_nothrow;

	const char_T* _m;
	size_t _size;

	constexpr_string_data_t(string_view_t str)noexcept{
		_m=str.begin();
		_size=str.size();
	}
	constexpr_string_data_t(string_view_t str,hash_t hash)noexcept:constexpr_string_data_t(str){
		base_t::hash_cache=hash;
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		return get<this_t>(string_view_t{_m+begin,size});
	}
	[[nodiscard]]virtual const char_T* get_data(ptr_t&)noexcept(get_data_nothrow)override final{return _m;}
	[[nodiscard]]virtual const char_T* get_const_c_str(ptr_t&p)noexcept(get_data_nothrow)override final{
		if(!_m[_size])
			return _m;
		else
			return base_t::get_const_c_str(p);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{ copy_assign[size](note::form(_m+pos),note::to(to)); }
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{ return _m[index]; }

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{base_t::be_replace_as(a);}
	virtual void arec_set(size_t index,char_T a,ptr_t&p)noexcept override final{base_t::arec_set(index,a,p);}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		return float_size_t{sizeof(*this)}/get_ref_num((const base_t*)this);
	}
};

//file_end

