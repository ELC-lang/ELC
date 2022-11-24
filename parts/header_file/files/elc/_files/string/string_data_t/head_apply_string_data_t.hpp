//end_apply_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct head_apply_string_data_t final:base_string_data_t<char_T>,instance_struct<head_apply_string_data_t<char_T>>{
	typedef head_apply_string_data_t<char_T> this_t;
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

	array_t<char_T> _m;//意义为向前延续的数组：当需要向前附加内容时向前拓展以避免重新分配内存
	ptr_t _to;
	size_t _to_size;
	size_t _used_size;

	head_apply_string_data_t(ptr_t str,string_view_t head)noexcept(construct_nothrow&&copy_assign_nothrow):
		_to_size(str->get_size()),
		_used_size(head.size()),
		_to(str)
	{
		_m.resize(get_next_gold_size_to_resize_for_array(_to_size+_used_size));
		copy_assign[_used_size](note::from<const char_T*>(head.begin()),note::to<char_T*>(_m.end()-_used_size));
	}
	head_apply_string_data_t(ptr_t str,pre_alloc_t,size_t head_size)noexcept(construct_nothrow):
		_to_size(str->get_size()),
		_used_size(0),
		_to(str)
	{
		_m.resize(head_size);
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin>=_used_size)
			return _to->get_substr_data(begin-_used_size,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	[[nodiscard]]virtual string_ptr_t get_c_str(ptr_t&p)noexcept(get_data_nothrow)override final{
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
	[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str)noexcept(copy_construct_nothrow)override final{
		if(pos==0)
			return this->apply_str_to_begin(str);
		elseif(pos==get_size())
			return this->apply_str_to_end(str);
		elseif(this->is_unique()){
			if(pos<_used_size){
				if(_m.size()-_used_size>=str.size()){
					char_T* orogin_head_begin=_m.end()-_used_size;
					char_T* head_begin=orogin_head_begin-str.size();
					copy_assign[pos](note::from<const char_T*>(orogin_head_begin),note::to<char_T*>(head_begin));
					copy_assign[str.size()](note::from<const char_T*>(str.begin()),note::to<char_T*>(head_begin+pos));
				}
				else{
					const auto size_now=this->get_size()+str.size();
					const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
					_m.insert_with_forward_resize(pos,str.size(),str.begin(),size_new);
				}
				_used_size+=str.size();
			}else{
				_to=_to->do_insert(pos-_used_size,str);
				_to_size=_to->get_size();
			}
			self_changed();
			return this;
		}
		else
			return base_t::do_insert(pos,str);
	}
protected:
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		if(pos<_used_size){
			const char_T* head_begin=_m.end()-_used_size;
			const char_T* head_end=_m.end();
			const char_T* copy_begin=pos+head_begin;
			size_t size_of_copy_from_head=min(size_t(head_end-copy_begin),size);

			copy_assign[size_of_copy_from_head](note::from(copy_begin),note::to(to));
			if(size!=size_of_copy_from_head){
				const size_t size_left=size-size_of_copy_from_head;
				char_T* next_copy_begin_pos=to+size_of_copy_from_head;
				_to->copy_part_data_to(next_copy_begin_pos,0,size_left);
			}
		}
		else
			_to->copy_part_data_to(to,pos-_used_size,size);
	}
public:
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		if(index<_used_size){
			const char_T* head_begin=_m.end()-_used_size;
			return head_begin[index];
		}
		else
			return _to->arec(index-_used_size);
	}
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		if(this->is_unique()){
			if(index<_used_size){
				char_T* head_begin=_m.end()-_used_size;
				copy_assign(head_begin[index],a);
			}
			else
				_to->arec_set(index-_used_size,a,_to);
			self_changed();
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{
		if(this->is_unique()){
			if(_m.size()-_used_size>=str.size())
				copy_assign[str.size()](note::from<const char_T*>(str.begin()),note::to<char_T*>(_m.end()-_used_size-str.size()));
			else{
				const auto size_now=this->get_size()+str.size();
				const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
				_m.insert_with_forward_resize(0,str.size(),str.begin(),size_new);
			}
			_used_size+=str.size();
			self_changed();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{
		this->shrink();
		return base_t::apply_str_to_begin(str);
	}
	void shrink()noexcept(_m.forward_resize_nothrow){
		_m.forward_resize(_used_size);
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
	[[nodiscard]]virtual ptr_t do_remove_front(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(size>=_used_size)
			return _to->do_remove_front(size-_used_size);
		if(this->is_unique()){//&& size<_used_size
			_used_size-=size;
			self_changed();
			return this;
		}
		else
			return base_t::do_remove_front(size);
	}
	[[nodiscard]]virtual ptr_t do_remove_back(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		/*
		if(size>=_to_size){
			const auto size_left=_used_size+_to_size-size;
			return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m.end()-_used_size,size_left});
		}
		*/
		if(this->is_unique() && size<_to_size){
			_to=_to->do_remove_back(size);
			_to_size-=size;
			self_changed();
			return this;
		}
		else
			return base_t::do_remove_back(size);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && _used_size>=size){
			_used_size-=size;
			self_changed();
			return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m.end()-_used_size-size,size});
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			auto aret=_to->do_pop_back(size,_to);
			_to_size-=size;
			self_changed();
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&)noexcept(hash_nothrow)override final{
		suppress_msvc_warning(26494)//未初始化警告diss
		hash_t result;
		if(_used_size){
			const char_T* head_begin=_m.end()-_used_size;
			result=hash(head_begin,_used_size);
			if(_to_size)
				result=hash.merge_array_hash_results(result,_used_size,_to->get_hash(_to),_to_size);
		}
		else
			result=_to->get_hash(_to);
		return result;
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&,size_t pos,size_t size)noexcept(hash_nothrow)override final{
		if(pos<_used_size){
			const char_T* head_begin=_m.end()-_used_size;
			const char_T* head_end=_m.end();
			const char_T* calculate_begin=pos+head_begin;
			const size_t size_of_calculate_from_head=min(size_t(head_end-calculate_begin),size);

			before=hash.with_calculated_before(before,before_size,calculate_begin,size_of_calculate_from_head);
			if(size!=size_of_calculate_from_head){
				const size_t size_left=size-size_of_calculate_from_head;
				before_size+=size_of_calculate_from_head;
				before=_to->get_others_hash_with_calculated_before(before,before_size,_to,0,size_left);
			}
		}
		else
			before=_to->get_others_hash_with_calculated_before(before,before_size,_to,pos-_used_size,size);
		return before;
	}
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t with)noexcept override final{
		auto wp=down_cast<this_t*>(with.get());
		return _used_size==wp->_used_size;// && _to_size==wp->_to_size; //总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept override final{
		if(begin >= _used_size)
			return _to->get_the_largest_complete_data_block_begin_form(begin-_used_size);
		else {
			const char_T* head_begin = _m.end() - _used_size;
			const char_T* head_end	 = _m.end();
			const char_T* ret_begin	 = begin+head_begin;
			return {ret_begin, head_end};
		}
	}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		const char_T* head_begin = _m.end() - _used_size;
		const char_T* wp_head_begin = wp->_m.end() - _used_size;
		return equal(head_begin,wp_head_begin,_used_size) && _to->equal_with_same_size(wp->_to);
	}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		const char_T* head_begin = _m.end() - _used_size;
		const char_T* wp_head_begin = wp->_m.end() - _used_size;
		if(auto tmp=compare(head_begin,wp_head_begin,_used_size); tmp!=0)
			return tmp;
		return _to->compare_with_same_size(wp->_to);
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{
		return _to->get_memory_cost()+float_size_of(*this)+_m.size_in_byte();
	}
public:
	[[nodiscard]]virtual ptr_t pre_alloc_before_begin(size_t size)noexcept override final{
		if(this->is_unique()){
			const auto size_new=_used_size+size;
			if(size_new>_m.size())
				_m.forward_resize(size_new);
			return this;
		}
		return base_t::pre_alloc_before_begin(size);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow){
	if(str.size())
		return get<head_apply_string_data_t<char_T>>(this,str);
	else
		return this;
}
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::pre_alloc_before_begin(size_t size)noexcept{
	if(size)
		return get<head_apply_string_data_t<char_T>>(this,pre_alloc,size);
	else
		return this;
}

//file_end

