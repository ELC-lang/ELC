//comn_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct comn_string_data_t final:base_string_data_t<char_T>,instance_struct<comn_string_data_t<char_T>>{
	typedef comn_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;

	array_t<char_T> _m;

	comn_string_data_t(string_view_t str):_m(note::size(str.size()+1)){
		copy_assign[str.size()](note::form(str.begin()),note::to((char_T*)_m));
	}
	comn_string_data_t(ptr_t str):_m(note::size(str->get_size()+1)){
		str->copy_part_data_to((char_T*)_m,0,this->get_size());
	}
	comn_string_data_t(ptr_t str,size_t pos,size_t size):_m(note::size(size+1)){
		str->copy_part_data_to((char_T*)_m,pos,size);
	}

	virtual void be_replace_as(ptr_t a)override final{
		_m.clear();
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual char_T* get_c_str()override final{ return (char_T*)_m; }
	[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&a)override final{
		if(this->is_unique())
			return (char_T*)_m;
		else
			return base_t::get_unique_c_str(a);
	}
	[[nodiscard]]virtual size_t get_size()override final{ return _m.size()-1; }
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ copy_assign[size](note::form((const char_T*)_m+pos),note::to(to)); }
	[[nodiscard]]virtual char_T arec(size_t index)override final{ return _m[index]; }
	virtual void arec_set(size_t index,char_T a,ptr_t&p)override final{
		if(this->is_unique())
			_m[index]=a;
		else
			base_t::arec_set(index,a,p);
	}
};
template<typename char_T>
[[nodiscard]]char_T* base_string_data_t<char_T>::get_c_str(){
	auto comn_data=get<comn_string_data_t<char_T>>(this);
	this->be_replace_as(comn_data);
	return comn_data->get_c_str();
}
template<typename char_T>
[[nodiscard]]char_T* base_string_data_t<char_T>::get_unique_c_str(ptr_t&a){
	auto comn_data=get<comn_string_data_t<char_T>>(this);
	a=comn_data;
	return comn_data->get_c_str();
}
template<typename char_T>
void base_string_data_t<char_T>::arec_set(size_t index,char_T a,ptr_t& p){
	this->get_unique_c_str(p)[index]=a;
}

//file_end

