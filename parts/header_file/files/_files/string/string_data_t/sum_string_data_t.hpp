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

	ptr_t  _defore;
	ptr_t  _after;
	size_t _defore_size;
	size_t _after_size;

	sum_string_data_t(ptr_t defore,ptr_t after):_defore(defore),_after(after),_defore_size(_defore->get_size()),_after_size(_after->get_size()){}

	virtual void be_replace_as(ptr_t a)override final{
		if(type_info<this_t> == typeid(*a)){
			auto p=static_cast<this_t*>(a.get());
			if(_defore_size==p->_defore_size){
				if(_defore!=p->_defore)
					base_t::equivalent_optimization(_defore, p->_defore);
				if(_after!=p->_after)
					base_t::equivalent_optimization(_after, p->_after);
			}
		}
		_defore.reset();
		_after.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _defore_size+_after_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
		if(pos<_defore_size){
			auto copy_defore_begin=pos;
			auto copy_defore_end=min(pos+size,_defore_size);
			auto copy_defore_size=copy_defore_end-copy_defore_begin;
			_defore->copy_part_data_to(to,copy_defore_begin,copy_defore_size);
			if(size!=copy_defore_size){
				auto copy_after_size=size-copy_defore_size;
				to+=copy_defore_size;
				_after->copy_part_data_to(to,0,copy_after_size);
			}
		}
		else
			_after->copy_part_data_to(to,pos-_defore_size,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)override final{
		if(index<_defore_size)
			return _defore->arec(index);
		else
			return _after->arec(index-_defore_size);
	}
	virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
		if(this->is_unique())
			if(index<_defore_size)
				_defore->arec_set(index,a,_defore);
			else
				_after->arec_set(index-_defore_size,a,_after);
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
		if(this->is_unique()){
			_defore=_defore->apply_str_to_begin(str);
			_defore_size+=str.size();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
		if(this->is_unique()){
			_defore=_defore->apply_str_to_begin(str);
			_defore_size+=str->get_size();
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
		if(this->is_unique()){
			_after=_after->apply_str_to_end(str);
			_after_size+=str.size();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
		if(this->is_unique()){
			_after=_after->apply_str_to_end(str);
			_after_size+=str->get_size();
			return this;
		}
		else
			return base_t::apply_str_to_end(str);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self) override final{
		if(this->is_unique() && _defore_size>=size){
			auto aret=_defore->do_pop_front(size,_defore);
			_defore_size-=size;
			return aret;
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self) override final{
		if(this->is_unique() && _after_size>=size){
			auto aret=_after->do_pop_back(size,_after);
			_after_size-=size;
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}

	[[nodiscard]]virtual size_t get_memory_cost()override final{
		return sizeof(*this)+_defore->get_memory_cost()+_after->get_memory_cost();
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(ptr_t str){
	return get<sum_string_data_t<char_T>>(this,str);
}
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(ptr_t str){
	return get<sum_string_data_t<char_T>>(str,this);
}

//file_end

