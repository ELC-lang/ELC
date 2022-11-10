//inserted_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct inserted_string_data_t final: base_string_data_t<char_T>,instance_struct<inserted_string_data_t<char_T>>{
	typedef inserted_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;
	using base_t::string_ptr_t;
	using base_t::const_string_ptr_t;
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
	using base_t::set_hash_cache;

	ptr_t  _to;
	ptr_t  _insert_data;
	size_t _to_size;
	size_t _insert_pos;
	size_t _insert_size;

	inserted_string_data_t(ptr_t to,ptr_t insert_data,size_t insert_pos)noexcept:_to(to),_insert_data(insert_data),_insert_pos(insert_pos),_to_size(to->get_size()),_insert_size(insert_data->get_size()){}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<_insert_pos)
			return _to->get_substr_data(begin,size);
		elseif(begin>_insert_pos+_insert_size)
			return _to->get_substr_data(begin-_insert_size,size);
		elseif(begin>=_insert_pos && begin+size<=_insert_pos+_insert_size)
			return _insert_data->get_substr_data(begin-_insert_pos,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		if(type_info<this_t> == typeid(*a)){
			const auto p = down_cast<this_t*>(a.get());
			if(_insert_pos==p->_insert_pos && _insert_size==p->_insert_size){
				if(_to!=p->_to)
					base_t::equivalent_optimization(_to, p->_to);
				if(_insert_data!=p->_insert_data)
					base_t::equivalent_optimization(_insert_data, p->_insert_data);
			}
		}
		_to.reset();
		_insert_data.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _to_size+_insert_size; }
protected:
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		if(pos+size<_insert_pos)
			_to->copy_part_data_to(to,pos,size);
		elseif(pos>_insert_pos+_insert_size)
			_to->copy_part_data_to(to,pos-_insert_size,size);
		else{
			if(_insert_pos>pos){
				auto size_before_insert_pos=_insert_pos-pos;
				_to->copy_part_data_to(to,pos,size_before_insert_pos);
				to+=size_before_insert_pos;
				size-=size_before_insert_pos;
			}
			auto size_before_insert_end=min(size,_insert_size);
			_insert_data->copy_part_data_to(to,0,size_before_insert_end);
			to+=size_before_insert_end;
			size-=size_before_insert_end;
			if(size)
				_to->copy_part_data_to(to,_insert_pos,size);
		}
	}
