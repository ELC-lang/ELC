//erased_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct erased_string_data_t final:base_string_data_t<char_T>,instance_struct<erased_string_data_t<char_T>>{
	typedef erased_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;
	using base_t::self_changed;
	using base_t::has_hash_cache;
	using base_t::hash_cache;

	ptr_t  _to;
	size_t _to_size;
	size_t _erase_pos;
	size_t _erase_size;

	erased_string_data_t(ptr_t str,size_t erase_pos,size_t erase_size)noexcept:_to(str),_to_size(_to->get_size()),_erase_pos(erase_pos),_erase_size(erase_size){}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<_erase_pos)
			return _to->get_substr_data(begin,size);
		elseif(begin>_erase_pos)
			return _to->get_substr_data(begin+_erase_size,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	virtual void be_replace_as(ptr_t a) noexcept override final {
		static_assert(noexcept(*a));//貌似msvc在这里有bug
		if(type_info<this_t> == typeid(*a)){
			const auto p = down_cast<this_t*>(a.get());
			if(_erase_pos==p->_erase_pos && _erase_size==p->_erase_size && _to!=p->_to)
				base_t::equivalent_optimization(_to, p->_to);
		}
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _to_size-_erase_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept override final{
		if(pos+size<_erase_pos)
			_to->copy_part_data_to(to,pos,size);
		elseif(pos>_erase_pos)
			_to->copy_part_data_to(to,pos+_erase_size,size);
		else{
			const auto size_defore_erase_pos=_erase_pos-pos;
			const auto size_after_erase_pos=size-size_defore_erase_pos;
			_to->copy_part_data_to(to,pos,size_defore_erase_pos);
			_to->copy_part_data_to(to+size_defore_erase_pos,_erase_pos+_erase_size,size_after_erase_pos);
		}
	}
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)noexcept override final{
		if(this->is_unique()){
			if(pos<=_erase_pos && pos+size>=_erase_pos+_erase_size){
				_erase_pos=pos;
				_erase_size+=size;
				self_changed();
				return this;
			}
		}
		return base_t::do_erase(pos,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)noexcept override final{
		if(index>_erase_pos)
			return _to->arec(index+_erase_size);
		else
			return _to->arec(index);
	}

	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept override final{
		if(this->is_unique()){
			if(index>_erase_pos)
				_to->arec_set(index+_erase_size,a,_to);
			else
				_to->arec_set(index,a,_to);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			const auto strsize=str.size();
			_to_size+=strsize;
			_erase_pos+=strsize;
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			const auto strsize=str->get_size();
			_to_size+=strsize;
			_erase_pos+=strsize;
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept override final{
		if(this->is_unique() && _erase_pos > size){
			auto aret=_to->do_pop_front(size,_to);
			_to_size-=size;
			_erase_pos-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept override final{
		if(this->is_unique() && _erase_pos+_erase_size <= _to_size-size){
			auto aret=_to->do_pop_back(size,_to);
			_to_size-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}

	virtual hash_t get_hash(ptr_t&p)noexcept override final{
		if(has_hash_cache())
			return hash_cache;
		else{
			auto result=hash(nothing);
			const auto size_defore_erase_pos=_erase_pos;
			const auto size_after_erase_pos=_to_size-size_defore_erase_pos;
			result=_to->get_others_hash_with_calculated_before(result,_to,0,size_defore_erase_pos);
			result=_to->get_others_hash_with_calculated_before(result,_to,_erase_pos+_erase_size,size_after_erase_pos);
			return hash_cache=result;
		}
	}
	virtual hash_t get_others_hash_with_calculated_before(hash_t before,ptr_t&p,size_t pos,size_t size)noexcept override final{
		if(pos+size<_erase_pos)
			before=_to->get_others_hash_with_calculated_before(before,_to,pos,size);
		elseif(pos>_erase_pos)
			before=_to->get_others_hash_with_calculated_before(before,_to,pos+_erase_size,size);
		else{
			const auto size_defore_erase_pos=_erase_pos-pos;
			const auto size_after_erase_pos=size-size_defore_erase_pos;
			before=_to->get_others_hash_with_calculated_before(before,_to,pos,size_defore_erase_pos);
			before=_to->get_others_hash_with_calculated_before(before,_to,_erase_pos+_erase_size,size_after_erase_pos);
		}
		return before;
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		return (sizeof(*this)+_to->get_memory_cost())/get_ref_num((const base_t*)this);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size)noexcept{
	return get<erased_string_data_t<char_T>>(this,pos,size);
}

//file_end

