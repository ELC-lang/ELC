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

	array_t<char_T> _m;//意义为向前延续的数组：当需要向前附加内容时向前拓展以避免重新分配内存
	ptr_t _to;
	size_t _to_size;
	size_t _used_size;

	head_apply_string_data_t(ptr_t str,string_view_t head):
		_to_size(str->get_size()),
		_used_size(head.size()),
		_to(str)
	{
		_m.resize(size_t((_to_size+_used_size)*magic_number::gold_of_resize));
		copy_assign[_used_size](note::form((const char_T*)head.begin()),note::to((char_T*)_m.end()-_used_size));
	}

	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)override final{
		if(begin>=_used_size)
			return _to->get_substr_data(begin-_used_size,size);
		else
			return base_t::get_substr_data(begin,size);
	}
	virtual void be_replace_as(ptr_t a)override final{
		if(type_info<this_t> == typeid(*a)){
			auto p=static_cast<this_t*>(a.get());
			if(_used_size==p->_used_size && _to!=p->_to)
				base_t::equivalent_optimization(_to, p->_to);
		}
		_m.clear();
		_to.reset();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _used_size+_to_size; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
		if(pos<_used_size){
			const char_T* head_begin=_m.end()-_used_size;
			const char_T* head_end=_m.end();
			const char_T* copy_begin=pos+head_begin;
			size_t size_of_copy_from_head=min(size_t(head_end-copy_begin),size);

			copy_assign[size_of_copy_from_head](note::form(copy_begin),note::to(to));
			if(size!=size_of_copy_from_head){
				size_t size_left=size-size_of_copy_from_head;
				char_T* next_copy_begin_pos=to+size_of_copy_from_head;
				_to->copy_part_data_to(next_copy_begin_pos,0,size_left);
			}
		}
		else
			_to->copy_part_data_to(to,pos-_used_size,size);
	}
	[[nodiscard]]virtual char_T arec(size_t index)override final{
		if(index<_used_size)
			return _m[index];
		else
			return _to->arec(index-_used_size);
	}
	virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
		if(this->is_unique())
			if(index<_used_size)
				_m[index]=a;
			else
				_to->arec_set(index-_used_size,a,_to);
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
		if(this->is_unique()){
			if(_m.size()-_used_size<str.size()){
				auto size_now=this->get_size()+str.size();
				auto size_new=size_t(size_now*magic_number::gold_of_resize);
				_m.forward_resize(size_new);
			}
			_used_size+=str.size();
			copy_assign[str.size()](note::form((const char_T*)str.begin()),note::to((char_T*)_m.end()-_used_size));
			return this;
		}
		else
			return base_t::apply_str_to_begin(str);
	}
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
		this->shrink();
		return base_t::apply_str_to_begin(str);
	}
	void shrink(){
		_m.forward_resize(_used_size);
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
		if(this->is_unique() && _used_size>=size){
			_used_size-=size;
			return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m.end()-_used_size-size,size});
		}
		else
			return base_t::do_pop_front(size,self);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self) override final{
		if(this->is_unique()){
			auto aret=_to->do_pop_back(size,_to);
			_to_size-=size;
			return aret;
		}
		else
			return base_t::do_pop_back(size,self);
	}

	[[nodiscard]]virtual size_t get_memory_cost()override final{
		return sizeof(*this)+_m.size()*sizeof(char_T)+_to->get_memory_cost();
	}
};
template<typename char_T>
[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(string_view_t str){
	return get<head_apply_string_data_t<char_T>>(this,str);
}

//file_end

