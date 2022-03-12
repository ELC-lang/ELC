//substr_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct substr_string_data_t final:base_string_data_t<char_T>,instance_struct<substr_string_data_t<char_T>>{
	typedef substr_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;
	using base_t::self_changed;
	using base_t::has_hash_cache;
	using base_t::hash_cache;

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

	ptr_t _to;
	size_t _sub_begin;
	size_t _sub_size;

	substr_string_data_t(ptr_t str,size_t sub_begin,size_t sub_size)noexcept:_to(str),_sub_begin(sub_begin),_sub_size(sub_size){}

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{ return get<substr_string_data_t<char_T>>(_to,begin+_sub_begin,size); }
	[[nodiscard]]virtual const char_T* get_const_c_str(ptr_t&p)noexcept(get_data_nothrow)override final{
		if(_sub_begin+_sub_size==_to->get_size())
			return _to->get_const_c_str(_to)+_sub_begin;
		else
			return base_t::get_const_c_str(p);
	}
	[[nodiscard]]virtual const char_T* get_data(ptr_t&)noexcept(get_data_nothrow)override final{ return _to->get_data(_to)+_sub_begin; }
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _sub_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow) override final{ _to->copy_part_data_to(to,pos+_sub_begin,size); }
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow) override final{ return _to->arec(index+_sub_begin); }
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow) override final{
		if(this->is_unique()){
			_to->arec_set(index+_sub_begin,a,_to);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique() && _sub_begin==0){
			_to=_to->apply_str_to_begin(str);
			_sub_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique() && _sub_begin==0){
			_to=_to->apply_str_to_begin(str);
			_sub_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()){
			_to=_to->apply_str_to_end(str);
			_sub_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()){
			_to=_to->apply_str_to_end(str);
			_sub_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept override final{
		if(this->is_unique() && _sub_begin==0){
			auto aret=_to->do_pop_front(size,_to);
			_sub_size-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept override final{
		if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()){
			auto aret=_to->do_pop_back(size,_to);
			_sub_size-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}

	virtual hash_t get_hash(ptr_t&p)noexcept(hash_nothrow)override final{
		if(has_hash_cache())
			return hash_cache;
		else{
			auto result=hash(nothing);
			result=_to->get_others_hash_with_calculated_before(result,0,_to,_sub_begin,_sub_size);
			return hash_cache=result;
		}
	}
	virtual hash_t get_others_hash_with_calculated_before(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos==0&&size==get_size())
			return hash.merge_array_hash_results(before,before_size,get_hash(p),size);
		before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos+_sub_begin,size);
		return before;
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		return (sizeof(*this)+_to->get_memory_cost())/get_ref_num((const base_t*)this);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::get_substr_data(size_t begin,size_t size)noexcept{
	return get<substr_string_data_t<char_T>>(this,begin,size);
}

//file_end