public:
	[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str)noexcept(copy_construct_nothrow)override final{
		if(this->is_unique()){
			if(pos>=_insert_pos && pos<=_insert_pos+_insert_size){
				_insert_data=_insert_data->do_insert(pos-_insert_pos,str);
				_insert_size+=str.size();
			}
			elseif(pos<_insert_pos){
				_to=_to->do_insert(pos,str);
				_to_size+=str.size();
				_insert_pos+=str.size();
			}
			elseif(pos>_insert_pos+_insert_size){
				_to=_to->do_insert(pos-_insert_size,str);
				_to_size+=str.size();
			}
			self_changed();
			return this;
		}
		return base_t::do_insert(pos,str);
	}
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)noexcept override final{
		if(this->is_unique()){
			if(pos>=_insert_pos && pos+size<=_insert_pos+_insert_size){
				_insert_data=_insert_data->do_erase(pos-_insert_pos,size);
				_insert_size-=size;
				self_changed();
				return this;
			}
			elseif(pos+size<_insert_pos){
				_to=_to->do_erase(pos,size);
				_to_size-=size;
				_insert_pos-=size;
				self_changed();
				return this;
			}
			elseif(pos>_insert_pos+_insert_size){
				_to=_to->do_erase(pos-_insert_size,size);
				_to_size-=size;
				self_changed();
				return this;
			}
		}
		return base_t::do_erase(pos,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		if(index>=_insert_pos && index<_insert_pos+_insert_size)
			return _insert_data->arec(index-_insert_pos);
		elseif(index>=_insert_pos+_insert_size)
			return _to->arec(index-_insert_size);
		else
			return _to->arec(index);
	}

	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		if(this->is_unique()){
			if(index>=_insert_pos && index<_insert_pos+_insert_size)
				_insert_data->arec_set(index-_insert_pos,a,p);
			elseif(index>=_insert_pos+_insert_size)
				_to->arec_set(index-_insert_size,a,p);
			else
				_to->arec_set(index,a,p);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			const auto size=str.size();
			if(_insert_pos==0){
				_insert_data=_insert_data->apply_str_to_begin(str);
				_insert_size+=size;
			}
			else{
				_to=_to->apply_str_to_begin(str);
				_to_size+=str.size();
				_insert_pos+=size;
			}
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			const auto size=str->get_size();
			if(_insert_pos==0){
				_insert_data=_insert_data->apply_str_to_begin(str);
				_insert_size+=size;
			}
			else{
				_to=_to->apply_str_to_begin(str);
				_to_size+=str->get_size();
				_insert_pos+=size;
			}
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			if(_insert_pos==_to_size){
				_insert_data=_insert_data->apply_str_to_end(str);
				_insert_size+=str.size();
			}
			else{
				_to=_to->apply_str_to_end(str);
				_to_size+=str.size();
			}
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			if(_insert_pos==_to_size){
				_insert_data=_insert_data->apply_str_to_end(str);
				_insert_size+=str->get_size();
			}
			else{
				_to=_to->apply_str_to_end(str);
				_to_size+=str->get_size();
			}
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			if(_insert_pos > size){
				auto aret=_to->do_pop_front(size,_to);
				_to_size-=size;
				_insert_pos-=size;
				self_changed();
				return aret;
			}
			elseif(_insert_pos==0 && _insert_size>=size){
				auto aret=_insert_data->do_pop_front(size,_insert_data);
				_insert_size-=size;
				self_changed();
				return aret;
			}
		}
		return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			if(_insert_pos+_insert_size <= _to_size-size){
				auto aret=_to->do_pop_back(size,_to);
				_to_size-=size;
				self_changed();
				return aret;
			}
			elseif(_insert_pos==_to_size && _insert_size>=size){
				auto aret=_insert_data->do_pop_front(size,_insert_data);
				_insert_size-=size;
				self_changed();
				return aret;
			}
		}
		return base_t::do_pop_back(size,self);
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&)noexcept(hash_nothrow)override final{
		auto result=hash(nothing);
		if(_insert_pos){
			result=_to->get_others_hash_with_calculated_before(result,0,_to,0,_insert_pos);
		}
		result=hash.merge_array_hash_results(result,_insert_pos,_insert_data->get_hash(_insert_data),_insert_size);
		const auto size=_to_size-_insert_pos;
		if(size)
			result=_to->get_others_hash_with_calculated_before(result,_insert_pos+_insert_size,_to,_insert_pos,size);
		return result;
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos+size<_insert_pos)
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,size);
		elseif(pos>_insert_pos+_insert_size)
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos-_insert_size,size);
		else{
			if(_insert_pos>pos){
				const auto size_before_insert_pos=_insert_pos-pos;
				before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos,size_before_insert_pos);
				size-=size_before_insert_pos;
				before_size+=size_before_insert_pos;
			}
			auto size_before_insert_end=min(size,_insert_size);
			before=_insert_data->get_others_hash_with_calculated_before(before,before_size,_insert_data,0,size_before_insert_end);
			size-=size_before_insert_end;
			before_size+=size_before_insert_end;
			if(size)
				before=_to->get_others_hash_with_calculated_before(before,before_size,_to,_insert_pos,size);
		}
		return before;
	}
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t with)noexcept override final{
		auto wp=down_cast<this_t*>(with.get());
		return _insert_pos==wp->_insert_pos && _insert_size==wp->_insert_size;//总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t index)noexcept override final{
		if(index>=_insert_pos && index<_insert_pos+_insert_size)
			return _insert_data->get_the_largest_complete_data_block_begin_form(index-_insert_pos);
		elseif(index>=_insert_pos+_insert_size)
			return _to->get_the_largest_complete_data_block_begin_form(index-_insert_size);
		else {
			auto aret=_to->get_the_largest_complete_data_block_begin_form(index);
			if(aret.size()+index > _insert_pos)
				aret = {aret.begin(), note::size(_insert_pos-index)};
			return aret;
		}
	}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		return _insert_data->equal_with_same_size(wp->_insert_data) && _to->equal_with_same_size(wp->_to);
	}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		if(auto tmp=_to->compare_with(wp->_to,0,_insert_pos); tmp!=0)
			return tmp;
		if(auto tmp=_insert_data->compare_with_same_size(wp->_insert_data); tmp!=0)
			return tmp;
		return _to->compare_with(wp->_to,_insert_pos,_to_size-(_insert_pos+_insert_size));
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{
		return float_size_of(*this)+_insert_data->get_memory_cost()+_to->get_memory_cost();
	}
};
template<typename char_T>
base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos,ptr_t str)noexcept{
	return get<inserted_string_data_t<char_T>>(this,str,pos);
}
template<typename char_T>
base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos,string_view_t str)noexcept(copy_construct_nothrow){
	if(pos==0)
		return this->apply_str_to_begin(str);
	elseif(pos==get_size())
		return this->apply_str_to_end(str);
	else
		return this->do_insert(pos,get<comn_string_data_t<char_T>>(str));
}

//file_end

