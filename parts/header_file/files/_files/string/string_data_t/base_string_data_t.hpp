//base_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct base_string_data_t:type_info_t<base_string_data_t<char_T>>::template_name
with_common_attribute<abstract_base,never_in_array,replace_able,ref_able>,build_by_get_only{
	typedef base_string_data_t<char_T> this_t;
	typedef comn_ptr_t<this_t> ptr_t;
	typedef array_like_view_t<const char_T> string_view_t;

	base_string_data_t()noexcept=default;
	base_string_data_t(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }

	[[nodiscard]]bool is_unique()noexcept{ return get_ref_num(this)==1; }
	virtual void be_replace_as(ptr_t a)=0;

	virtual ~base_string_data_t()=default;

	[[nodiscard]]virtual char_T* get_c_str();
	[[nodiscard]]virtual const char_T* get_const_c_str(){return get_c_str();}
	[[nodiscard]]virtual const char_T* get_data(){return get_c_str();}//不要求以0结尾
	[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&);
	[[nodiscard]]virtual size_t get_size()=0;
	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size);
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str);
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str);
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str);
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str);

	[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str);
	[[nodiscard]]virtual ptr_t do_insert(size_t pos,ptr_t str);
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size);

	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self){
		auto pos	= this->get_size()-size;
		auto defore = get_substr_data(0,pos);
		auto after	= get_substr_data(pos,size);
		self		= defore;
		return after;
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self){
		auto pos	= size;
		auto defore = get_substr_data(0,pos);
		auto after	= get_substr_data(pos,this->get_size()-size);
		self		= after;
		return defore;
	}
	/*
	TODO:

	find
	rfind
	find_first_of
	find_first_not_of
	find_last_of
	find_last_not_of

	compare
	starts_with
	ends_with
	contains
	replace
	*/

	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)=0;
	[[nodiscard]]virtual char_T arec(size_t index)=0;
	virtual void arec_set(size_t index,char_T a,ptr_t&p)=0;

	[[nodiscard]]virtual double get_memory_cost()=0;
	[[nodiscard]]double get_memory_cost_after_gc();
	[[nodiscard]]double get_gc_profit(){return get_memory_cost()-get_memory_cost_after_gc();}
	//for gc:
	/*
	[[nodiscard]]virtual bool gc()=0{
		bool need_be_replace=get_gc_profit()>=0
		if(need_be_replace)
			this->be_replace_as(get<comn_string_data_t>(this));
		return need_be_replace;
	}
	*/
	static inline void equivalent_optimization(ptr_t& a,ptr_t& b){
		if(a->get_memory_cost() >= b->get_memory_cost())
			a.do_replace(b);
		else
			b.do_replace(a);
	}
};
template<typename char_T>
void base_string_data_t<char_T>::be_replace_as(ptr_t a){
	replace_able<this_t>::be_replace_as(a.get());
}

//file_end

