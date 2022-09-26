//sum_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct same_value_compress_string_data_t final:base_string_data_t<char_T>,instance_struct<same_value_compress_string_data_t<char_T>>{
	typedef same_value_compress_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;
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

	size_t _size;
	char_T _value;

	same_value_compress_string_data_t(size_t size,char_T value)noexcept:_size(size),_value(value){}

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{
		base_t::be_replace_as(a);
	}
	[[nodiscard]]virtual ptr_t get_substr_data([[maybe_unused]]size_t begin,size_t size)noexcept override final{
		return get<same_value_compress_string_data_t<char_T>>(size,_value);
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
	virtual void arec_set(size_t index,char_T a,ptr_t& p)noexcept(copy_assign_nothrow&&move_construct_nothrow)override final{
		base_t::arec_set(index,a,p);
	}
	[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t&)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return this;
		}
		else
			return get<same_value_compress_string_data_t<char_T>>(_size-size,_value);
	}
	[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t&)noexcept(construct_nothrow&&copy_assign_nothrow)override final{
		if(this->is_unique()){
			_size-=size;
			self_changed();
			return this;
		}
		else
			return get<same_value_compress_string_data_t<char_T>>(_size-size,_value);
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
		return true;//总size被保证一样
	}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form([[maybe_unused]]size_t begin)noexcept override final{
		if constexpr(construct<char_T>.trivial<const char_T&> && destruct.trivial<char_T>){
			constexpr size_t data_size=512;
			static char_T data[data_size];
			if(data[0]!=_value)
				copy_assign[data_size](_value,note::to(data));
			return {data,note::size(min(_size-begin,data_size))};
		}
		else
			return {&_value,note::size<size_t>(1)};
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
		return float_size_of(*this);
	}
};

//file_end

