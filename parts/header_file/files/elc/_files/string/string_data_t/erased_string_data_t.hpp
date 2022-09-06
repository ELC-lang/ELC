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

	ptr_t  _to;
	size_t _to_size;
	size_t _erase_pos;
	size_t _erase_size;

	void null_equivalent_check()noexcept{
		if(_to_size==_erase_size || !_to_size)
			be_replace_as(null_ptr);
	}

	erased_string_data_t(ptr_t str,size_t erase_pos,size_t erase_size)noexcept:_to(str),_to_size(_to->get_size()),_erase_pos(erase_pos),_erase_size(erase_size){
		null_equivalent_check();
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<_erase_pos)
			return _to->get_substr_data(begin,size);
		elseif(begin>_erase_pos)
			return _to->get_substr_data(begin+_erase_size,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final {
		if(type_info<this_t> == typeid(*a)){
			const auto p = down_cast<this_t*>(a.get());
			if(_erase_pos==p->_erase_pos && _erase_size==p->_erase_size && _to!=p->_to)
				base_t::equivalent_optimization(_to, p->_to);
		}
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _to_size-_erase_size; }
protected:
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		if(pos+size<_erase_pos)
			_to->copy_part_data_to(to,pos,size);
		elseif(pos>_erase_pos)
			_to->copy_part_data_to(to,pos+_erase_size,size);
		else{
			const auto size_before_erase_pos=_erase_pos-pos;
			const auto size_after_erase_pos=size-size_before_erase_pos;
			_to->copy_part_data_to(to,pos,size_before_erase_pos);
			_to->copy_part_data_to(to+size_before_erase_pos,_erase_pos+_erase_size,size_after_erase_pos);
		}
	}
public:
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
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		if(index>_erase_pos)
			return _to->arec(index+_erase_size);
		else
			return _to->arec(index);
	}

	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
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
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
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
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
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
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _erase_pos > size){
			auto aret=_to->do_pop_front(size,_to);
			_to_size-=size;
			_erase_pos-=size;
			null_equivalent_check();
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _erase_pos+_erase_size <= _to_size-size){
			auto aret=_to->do_pop_back(size,_to);
			_to_size-=size;
			null_equivalent_check();
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&)noexcept(hash_nothrow)override final{
		auto result=hash(nothing);
		const auto size=get_size();
		const auto size_before_erase_pos=_erase_pos;
		const auto size_after_erase_pos=size-size_before_erase_pos;
		result=_to->get_others_hash_with_calculated_before(result,0,_to,0,size_before_erase_pos);
		result=_to->get_others_hash_with_calculated_before(result,size_before_erase_pos,_to,_erase_pos+_erase_size,size_after_erase_pos);
		return result;
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos+size<_erase_pos)
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,size);
		elseif(pos>_erase_pos)
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos+_erase_size,size);
		else{
			const auto size_before_erase_pos=_erase_pos-pos;
			const auto size_after_erase_pos=size-size_before_erase_pos;
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,size_before_erase_pos);
			before=_to->get_others_hash_with_calculated_before(before,before_size+size_before_erase_pos,_to,_erase_pos+_erase_size,size_after_erase_pos);
		}
		return before;
	}
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t with)noexcept override final{
		auto wp = down_cast<this_t*>(with.get());
		return _erase_pos == wp->_erase_pos && _erase_size == wp->_erase_size;
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept override final{
		if(begin < _erase_pos) {
			auto aret=_to->get_the_largest_complete_data_block_begin_form(begin);
			if(aret.size() > _erase_pos) {
				aret = {aret.begin(),_erase_pos};
			}
			return aret;
		}
		else
			return _to->get_the_largest_complete_data_block_begin_form(begin + _erase_size);
	}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		if(!_to->equal_with(wp->_to,0,_erase_pos))
			return false;
		return _to->equal_with(wp->_to,_erase_pos+_erase_size,_to_size);
	}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		if(auto tmp=_to->compare_with(wp->_to,0,_erase_pos); tmp!=0)
			return tmp;
		return _to->compare_with(wp->_to,_erase_pos+_erase_size,_to_size);
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{
		return float_size_of(*this)+_to->get_memory_cost();
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size)noexcept{
	if(size==get_size())
		return null_ptr;
	else
		return get<erased_string_data_t<char_T>>(this,pos,size);
}

//file_end

