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
	class iterator_t:helper_struct{
		template<typename,typename>
		friend class iterator_t;

		typedef iterator_t<value_t,base_t>this_t;
		//
		base_t _m;
		//
		[[nodiscard]]inline value_t*handle_getter()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m;
			elseif constexpr(is_pointer<T>)
				return _m->get_handle();
			else
				return _m.get_handle();
		}
		[[nodiscard]]inline base_t next_getter()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m+1;
			elseif constexpr(is_pointer<T>)
				return _m->get_next();
			else
				return _m.get_next();
		}
		[[nodiscard]]inline base_t before_getter()noexcept{
			if constexpr(type_info<decltype(*declvalue(base_t))> == type_info<value_t>)
				return _m-1;
			elseif constexpr(is_pointer<T>)
				return _m->get_before();
			else
				return _m.get_before();
		}
		constexpr this_t&operator=(base_t a)&noexcept{_m=a;return*this;}
	public:
		constexpr void swap_with(this_t&a)noexcept{swap(_m,a._m);}
		//
		constexpr iterator_t()noexcept=default;
		constexpr iterator_t(base_t a)noexcept:_m(a){}
		constexpr iterator_t(const this_t&a)noexcept:_m(a,_m){}
		constexpr iterator_t(this_t&&a)noexcept{swap_with(a);}
		template<typename other_T,typename other_base_t,enable_if(construct<base_t>.able<other_base_t>)>
		constexpr iterator_t(const iterator_t<other_T,base_t>&a)noexcept(construct<base_t>.nothrow<other_base_t>):_m(a._m){}
		~iterator_t()noexcept=default;
		[[nodiscard]]constexpr bool operator==(this_t a)const noexcept{return _m==a._m;}
		[[nodiscard]]constexpr bool operator!=(this_t a)const noexcept{return!operator==(a);}
		[[nodiscard]]constexpr bool operator==(value_t*a)const noexcept{return handle_getter()==a;}
		[[nodiscard]]constexpr bool operator!=(value_t*a)const noexcept{return!operator==(a);}
		constexpr this_t&operator++()&noexcept{return*this=next_getter();}
		constexpr this_t&operator--()&noexcept{return*this=before_getter();}
		constexpr this_t operator++(int)&noexcept{auto a=*this;operator++();return a;}
		constexpr this_t operator--(int)&noexcept{auto a=*this;operator--();return a;}
		[[nodiscard]]constexpr value_t*operator->()noexcept{return handle_getter();}
		[[nodiscard]]constexpr value_t&operator*()noexcept{return*operator->();}
		[[nodiscard]]constexpr operator value_t*()noexcept{return operator->();}
	};
	template<typename T>
	inline void swap(iterator_t<T>&a,iterator_t<T>&b)noexcept{a.swap_with(b);}

	template<typename value_t,typename base_t>
	iterator_t(iterator_t<value_t,base_t>) -> iterator_t<value_t,base_t>;
	template<typename value_t>
	iterator_t(value_t*) -> iterator_t<value_t>;
}
using iterator_n::iterator_t;

//file_end

