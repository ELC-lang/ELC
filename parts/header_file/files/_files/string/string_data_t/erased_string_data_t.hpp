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

	ptr_t  _to;
	size_t _to_size;
	size_t _erase_pos;
	size_t _erase_size;

	erased_string_data_t(ptr_t str,size_t erase_pos,size_t erase_size):_to(str),_to_size(_to->get_size()),_erase_pos(erase_pos),_erase_size(erase_size){}

	virtual void be_replace_as(ptr_t a)override final{
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _to_size-_erase_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
		if(pos+size<_erase_pos)
			_to->copy_part_data_to(to,pos,size);
		elseif(pos>_erase_pos)
			_to->copy_part_data_to(to,pos+_erase_size,size);
		else{
			auto size_defore_erase_pos=_erase_pos-pos;
			auto size_after_erase_pos=size-size_defore_erase_pos;
			_to->copy_part_data_to(to,pos,size_defore_erase_pos);
			_to->copy_part_data_to(to+size_defore_erase_pos,_erase_pos+_erase_size,size_after_erase_pos);
		}
	}
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)override final{
		if(this->is_unique()){
			if(pos<=_erase_pos && pos+size>=_erase_pos+_erase_size){
				_erase_pos=pos;
				_erase_size+=size;
				return this;
			}
		}
		return base_t::do_erase(pos,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)override final{
		if(index>_erase_pos)
			return _to->arec(index+_erase_size);
		else
			return _to->arec(index);
	}

	virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
		if(this->is_unique())
			if(index>_erase_pos)
				return _to->arec_set(index+_erase_size,a,_to);
			else
				return _to->arec_set(index,a,_to);
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			auto strsize=str.size();
			_to_size+=strsize;
			_erase_pos+=strsize;
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_begin(str);
			auto strsize=str->get_size();
			_to_size+=strsize;
			_erase_pos+=strsize;
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str.size();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
		if(this->is_unique()){
			_to=_to->apply_str_to_end(str);
			_to_size+=str->get_size();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self) override final{
		if(this->is_unique() && _erase_pos > size){
			auto aret=_to->do_pop_front(size,_to);
			_to_size-=size;
			_erase_pos-=size;
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self) override final{
		if(this->is_unique() && _erase_pos+_erase_size <= _to_size-size){
			auto aret=_to->do_pop_back(size,_to);
			_to_size-=size;
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size){
	return get<erased_string_data_t<char_T>>(this,pos,size);
}

//file_end
