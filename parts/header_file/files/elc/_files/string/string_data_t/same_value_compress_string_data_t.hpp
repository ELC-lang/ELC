//sum_string_data_t.hpp
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
struct same_value_compress_string_data_t final:base_string_data_t<char_T>,instance_struct<same_value_compress_string_data_t<char_T>>{
	typedef same_value_compress_string_data_t<char_T> this_t;
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

	size_t _size;
	char_T _value;
	ptr_t _static_data_p;//必要时创建一个较大的数组，用于向外提供数据
	static constexpr size_t max_static_data_size=size_t(pow(BIT_POSSIBILITY, 9));

	same_value_compress_string_data_t(size_t size,char_T value)noexcept:_size(size),_value(value){}
	same_value_compress_string_data_t(size_t size,char_T value,ptr_t static_data_p)noexcept:_size(size),_value(value),_static_data_p(static_data_p){}
	ptr_t get_clone_with_new_size(size_t new_size)noexcept{
		if(new_size==_size)return this;
		if(new_size==0)return nullptr;
		return get<same_value_compress_string_data_t<char_T>>(new_size,_value,_static_data_p);
	}

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual ptr_t get_substr_data([[maybe_unused]]size_t begin,size_t size)noexcept override final{
		return get_clone_with_new_size(size);
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return _size; }
protected:
	virtual void copy_part_data_to(char_T* to,[[maybe_unused]]size_t pos,size_t size)noexcept(copy_assign_nothrow)override final{
		copy_assign[size](_value,note::to(to));
	}
public:
	[[nodiscard]]virtual char_T arec([[maybe_unused]]size_t index)noexcept(copy_construct_nothrow&&move_construct_nothrow)override final{
		return _value;
	}
	void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		//小优化
		if(a==_value)return;
		if(_size==1){_value=a;_static_data_p.reset();return;}
		//若_size>max_static_data_size，将当前类型转换为sum_string_data_t
		if(_size>max_static_data_size){
			//中间一段为comn_string_data_t，大小为max_static_data_size
			ptr_t tmp=get<comn_string_data_t<char_T>>(max_static_data_size,_value);
			//尽量确保index在中间一段的中间
			const size_t index_in_tmp=min(index,max_static_data_size/2);
			//将中间一段的index处的值改为a
			tmp->arec_set(index_in_tmp,a,tmp);
			//判断是否需要附加其他段
			if(index>max_static_data_size/2){
				const size_t first_size=index-max_static_data_size/2;
				ptr_t first_p=get_clone_with_new_size(first_size);
				tmp=get<sum_string_data_t<char_T>>(first_p,tmp);
			}
			const size_t last_size=_size-tmp->get_size();
			if(last_size){
				ptr_t last_p=get_clone_with_new_size(last_size);
				tmp=get<sum_string_data_t<char_T>>(tmp,last_p);
			}
			p=tmp;
			return;
		}
		else
			base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t do_remove_front(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return this;
		}
		else
			return get_clone_with_new_size(_size-size);
	}
	[[nodiscard]]virtual ptr_t do_remove_back(size_t size)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return this;
		}
		else
			return get_clone_with_new_size(_size-size);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t&self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return get_clone_with_new_size(size);
		}
		else{
			auto aret=get_clone_with_new_size(size);
			self=get_clone_with_new_size(_size-size);
			return aret;
		}
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t&self)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return get_clone_with_new_size(size);
		}
		else{
			auto aret=get_clone_with_new_size(size);
			self=get_clone_with_new_size(_size-size);
			return aret;
		}
	}
protected:
	virtual hash_t get_hash_detail(ptr_t&)noexcept(hash_nothrow)override final{
		return hash.repeat_times(_value,_size);
	}
	virtual hash_t get_others_hash_with_calculated_before_detail(hash_t before,size_t before_size,ptr_t&,[[maybe_unused]]size_t pos,size_t size)noexcept(hash_nothrow)override final{
		return hash.merge_array_hash_results(before,before_size,hash.repeat_times(_value,size),size);
	}
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t with)noexcept override final{
		auto wp=down_cast<this_t*>(with.get());
		if(wp->_value==_value && wp->_static_data_p!=_static_data_p){
			if(_static_data_p)
				wp->_static_data_p.do_replace(_static_data_p);
			elseif(wp->_static_data_p)
				_static_data_p.do_replace(wp->_static_data_p);
		}
		return true;//总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form([[maybe_unused]]size_t begin)noexcept override final{
		if constexpr(construct<char_T>.trivial<const char_T&> && destruct.trivial<char_T>){
			if(!_static_data_p)
				_static_data_p=get<comn_string_data_t<char_T>>(max_static_data_size,_value);
			return{_static_data_p->get_data(_static_data_p),note::size(min(_size-begin,max_static_data_size))};
		}
		else
			return{&_value,note::size<size_t>(1)};
	}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept(equal.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		return equal(_value,wp->_value);
	}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept(compare.nothrow<char_T>)override final{
		auto wp=down_cast<this_t*>(with.get());
		return compare(_value,wp->_value);
	}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{
		return float_size_of(*this)+_static_data_p->get_memory_cost();
	}
};

//file_end

