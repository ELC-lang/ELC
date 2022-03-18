//sum_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct sum_string_data_t final:base_string_data_t<char_T>,instance_struct<sum_string_data_t<char_T>>{
	typedef sum_string_data_t<char_T> this_t;
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

	ptr_t  _before;
	ptr_t  _after;
	size_t _before_size;
	size_t _after_size;

	sum_string_data_t(ptr_t before,ptr_t after)noexcept:_before(before),_after(after),_before_size(_before->get_size()),_after_size(_after->get_size()){
		if(!_before_size)
			be_replace_as(_after);
		elseif(!_after_size)
			be_replace_as(_before);
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<=_before_size)
			return _before->get_substr_data(begin,size);
		elseif(begin>=_before_size)
			return _after->get_substr_data(begin-_before_size,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		if(type_info<this_t> == typeid(*a)){
			const auto p = down_cast<this_t*>(a.get());
			if(_before_size==p->_before_size){
				if(_before!=p->_before)
					base_t::equivalent_optimization(_before, p->_before);
				if(_after!=p->_after)
					base_t::equivalent_optimization(_after, p->_after);
			}
		}
		_before.reset();
		_after.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _before_size+_after_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		if(pos<_before_size){
			const auto copy_before_begin=pos;
			const auto copy_before_end=min(pos+size,_before_size);
			const auto copy_before_size=copy_before_end-copy_before_begin;
			_before->copy_part_data_to(to,copy_before_begin,copy_before_size);
			if(size!=copy_before_size){
				const auto copy_after_size=size-copy_before_size;
				to+=copy_before_size;
				_after->copy_part_data_to(to,0,copy_after_size);
			}
		}
		else
			_after->copy_part_data_to(to,pos-_before_size,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		if(index<_before_size)
			return _before->arec(index);
		else
			return _after->arec(index-_before_size);
	}
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		if(this->is_unique()){
			if(index<_before_size)
				_before->arec_set(index,a,_before);
			else
				_after->arec_set(index-_before_size,a,_after);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			_before=_before->apply_str_to_begin(str);
			_before_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			_before=_before->apply_str_to_begin(str);
			_before_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			_after=_after->apply_str_to_end(str);
			_after_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow)override final{
		if(this->is_unique()){
			_after=_after->apply_str_to_end(str);
			_after_size+=str->get_size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _before_size>=size){
			ptr_t aret;
			if(_before_size==size)
				swap(aret,_before);
			else
				aret=_before->do_pop_front(size,_before);
			_before_size-=size;
			if(!_before_size)
				be_replace_as(_after);
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _after_size>=size){
			ptr_t aret;
			if(_before_size==size)
				swap(aret,_after);
			else
				aret=_after->do_pop_back(size,_after);
			_after_size-=size;
			if(!_after_size)
				be_replace_as(_before);
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}

	virtual hash_t get_hash_detail(ptr_t&p)noexcept(hash_nothrow)override final{
		#if defined(_MSC_VER)
			#pragma warning(push)
			#pragma warning(disable:26494)//未初始化警告diss
		#endif
		hash_t result;
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		if(_before_size){
			result=_before->get_hash(_before);
			if(_after_size)
				result=hash.merge_array_hash_results(result,_before_size,_after->get_hash(_after),_after_size);
		}
		else
			result=_after->get_hash(_after);
		return result;
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos<_before_size){
			const auto calculate_before_begin=pos;
			const auto calculate_before_end=min(pos+size,_before_size);
			const auto calculate_before_size=calculate_before_end-calculate_before_begin;
			before=_before->get_others_hash_with_calculated_before(before,before_size,_before,calculate_before_begin,calculate_before_size);
			if(size!=calculate_before_size){
				const auto calculate_after_size=size-calculate_before_size;
				before_size+=calculate_before_size;
				before=_after->get_others_hash_with_calculated_before(before,before_size,_after,0,calculate_after_size);
			}
		}
		else
			before=_after->get_others_hash_with_calculated_before(before,before_size,_after,pos-_before_size,size);
		return before;
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept override final{
		return (sizeof(*this)+_before->get_memory_cost()+_after->get_memory_cost())/get_ref_num((const base_t*)this);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow){
	return get<sum_string_data_t<char_T>>(this,str);
}
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow){
	return get<sum_string_data_t<char_T>>(str,this);
}

//file_end

