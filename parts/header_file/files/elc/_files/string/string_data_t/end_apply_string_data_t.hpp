//end_apply_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct end_apply_string_data_t final:base_string_data_t<char_T>,instance_struct<end_apply_string_data_t<char_T>>{
	typedef end_apply_string_data_t<char_T> this_t;
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
	ptr_t _to;
	size_t _to_size;
	size_t _used_size;

	end_apply_string_data_t(ptr_t str,string_view_t end)noexcept(construct_nothrow&&copy_assign_nothrow):
		_to_size(str->get_size()),
		_used_size(end.size()),
		_to(str)
	{
		_m.resize(get_next_gold_size_to_resize_for_array(_to_size+_used_size));
		copy_assign[_used_size](note::form(end.begin()),note::to((char_T*)_m));
	}
	end_apply_string_data_t(ptr_t str,size_t count,char_T ch)noexcept:
		_to_size(str->get_size()),
		_used_size(count),
		_to(str)
	{
		_m.resize(get_next_gold_size_to_resize_for_array(_to_size+_used_size));
		copy_assign[_used_size](ch,note::to((char_T*)_m));
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<=_to_size)
			return _to->get_substr_data(begin,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	[[nodiscard]]virtual char_T* get_c_str(ptr_t&p)noexcept(get_data_nothrow)override final{
		if(!_used_size)
			return _to->get_c_str(_to);
		else
			return base_t::get_c_str(p);
	}
	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		if(type_info<this_t> == typeid(*a)){
			const auto p = down_cast<this_t*>(a.get());
			if(_used_size==p->_used_size && _to!=p->_to)
				base_t::equivalent_optimization(_to, p->_to);
		}
		_m.clear();
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _used_size+_to_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		if(pos+size<=_to_size)
			_to->copy_part_data_to(to,pos,size);
		else{
			if(pos<_to_size){
				_to->copy_part_data_to(to,pos,_to_size-pos);
				auto copied_size=_to_size-pos;
				pos=0;
				size-=copied_size;
				to+=copied_size;
			}
			else
				pos-=_to_size;
			copy_assign[size](note::form((const char_T*)_m+pos),note::to(to));
		}
	}
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		if(index<_to_size)
			return _to->arec(index);
		else
			return _m[index-_to_size];
	}
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		if(this->is_unique()){
			if(index<_to_size)
				_to->arec_set(index,a,_to);
			else
				copy_assign(_m[index - _to_size],a);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}

	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			if(_m.size()-_used_size < str.size()){
				const auto size_now=this->get_size()+str.size();
				const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
				_m.resize(size_new);
			}
			copy_assign[str.size()](note::form(str.begin()),note::to((char_T*)_m+_used_size));
			_used_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow)override final{
		this->shrink();
		return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			_to_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			_to_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			auto aret=_to->do_pop_front(size,_to);
			_to_size-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _used_size>=size){
			_used_size-=size;
			self_changed();
			return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m+_used_size,size});
		}
		else
			return base_t::do_pop_back(size,self);
	}
	virtual hash_t get_hash_detail(ptr_t&p)noexcept(hash_nothrow)override final{
		return hash.with_calculated_before(_to->get_hash(_to),_to_size,string_view_t(_m.begin(),_used_size));
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos+size<=_to_size)
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,size);
		else{
			if(pos<_to_size){
				before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,_to_size-pos);
				const auto calculated_size=_to_size-pos;
				pos=0;
				size-=calculated_size;
				before_size+=calculated_size;
			}
			else
				pos-=_to_size;
			before=hash.with_calculated_before(before,before_size,(const char_T*)_m+pos,size);
		}
		return before;
	}
	void shrink()noexcept(_m.resize_nothrow){
		_m.resize(_used_size);
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		return (_to->get_memory_cost()+sizeof(*this)+_m.size_in_byte())/get_ref_num((const base_t*)this);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow){
	return get<end_apply_string_data_t<char_T>>(this,str);
}

//file_end

