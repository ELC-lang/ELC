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
	typedef string_view_t<char_T> string_view_t;

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

	friend class null_string_data_t<char_T>;
	friend class constexpr_string_data_t<char_T>;
	friend class comn_string_data_t<char_T>;
	friend class substr_string_data_t<char_T>;
	friend class head_apply_string_data_t<char_T>;
	friend class end_apply_string_data_t<char_T>;
	friend class sum_string_data_t<char_T>;
	friend class erased_string_data_t<char_T>;
	friend class inserted_string_data_t<char_T>;

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

	starts_with
	ends_with
	contains
	replace
	*/
	typedef compare_t::type<char_T> compare_type;
protected:
	bool same_type(ptr_t a)noexcept{return typeid(*this)==typeid(*a);}
	[[nodiscard]]virtual bool same_struct(ptr_t)noexcept=0;
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)=0;
	[[nodiscard]]virtual compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)=0;
protected:
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept=0;
public:
	[[nodiscard]]bool equal_with(ptr_t with)noexcept(equal.nothrow<char_T>){
		//eq->equal优化
		if(this==with)
			return true;
		//hash_diff->not_equal优化
		if(this->has_hash_cache()&&with->has_hash_cache())
			if(this->hash_cache!=with->hash_cache)
				return false;
		//size比较优化被移至string_t实现内部：原因：same_struct_compare大部分情况下size相同。
		//快速比较结束，实际比较段
		{
			#if defined(_MSC_VER)
				#pragma warning(push)
				#pragma warning(disable:26494)//未初始化警告diss
			#endif
			bool aret;
			#if defined(_MSC_VER)
				#pragma warning(pop)
			#endif
			{
				//同结构比较优化比较方式
				if(same_type(with)&&same_struct(with))
					aret=same_struct_equal(with);
				//否则默认比较
				else
					aret=default_equal_method(with);
			}
			//如果equal，则eq处理
			if(aret)
				equivalent_optimization(this,with);
			return aret;
		}
	}
protected:
	[[nodiscard]]bool default_equal_method(ptr_t with)noexcept(equal.nothrow<char_T>){
		const size_t self_size=get_size();
		return equal_with(with,0,self_size);
	}
public:
	[[nodiscard]]bool equal_with(ptr_t with,size_t pos,size_t size)noexcept(equal.nothrow<char_T>){
		//eq->equal优化
		if(this==with)
			return true;
		size_t index=pos;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		auto b=with->get_the_largest_complete_data_block_begin_form(index);
		while(size){
			const size_t step=min({a.size(),b.size(),size});
			if(a.begin()!=b.begin())//起始地址不同时才需要真的比较
				if(!equal(a.begin(),b.begin(),step))
					return false;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(b.size()==step)
				b=with->get_the_largest_complete_data_block_begin_form(index);
			else
				b={b.begin()+step,note::size(b.size()-step)};
			if(!a.size()&&!b.size())
				return a.size()==b.size();
		}
		return true;
	}
	[[nodiscard]]bool equal_with(string_view_t with)noexcept(equal.nothrow<char_T>){
		size_t size=with.size();
		size_t index=0;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		if(a.begin()==with.begin() && a.size()==with.size())
			return true;
		floop{
			auto b=with.begin()+index;
			const size_t step=min({a.size(),with.size()-index,size});
			if(!equal(a.begin(),b,step))
				return false;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(!a.size()||!size)
				return a.size()==size;
		}
	}
	[[nodiscard]]bool equal_with(const char_T*with)noexcept(equal.nothrow<char_T>){
		size_t size=get_size();
		size_t index=0;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		if(a.begin()==with && with[a.size()]==char_T{})
			return true;
		floop{
			auto b=with+index;
			if(!*b)
				return false;
			size_t step=min({a.size(),size});
			if(!equal(a.begin(),step,just_an_part,b,end_by_zero))
				return false;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(!*(b+step))
				return !a.size();
			if(!a.size())
				return !*(b+step);
		}
	}
public:
	[[nodiscard]]compare_type compare_with(ptr_t with)noexcept(compare.nothrow<char_T>){
		//eq->equal优化
		if(this==with)
			return strong_ordering::equivalent;
		//size比较优化被移至string_t实现内部：原因：same_struct_compare大部分情况下size相同。
		//快速比较结束，实际比较段
		{
			#if defined(_MSC_VER)
				#pragma warning(push)
				#pragma warning(disable:26494)//未初始化警告diss
			#endif
			compare_type aret;
			#if defined(_MSC_VER)
				#pragma warning(pop)
			#endif
			{
				//同结构比较优化比较方式
				if(same_type(with)&&same_struct(with))
					aret=same_struct_compare(with);
				//否则默认比较
				else
					aret=default_compare_method(with);
			}
			//如果equal，则eq处理
			if(aret==0)
				equivalent_optimization(this,with);
			return aret;
		}
	}
protected:
	[[nodiscard]]compare_type default_compare_method(ptr_t with)noexcept(compare.nothrow<char_T>){
		const size_t self_size=get_size();
		return compare_with(with,0,self_size);
	}
