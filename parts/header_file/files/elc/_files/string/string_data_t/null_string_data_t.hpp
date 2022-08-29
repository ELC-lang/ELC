//null_string_data_t.hpp
//at namespace elc::defs::string_n::string_data_n
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename char_T>
struct null_string_data_t final:base_string_data_t<char_T>,instance_struct<null_string_data_t<char_T>>{
	typedef null_string_data_t<char_T> this_t;
	typedef base_string_data_t<char_T> base_t;
	using base_t::ptr_t;
	using base_t::string_view_t;

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

	null_string_data_t()noexcept:base_t(never_ref_num_zero){}

	virtual void be_replace_as(ptr_t a)noexcept(clear_nothrow)override final{nothing;}
	[[nodiscard]]virtual char_T* get_c_str(ptr_t&)noexcept override final{
		static char_T data[1]{};
		return data;
	}
	[[nodiscard]]virtual size_t get_size()noexcept override final{ return 0; }
	[[nodiscard]]virtual ptr_t get_substr_data([[maybe_unused]]size_t begin,[[maybe_unused]]size_t size)noexcept override final{ return this; }
	[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{ return get<comn_string_data_t<char_T>>(str); }
	[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)noexcept(apply_data_nothrow)override final{ return str; }
	[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)noexcept(copy_construct_nothrow&&apply_data_nothrow)override final{ return get<comn_string_data_t<char_T>>(str); }
	[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)noexcept override final{ return str; }

	[[nodiscard]]virtual ptr_t do_insert([[maybe_unused]]size_t pos,[[maybe_unused]]string_view_t str)noexcept(construct_nothrow&&copy_assign_nothrow)override final{ return get<comn_string_data_t<char_T>>(str); }
	[[nodiscard]]virtual ptr_t do_insert([[maybe_unused]]size_t pos,[[maybe_unused]]ptr_t str)noexcept override final{ return str; }
	[[nodiscard]]virtual ptr_t do_erase([[maybe_unused]]size_t pos,[[maybe_unused]]size_t size)noexcept override final{ return this; }
protected:
	virtual void copy_part_data_to([[maybe_unused]]char_T* to,[[maybe_unused]]size_t pos,[[maybe_unused]]size_t size)noexcept override final{ return; }
public:
	[[nodiscard]]virtual char_T arec([[maybe_unused]]size_t index)noexcept override final{ return char_T{}; }
	virtual void arec_set([[maybe_unused]]size_t index,[[maybe_unused]]char_T a,[[maybe_unused]]ptr_t& p)noexcept override final{ nothing; }
	[[nodiscard]]virtual ptr_t do_pop_back([[maybe_unused]]size_t size,[[maybe_unused]]ptr_t& self)noexcept override final{ return this; }
	[[nodiscard]]virtual ptr_t do_pop_front([[maybe_unused]]size_t size,[[maybe_unused]]ptr_t& self)noexcept override final{ return this; }
protected:
	virtual hash_t get_hash_detail([[maybe_unused]]ptr_t&p)noexcept override final{ return hash(nothing); }
	virtual hash_t get_others_hash_with_calculated_before_detail([[maybe_unused]]hash_t before,[[maybe_unused]]size_t before_size,[[maybe_unused]]ptr_t&p,[[maybe_unused]]size_t pos,[[maybe_unused]]size_t size)noexcept override final{ return before; }
protected:
	[[nodiscard]]virtual bool same_struct(ptr_t)noexcept override final{return false;}
	[[nodiscard]]virtual range_t<const char_T*> get_the_largest_complete_data_block_begin_form(size_t begin)noexcept override final{return {0,0};}
	[[nodiscard]]virtual bool same_struct_equal(ptr_t with)noexcept override final{return false;}
	[[nodiscard]]virtual base_t::compare_type same_struct_compare(ptr_t with)noexcept override final{return strong_ordering::equivalent;}
	[[nodiscard]]virtual float_size_t get_base_memory_cost()noexcept override final{return 0;}
};
template<typename char_T>
distinctive inline null_string_data_t<char_T> null_string_data{};
template<typename char_T>
base_string_data_t<char_T>* the_get_null_ptr(const base_string_data_t<char_T>*)noexcept{ return&null_string_data<char_T>; }

//file_end

