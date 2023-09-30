//end_apply_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
template<typename char_T>
struct end_apply_string_data_t final:base_string_data_t<char_T>,instance_struct<end_apply_string_data_t<char_T>>{
	typedef end_apply_string_data_t<char_T> this_t;
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
		copy_assign[_used_size](note::from(end.begin()),note::to(_m.data()));
	}
	end_apply_string_data_t(ptr_t str,size_t count,char_T ch)noexcept:
		_to_size(str->get_size()),
		_used_size(count),
		_to(str)
	{
		_m.resize(get_next_gold_size_to_resize_for_array(_to_size+_used_size));
		copy_assign[_used_size](ch,note::to(_m.data()));
	}
	end_apply_string_data_t(ptr_t str,size_t count)noexcept:
		_to_size(str->get_size()),
		_used_size(count),
		_to(str)
	{
		_m.resize(get_next_gold_size_to_resize_for_array(_to_size+_used_size));
	}
	end_apply_string_data_t(ptr_t str,pre_alloc_t,size_t end_count)noexcept(construct_nothrow):
		_to_size(str->get_size()),
		_used_size(0),
		_to(str)
	{
		_m.resize(end_count);
	}

	void marge_same_value_type()noexcept{
		while(type_info<this_t> == typeid(*_to) && _to->is_unique()){
			//合并重复的end_apply_string_data_t以防树状结构过深
			const auto p=down_cast<this_t*>(_to.get());
			const auto p_begin=(const char_T*)(p->_m.begin());
			const auto p_size=p->_used_size;
			if(_m.size()-_used_size>=p_size)
				copy_assign[p_size](note::from<const char_T*>(p_begin),note::to<char_T*>(_m.data()+_used_size));
			else{
				const auto size_now=this->get_size();
				const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
				_m.insert_with_resize(_used_size,p_size,p_begin,size_new);
			}
			_to_size=p->_to_size;
			_used_size+=p_size;
			_to=p->_to;
		}
	}
	void self_changed()noexcept{
		marge_same_value_type();
		#if defined(_MSC_VER) && defined(ELC_STRING_CHECKING_NOT_INITED_CHARS)
		push_and_disable_msvc_warning(4310);//截断常量警告diss
		for(size_t i=0;i<_used_size;i++)
			if(_m[i]==char_T(0xCDCDCDCD))
				__debugbreak();
		pop_msvc_warning();
		#endif
		base_t::self_changed();
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept override final{
		if(begin+size<=_to_size)
			return _to->get_substr_data(begin,size);
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
			if(pos<_to_size){
				_to=_to->do_insert(pos-_used_size,str);
				_to_size=_to->get_size();
			}
			else{
				pos-=_to_size;
				if(_m.size()-_used_size>=str.size()){
					copy_assign[_used_size-pos](note::from<const char_T*>(&_m[pos]),note::to((char_T*)&_m[pos+str.size()]));
					copy_assign[str.size()](note::from<const char_T*>(str.begin()),note::to<char_T*>(&_m[pos]));
				}
				else{
					const auto size_now=this->get_size()+str.size();
					const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
					_m.insert_with_resize(pos,str.size(),str.begin(),size_new);
				}
				_used_size+=str.size();
			}
			self_changed();
			return this;
		}
		else
			return base_t::do_insert(pos,str);
	}
protected:
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
			copy_assign[size](note::from((const char_T*)_m+pos),note::to(to));
		}
	}
public:
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
			if(_m.size()-_used_size>=str.size())
				copy_assign[str.size()](note::from(str.begin()),note::to(_m.data()+_used_size));
			else{
				const auto size_now=this->get_size()+str.size();
				const auto size_new=get_next_gold_size_to_resize_for_array(size_now);
				_m.insert_with_resize(_used_size,str.size(),str.begin(),size_new);
			}
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
	[[nodiscard]]virtual ptr_t do_remove_front(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		/*
		if(size>=_to_size)
			return get<comn_string_data_t<char_T>>(string_view_t{_m.begin(),_used_size+_to_size-size});
		*/
		if(this->is_unique() && size<_to_size){
			_to=_to->do_remove_front(size);
			_to_size-=size;
			self_changed();
			return this;
		}
		else
			return base_t::do_remove_front(size);
	}
	[[nodiscard]]virtual ptr_t do_remove_back(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(size>=_used_size)
			return _to->do_remove_back(size-_used_size);
		if(this->is_unique()){//&& size<_used_size
			_used_size-=size;
			self_changed();
			return this;
		}
		else
			return base_t::do_remove_back(size);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique() && size<_to_size){
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
			return get<comn_string_data_t<char_T>>(string_view_t{_m.data()+_used_size,size});
		}
		else
			return base_t::do_pop_back(size,self);
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&)noexcept(hash_nothrow)override final{
		return hash.with_calculated_before(_to->get_hash(_to),_to_size,string_view_t(_m.begin(),_used_size));
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&,size_t pos,size_t size)noexcept(hash_nothrow)override final{
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
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t with)noexcept override final{
		auto wp = down_cast<this_t*>(with.get());
		return _used_size==wp->_used_size;// && _to_size==wp->_to_size; //总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept override final{
		if(begin < _to_size)
			return _to->get_the_largest_complete_data_block_begin_form(begin);
		else{
			begin-=_to_size;
			return{_m.begin()+begin,note::size(_used_size-begin)};
		}
	}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		return _to->equal_with_same_size(wp->_to) && equal((char_T*)_m.begin(),(char_T*)wp->_m.begin(),_used_size);
	}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		if(auto tmp=_to->compare_with_same_size(wp->_to); tmp!=0)
			return tmp;
		return compare((char_T*)_m.begin(),(char_T*)wp->_m.begin(),_used_size);
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{
		return _to->get_memory_cost()+float_size_of(*this)+_m.size_in_byte();
	}
public:
	[[nodiscard]]virtual ptr_t pre_alloc_after_end(size_t size)noexcept override final{
		if(this->is_unique()){
			const auto size_new=_used_size+size;
			if(size_new>_m.size())
				_m.resize(size_new);
			return this;
		}
		return base_t::pre_alloc_after_end(size);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow){
	if(str.size())
		return get<end_apply_string_data_t<char_T>>(this,str);
	else
		return this;
}
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::pre_alloc_after_end(size_t size)noexcept{
	if(size)
		return get<end_apply_string_data_t<char_T>>(this,pre_alloc,size);
	else
		return this;
}

//file_end

