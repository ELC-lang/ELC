//base_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
no_vtable_struct base_string_data_t:type_info_t<base_string_data_t<char_T>>::template_name
	with_common_attribute<abstract_base,never_in_array,replace_able,ref_able>,build_by_get_only{
	typedef base_string_data_t<char_T> this_t;
	typedef comn_ptr_t<this_t> ptr_t;
	typedef array_like_view_t<const char_T> string_view_t;

	static constexpr bool copy_assign_nothrow=copy_assign.nothrow<char_T>;
	static constexpr bool copy_construct_nothrow=copy_construct.nothrow<char_T>;
	static constexpr bool move_construct_nothrow=move_construct.nothrow<char_T>;
	static constexpr bool construct_nothrow=construct<char_T>.nothrow<>;
	static constexpr bool destruct_nothrow=destruct.nothrow<char_T>;
	static constexpr bool clear_nothrow=destruct_nothrow;
	static constexpr bool ptr_reset_nothrow=destruct_nothrow;
	static constexpr bool hash_nothrow=hash.nothrow<char_T>;
	static constexpr bool get_data_nothrow=copy_construct_nothrow&&destruct_nothrow;
	static constexpr bool apply_data_nothrow=construct_nothrow&&copy_assign_nothrow;

	base_string_data_t()noexcept=default;
	base_string_data_t(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }

	[[nodiscard]]bool is_unique()noexcept{ return get_ref_num(this)==1; }
	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)=0;

	virtual ~base_string_data_t()noexcept(destruct_nothrow)=default;

	[[nodiscard]]virtual char_T* get_c_str(ptr_t&)noexcept(get_data_nothrow);
	[[nodiscard]]virtual const char_T* get_const_c_str(ptr_t&p)noexcept(get_data_nothrow){return get_c_str(p);}
	[[nodiscard]]virtual const char_T* get_data(ptr_t&p)noexcept(get_data_nothrow){return get_c_str(p);}//不要求以0结尾
	[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&)noexcept(get_data_nothrow);
	[[nodiscard]]virtual size_t get_size()noexcept=0;
	[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)noexcept;
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow);
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow);
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow);
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept(apply_data_nothrow);

	[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str)noexcept(copy_construct_nothrow);
	[[nodiscard]]virtual ptr_t do_insert(size_t pos,ptr_t str)noexcept;
	[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)noexcept;

	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow){
		const auto pos	  = this->get_size()-size;
		const auto before = get_substr_data(0,pos);
		const auto after  = get_substr_data(pos,size);
		self			  = before;
		return after;
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self)noexcept(construct_nothrow&&copy_assign_nothrow){
		const auto pos	  = size;
		const auto before = get_substr_data(0,pos);
		const auto after  = get_substr_data(pos,this->get_size()-size);
		self			  = after;
		return before;
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

	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)=0;
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)=0;
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)=0;

	hash_t hash_cache=hash(-1);
	bool has_hash_cache()noexcept{return hash_cache!=hash(-1);}
	void reset_hash_cache()noexcept{hash_cache=hash(-1);}
	hash_t get_hash(ptr_t&p)noexcept(hash_nothrow){
		if(has_hash_cache())
			return hash_cache;
		else{
			auto tmp=this->get_hash_detail(p);
			return p->hash_cache=tmp;
		}
	}
	virtual hash_t get_hash_detail(ptr_t&p)noexcept(hash_nothrow){
		const auto size=get_size();
		return hash(get_data(p),size);
	}
	hash_t get_others_hash_with_calculated_before(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow){
		if(pos==0&&size==get_size())
			return hash.merge_array_hash_results(before,before_size,get_hash(p),size);
		return this->get_others_hash_with_calculated_before_detail(before,before_size,p,pos,size);
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow){
		return hash.with_calculated_before(before,before_size,get_data(p)+pos,size);
	}

	void self_changed()noexcept{
		reset_hash_cache();
	}

	[[nodiscard]]virtual float_size_t get_memory_cost()noexcept=0;
	[[nodiscard]]float_size_t get_memory_cost_after_gc()noexcept;
	[[nodiscard]]float_size_t get_gc_profit()noexcept{return get_memory_cost()-get_memory_cost_after_gc();}
	[[nodiscard]]bool positive_gc_profit()noexcept{return get_gc_profit() > 0;}
	//for gc:
	/*
	[[nodiscard]]virtual bool gc()=0{
		bool need_be_replace=get_gc_profit()>=0
		if(need_be_replace)
			this->be_replace_as(get<comn_string_data_t>(this));
		return need_be_replace;
	}
	*/
	static inline void equivalent_optimization(ptr_t& a,ptr_t& b)noexcept(ptr_reset_nothrow){
		if(a->get_memory_cost() >= b->get_memory_cost())
			a.do_replace(b);
		else
			b.do_replace(a);
	}
};
template<typename char_T>
void base_string_data_t<char_T>::be_replace_as(ptr_t a)noexcept(clear_nothrow){
	replace_able<this_t>::be_replace_as(a.get());
}

//file_end

