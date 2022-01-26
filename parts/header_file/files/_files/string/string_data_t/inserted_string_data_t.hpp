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

	ptr_t  _to;
	ptr_t  _insert_data;
	size_t _to_size;
	size_t _insert_pos;
	size_t _insert_size;

	inserted_string_data_t(ptr_t to,ptr_t insert_data,size_t insert_pos):_to(to),_insert_data(insert_data),_insert_pos(insert_pos),_to_size(to->get_size()),_insert_size(insert_data->get_size()){}

	virtual void be_replace_as(ptr_t a)override final{
		_to.reset();
		_insert_data.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _to_size+_insert_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
		if(pos+size<_insert_pos)
			_to->copy_part_data_to(to,pos,size);
		elseif(pos>_insert_pos+_insert_size)
			_to->copy_part_data_to(to,pos-_insert_size,size);
		else{
			if(_insert_pos>pos){
				auto size_defore_insert_pos=_insert_pos-pos;
				_to->copy_part_data_to(to,pos,size_defore_insert_pos);
				to+=size_defore_insert_pos;
				size-=size_defore_insert_pos;
			}
			auto size_defore_insert_end=min(size,_insert_size);
			_insert_data->copy_part_data_to(to,0,size_defore_insert_end);
			to+=size_defore_insert_end;
			size-=size_defore_insert_end;
			if(size)
				_to->copy_part_data_to(to,_insert_pos,size);
		}
	}
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)override final{
		if(this->is_unique()){
			if(pos>=_insert_pos && pos+size<=_insert_pos+_insert_size){
				_insert_data=_insert_data->do_erase(pos-_insert_pos,size);
				_insert_size-=size;
				return this;
			}
			elseif(pos+size<_insert_pos){
				_to=_to->do_erase(pos,size);
			}
			elseif(pos>_insert_pos+_insert_size){
				_to=_to->do_erase(pos-_insert_size,size);
			}
		}
		return base_t::do_erase(pos,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)override final{
		if(index>=_insert_pos && index<_insert_pos+_insert_size)
			return _insert_data->arec(index-_insert_pos);
		elseif(index>=_insert_pos+_insert_size)
			return _to->arec(index-_insert_size);
		else
			return _to->arec(index);
	}

	virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
		if(this->is_unique())
			if(index>=_insert_pos && index<_insert_pos+_insert_size)
				_insert_data->arec_set(index-_insert_pos,a,p);
			elseif(index>=_insert_pos+_insert_size)
				_to->arec_set(index-_insert_size,a,p);
			else
				_to->arec_set(index,a,p);
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
		if(this->is_unique()){
			if(_insert_pos==0){
				_insert_data=_insert_data->apply_str_to_begin(str);
				_insert_size+=str.size();
			}
			else{
				_to=_to->apply_str_to_begin(str);
				_to_size+=str.size();
			}
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
		if(this->is_unique()){
			if(_insert_pos==0){
				_insert_data=_insert_data->apply_str_to_begin(str);
				_insert_size+=str->get_size();
			}
			else{
				_to=_to->apply_str_to_begin(str);
				_to_size+=str->get_size();
			}
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
		if(this->is_unique()){
			if(_insert_pos==_to_size){
				_insert_data=_insert_data->apply_str_to_end(str);
				_insert_size+=str.size();
			}
			else{
				_to=_to->apply_str_to_end(str);
				_to_size+=str.size();
			}
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
		if(this->is_unique()){
			if(_insert_pos==_to_size){
				_insert_data=_insert_data->apply_str_to_end(str);
				_insert_size+=str->get_size();
			}
			else{
				_to=_to->apply_str_to_end(str);
				_to_size+=str->get_size();
			}
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self) override final{
		if(this->is_unique()){
			if(_insert_pos > size){
				auto aret=_to->do_pop_front(size,_to);
				_to_size-=size;
				_insert_pos-=size;
				return aret;
			}
			elseif(_insert_pos==0 && _insert_size>=size){
				auto aret=_insert_data->do_pop_front(size,_insert_data);
				_insert_size-=size;
			}
		}
		return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self) override final{
		if(this->is_unique()){
			if(_insert_pos+_insert_size <= _to_size-size){
				auto aret=_to->do_pop_back(size,_to);
				_to_size-=size;
				return aret;
			}
			elseif(_insert_pos==_to_size && _insert_size>=size){
				auto aret=_insert_data->do_pop_front(size,_insert_data);
				_insert_size-=size;
			}
		}
		return base_t::do_pop_back(size,self);
	}

	[[nodiscard]]virtual size_t get_memory_cost()override final{
		return sizeof(*this)+_insert_data->get_memory_cost()+_to->get_memory_cost();
	}
};
template<typename char_T>
base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos,ptr_t str){
	return get<inserted_string_data_t<char_T>>(this,str,pos);
}
template<typename char_T>
base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos,string_view_t str){
	return this->do_insert(pos,get<comn_string_data_t<char_T>>(str));
}

//file_end

