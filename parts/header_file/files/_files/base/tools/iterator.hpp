//iterator.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace iterator_n{
	template<typename value_t,typename base_t=value_t*>
	class iterator_t{
		template<typename,typename>
		friend class iterator_t;

		typedef iterator_t<value_t,base_t>this_t;
		//
		base_t _m;
		//
		constexpr bool is_handle_getter_noexcept()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return 1;
			elseif constexpr(is_pointer<base_t>)
				return noexcept(declvalue(base_t)->get_handle());
			else
				return noexcept(declvalue(base_t).get_handle());
		}
		[[nodiscard]]inline value_t*handle_getter()noexcept(is_handle_getter_noexcept()){
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m;
			elseif constexpr(is_pointer<base_t>)
				return _m->get_handle();
			else
				return _m.get_handle();
		}
		//
		constexpr bool is_next_getter_noexcept()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return 1;
			elseif constexpr(is_pointer<base_t>)
				return noexcept(declvalue(base_t)->get_next());
			else
				return noexcept(declvalue(base_t).get_next());
		}
		[[nodiscard]]inline base_t next_getter()noexcept(is_next_getter_noexcept()){
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m+1;
			elseif constexpr(is_pointer<base_t>)
				return _m->get_next();
			else
				return _m.get_next();
		}
		//
		constexpr bool is_before_getter_noexcept()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return 1;
			elseif constexpr(is_pointer<base_t>)
				return noexcept(declvalue(base_t)->get_before());
			else
				return noexcept(declvalue(base_t).get_before());
		}
		[[nodiscard]]inline base_t before_getter()noexcept(is_before_getter_noexcept()){
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m-1;
			elseif constexpr(is_pointer<base_t>)
				return _m->get_before();
			else
				return _m.get_before();
		}
	public:
		constexpr void swap_with(this_t&a)noexcept_as(swap(declvalue(base_t),declvalue(base_t))){swap(_m,a._m);}
		//
		constexpr iterator_t()noexcept=default;
		constexpr iterator_t(base_t a)noexcept(construct<base_t>.nothrow<base_t>):_m(a){}
		constexpr iterator_t(const this_t&a)noexcept(construct<base_t>.nothrow<const base_t>):_m(a._m){}
		constexpr iterator_t(this_t&&a)noexcept_as(declvalue(this_t).swap_with(declvalue(this_t))){swap_with(a);}
		template<typename other_T,typename other_base_t,enable_if(construct<base_t>.able<other_base_t>)>
		constexpr iterator_t(const iterator_t<other_T,other_base_t>&a)noexcept(construct<base_t>.nothrow<other_base_t>):_m(a._m){}
		~iterator_t()noexcept(destruct.nothrow<base_t>)=default;
		[[nodiscard]]constexpr bool operator==(this_t a)const noexcept_as(declvalue(base_t)==declvalue(base_t)){return _m==a._m;}
		[[nodiscard]]constexpr bool operator!=(this_t a)const noexcept_as(declvalue(this_t)==declvalue(this_t)){return!operator==(a);}
		constexpr this_t&operator++()&noexcept(is_next_getter_noexcept()){return*this=next_getter();}
		constexpr this_t&operator--()&noexcept(is_before_getter_noexcept()){return*this=before_getter();}
		constexpr this_t operator++(int)&noexcept_as(this_t(++declvalue(this_t))){auto a=*this;operator++();return a;}
		constexpr this_t operator--(int)&noexcept_as(this_t(--declvalue(this_t))){auto a=*this;operator--();return a;}
		[[nodiscard]]constexpr value_t*operator->()noexcept(is_handle_getter_noexcept()){return handle_getter();}
		[[nodiscard]]constexpr value_t&operator*()noexcept_as(declvalue(this_t).operator->()){return*operator->();}
		[[nodiscard]]constexpr operator value_t*()noexcept_as(declvalue(this_t).operator->()){return operator->();}
		[[nodiscard]]constexpr bool operator==(value_t*a)const noexcept(is_handle_getter_noexcept()&&noexcept(pointer_equal(declvalue(value_t*),declvalue(value_t*)))){return pointer_equal(handle_getter(),a);}
		[[nodiscard]]constexpr bool operator!=(value_t*a)const noexcept_as(declvalue(this_t)==declvalue(value_t*)){return!operator==(a);}
	};
	template<typename value_t,typename base_t>
	inline void swap(iterator_t<value_t,base_t>&a,iterator_t<value_t,base_t>&b)noexcept_as(declvalue(iterator_t<value_t,base_t>&).swap_with(declvalue(iterator_t<value_t,base_t>&))){a.swap_with(b);}

	template<typename value_t,typename base_t>
	iterator_t(iterator_t<value_t,base_t>) -> iterator_t<value_t,base_t>;
	template<typename value_t>
	iterator_t(value_t*) -> iterator_t<value_t>;
}
using iterator_n::iterator_t;

//file_end

