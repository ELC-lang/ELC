//comn_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct comn_string_data_t final:base_string_data_t<char_T>,instance_struct<comn_string_data_t<char_T>>{
	typedef comn_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;
	using base_t::self_changed;

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

	array_t<char_T> _m;

	comn_string_data_t(string_view_t str)noexcept(construct_nothrow&&copy_assign_nothrow):_m(note::size(str.size()+1)){
		copy_assign[str.size()](note::form(str.begin()),note::to((char_T*)_m));
	}
	comn_string_data_t(ptr_t str)noexcept(construct_nothrow&&copy_assign_nothrow):_m(note::size(str->get_size()+1)){
		str->copy_part_data_to((char_T*)_m,0,this->get_size());
	}
	comn_string_data_t(ptr_t str,size_t pos,size_t size)noexcept(construct_nothrow&&copy_assign_nothrow):_m(note::size(size+1)){
		str->copy_part_data_to((char_T*)_m,pos,size);
	}

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		_m.clear();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual char_T* get_c_str(ptr_t&)noexcept override final{ return (char_T*)_m; }
	[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&p)noexcept(get_data_nothrow)override final{
		if(this->is_unique())
			return (char_T*)_m;
		else
			return base_t::get_unique_c_str(p);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _m.size()-1; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{ copy_assign[size](note::form((const char_T*)_m+pos),note::to(to)); }
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{ return _m[index]; }
	virtual void arec_set(size_t index,char_T a,ptr_t&p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		if(this->is_unique()){
			copy_assign(_m[index],a);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}

	[[nodiscard]]virtual bool same_struct(ptr_t)noexcept override final{
		return true;//总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept override final{return {&_m[begin],note::size(get_size()-begin)};}
	virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		return compare(_m, wp->_m);
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		const auto this_size=sizeof(*this)+_m.size_in_byte();
		return float_size_t(this_size)/get_ref_num((const base_t*)this);
	}
};
template<typename char_T>
[[nodiscard]]char_T* base_string_data_t<char_T>::get_c_str(ptr_t&a)noexcept(get_data_nothrow){
	auto comn_data=get<comn_string_data_t<char_T>>(this);
	if(positive_gc_profit())
		a.do_replace(comn_data);
	else
		a=comn_data;
	return comn_data->get_c_str(a);
}
template<typename char_T>
[[nodiscard]]char_T* base_string_data_t<char_T>::get_unique_c_str(ptr_t&a)noexcept(get_data_nothrow){
	auto comn_data=get<comn_string_data_t<char_T>>(this);
	a=comn_data;
	return comn_data->get_c_str(a);
}
template<typename char_T>
void base_string_data_t<char_T>::arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow){
	copy_assign(this->get_unique_c_str(p)[index],a);
}
template<typename char_T>
[[nodiscard]]float_size_t base_string_data_t<char_T>::get_memory_cost_after_gc()noexcept{
	const auto size_of_base_array=this->get_size()*sizeof(char_T);
	const auto size=sizeof(comn_string_data_t<char_T>)+size_of_base_array;
	return float_size_t(size)/get_ref_num(this);
}

//file_end