public:
	[[nodiscard]]compare_type compare_with(ptr_t with,size_t pos,size_t size)noexcept(compare.nothrow<char_T>){
		//eq->equal优化
		if(this==with)
			return strong_ordering::equivalent;
		size_t index=pos;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		auto b=with->get_the_largest_complete_data_block_begin_form(index);
		while(size){
			const size_t step=min({a.size(),b.size(),size});
			if(a.begin()!=b.begin())//起始地址不同时才需要真的比较
				if(auto tmp=compare(a.begin(),b.begin(),step); tmp!=0)
					return tmp;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(b.size()==step)
				b=with->get_the_largest_complete_data_block_begin_form(index);
			else
				b={b.begin()+step,note::size(b.size()-step)};
			if(!a.size()||!b.size())
				return a.size()<=>b.size();
		}
		return strong_ordering::equivalent;
	}
	[[nodiscard]]compare_type compare_with(string_view_t with)noexcept(compare.nothrow<char_T>){
		size_t size=with.size();
		size_t index=0;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		if(a.begin()==with.begin() && a.size()==with.size())
			return strong_ordering::equivalent;
		floop{
			auto b=with.begin()+index;
			size_t step=min({a.size(),with.size()-index,size});
			if(auto tmp=compare(a.begin(),b,step); tmp!=0)
				return tmp;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(!a.size()||!size)
				return a.size()<=>size;
		}
	}
	[[nodiscard]]compare_type compare_with(const char_T*with)noexcept(compare.nothrow<char_T>){
		size_t size=get_size();
		size_t index=0;
		auto a=this->get_the_largest_complete_data_block_begin_form(index);
		if(a.begin()==with && with[a.size()]==char_T{})
			return strong_ordering::equivalent;
		floop{
			auto b=with+index;
			if(!*b)
				return strong_ordering::greater;
			size_t step=min({a.size(),size});
			if(auto tmp=compare(a.begin(),step,just_an_part,b,end_by_zero); tmp!=0)
				return tmp;
			index+=step;
			size-=step;
			if(a.size()==step)
				a=this->get_the_largest_complete_data_block_begin_form(index);
			else
				a={a.begin()+step,note::size(a.size()-step)};
			if(!a.size())
				return *(with+index)==char_T{}? strong_ordering::equivalent:
												strong_ordering::less;
			if(!*(with+index))
				return strong_ordering::greater;
		}
	}
protected:
	virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)noexcept(copy_assign_nothrow)=0;
public:
	[[nodiscard]]virtual char_T arec(size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)=0;
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)=0;
protected:
	hash_t hash_cache=hash(-1);
	bool has_hash_cache()noexcept{return hash_cache!=hash(-1);}
	void reset_hash_cache()noexcept{hash_cache=hash(-1);}
public:
	hash_t get_hash(ptr_t&p)noexcept(hash_nothrow){
		if(has_hash_cache())
			return hash_cache;
		else{
			const auto tmp=this->get_hash_detail(p);
			return p->hash_cache=tmp;
		}
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&p)noexcept(hash_nothrow){
		const auto size=get_size();
		return hash(get_data(p),size);
	}
public:
	hash_t get_others_hash_with_calculated_before(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow){
		if(pos==0&&size==get_size())
			return hash.merge_array_hash_results(before,before_size,get_hash(p),size);
		return this->get_others_hash_with_calculated_before_detail(before,before_size,p,pos,size);
	}
protected:
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&p,size_t pos,size_t size)noexcept(hash_nothrow){
		return hash.with_calculated_before(before,before_size,get_data(p)+pos,size);
	}
protected:
	void self_changed()noexcept{
		reset_hash_cache();
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept=0;
public:
	[[nodiscard]]float_size_t get_memory_cost()noexcept{
		return get_base_memory_cost()/get_ref_num(this);
	}
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

	[[nodiscard]]virtual const range_n::match_pattern<const char_T>& get_match_pattern_from_self(ptr_t&self)noexcept(copy_assign_nothrow&&move_construct_nothrow);
	[[nodiscard]]virtual const range_n::reverse_match_pattern<const char_T>& get_reverse_match_pattern_from_self(ptr_t&self)noexcept(copy_assign_nothrow&&move_construct_nothrow);

protected:
	static inline void be_replace(this_t* a,ptr_t b)noexcept(ptr_reset_nothrow){
		a->be_replace_as(b);
	}
	static inline void be_replace(ptr_t& a,ptr_t b)noexcept(ptr_reset_nothrow){
		a.do_replace(b);
	}
	static inline void equivalent_optimization(auto&& a,auto&& b)noexcept(ptr_reset_nothrow){
		if(a->get_memory_cost() >= b->get_memory_cost())
			be_replace(a,b);
		else
			be_replace(b,a);
	}
};
template<typename char_T>
void base_string_data_t<char_T>::be_replace_as(ptr_t a)noexcept(clear_nothrow){
	replace_able<this_t>::be_replace_as(a.get());
}

//file_end

