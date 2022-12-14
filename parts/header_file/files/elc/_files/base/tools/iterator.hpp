//iterator.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace iterator_n{
	using namespace memory;//copy_assign、move_assign
	template<typename base_t_w>
	struct reverse_base_t{
		typedef reverse_base_t<base_t_w> this_t;
		typedef remove_cv<base_t_w> base_t_rw;
		base_t_rw _m;
		template<typename build_base_t_T> requires(construct<base_t_rw>.able<build_base_t_T>)
		constexpr reverse_base_t(build_base_t_T&& a)noexcept(construct<base_t_rw>.nothrow<build_base_t_T>):_m(a){}
		constexpr reverse_base_t(const reverse_base_t&a)noexcept(construct<base_t_rw>.nothrow<const base_t_rw&>):_m(a._m){}
		constexpr reverse_base_t(reverse_base_t&&a)noexcept(construct<base_t_rw>.nothrow<base_t_rw&&>):_m(move(a._m)){}
		constexpr reverse_base_t& operator=(const reverse_base_t&a)&noexcept{_m=a._m;return*this;}
		constexpr reverse_base_t& operator=(const reverse_base_t&&a)&noexcept{_m=move(a._m);return*this;}
		constexpr auto operator==(const reverse_base_t&a)noexcept{return _m==a._m;}
		constexpr auto operator<=>(const reverse_base_t&a)noexcept{return _m<=>a._m;}

		static constexpr bool is_pointer=::std::is_pointer_v<base_t_w>;

		[[nodiscard]]static constexpr bool is_get_before_noexcept()noexcept{
			if constexpr(is_pointer)
				return true;
			else return noexcept(declvalue(base_t_w).get_next());
		}
		[[nodiscard]]static constexpr bool is_get_next_noexcept()noexcept{
			if constexpr(is_pointer)
				return true;
			else return noexcept(declvalue(base_t_w).get_before());
		}
		[[nodiscard]]static constexpr bool is_get_handle_noexcept()noexcept{
			if constexpr(is_pointer)
				return true;
			else return noexcept(declvalue(base_t_w).get_handle());
		}

		[[nodiscard]]auto get_before()noexcept(is_get_before_noexcept()){
			if constexpr(is_pointer)
				return _m+1;
			else return ((base_t_w&)_m).get_next();
		}
		[[nodiscard]]auto get_next()noexcept(is_get_next_noexcept()){ 
			if constexpr(is_pointer)
				return _m-1;
			else return ((base_t_w&)_m).get_before();
		}
		[[nodiscard]]auto get_handle()noexcept(is_get_handle_noexcept()){ 
			if constexpr(is_pointer)
				return _m;
			else return ((base_t_w&)_m).get_handle();
		}
	};
	template<typename base_t> requires(compare.able<base_t>)
	[[nodiscard]]auto operator<=>(const reverse_base_t<base_t>&a,const reverse_base_t<base_t>&b)noexcept(compare.nothrow<base_t>){
		return compare((const base_t&)b._m,(const base_t&)a._m);
	}
	template<typename base_t,typename T> requires(compare.able<T,base_t> && type_info<remove_cvref<T>> != type_info<reverse_base_t<base_t>::base_t_rw>)
	[[nodiscard]]auto operator<=>(const reverse_base_t<base_t>& a,T&& b)noexcept(compare.nothrow<T,base_t>){
		return compare(b,(const base_t&)a._m);
	}
	template<typename base_t,typename T> requires(compare.able<base_t,T> && type_info<remove_cvref<T>> != type_info<reverse_base_t<base_t>::base_t_rw>)
	[[nodiscard]]auto operator<=>(T&& a,const reverse_base_t<base_t>& b)noexcept(compare.nothrow<base_t,T>){
		return compare((const base_t&)b._m,a);
	}
	template<typename base_t,typename T> requires(equal.able<T,base_t>)
	[[nodiscard]]auto operator==(const reverse_base_t<base_t>& a,T&& b)noexcept(equal.nothrow<T,base_t>){
		return equal(b,(const base_t&)a._m);
	}
	template<typename base_t,typename T> requires(equal.able<base_t,T>)
	[[nodiscard]]auto operator==(T&& a,const reverse_base_t<base_t>& b)noexcept(equal.nothrow<base_t,T>){
		return equal((const base_t&)b._m,a);
	}
	template<typename base_t>
	inline void swap(reverse_base_t<base_t>&a,reverse_base_t<base_t>&b)noexcept_as(swap(a._m,b._m))
	{swap(a._m,b._m);}

	template<typename value_t,typename base_t_w>
	class same_base_t{
		template<typename,typename>
		friend class same_base_t;
	protected:
		typedef same_base_t<value_t,base_t_w>this_t;
		typedef remove_cv<base_t_w> base_t_rw;
		//
		mutable base_t_rw _m;
		//
		[[nodiscard]]static constexpr bool is_handle_getter_noexcept()noexcept{
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return true;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_handle()))
				return noexcept(declvalue(base_t_w)->get_handle());
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_handle()))
				return noexcept(declvalue(base_t_w).get_handle());
		}
		[[nodiscard]]inline value_t*handle_getter()const noexcept(is_handle_getter_noexcept()){
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return ((base_t_w)_m);
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_handle()))
				return ((base_t_w)_m)->get_handle();
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_handle()))
				return ((base_t_w)_m).get_handle();
		}
		//
		[[nodiscard]]static constexpr bool is_value_getter_noexcept()noexcept{
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return true;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_value()))
				return noexcept(declvalue(base_t_w)->get_value());
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_value()))
				return noexcept(declvalue(base_t_w).get_value());
		}

		[[nodiscard]]static constexpr auto value_type_getter()noexcept{
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return type_info<decltype(*((base_t_w)_m))>;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_value()))
				return type_info<decltype(((base_t_w)_m)->get_value())>;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_value()))
				return type_info<decltype(((base_t_w)_m).get_value())>;
			else
				return type_info<decltype(*declvalue(value_t*))>;
		}
		typedef decltype(value_type_getter())::template_name type value_type;

		[[nodiscard]]inline value_type value_getter()const noexcept(is_handle_getter_noexcept()){
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return *((base_t_w)_m);
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_value()))
				return ((base_t_w)_m)->get_value();
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_value()))
				return ((base_t_w)_m).get_value();
			else
				return *handle_getter();
		}
		//
		[[nodiscard]]static constexpr bool is_next_getter_noexcept()noexcept{
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return true;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_next()))
				return noexcept(declvalue(base_t_w)->get_next());
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_next()))
				return noexcept(declvalue(base_t_w).get_next());
		}
		[[nodiscard]]inline base_t_w next_getter()const noexcept(is_next_getter_noexcept()){
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return ((base_t_w)_m)+1;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_next()))
				return ((base_t_w)_m)->get_next();
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_next()))
				return ((base_t_w)_m).get_next();
		}
		//
		[[nodiscard]]static constexpr bool is_before_getter_noexcept()noexcept{
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return true;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_before()))
				return noexcept(declvalue(base_t_w)->get_before());
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_before()))
				return noexcept(declvalue(base_t_w).get_before());
		}
		[[nodiscard]]inline base_t_w before_getter()const noexcept(is_before_getter_noexcept()){
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return ((base_t_w)_m)-1;
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w)->get_before()))
				return ((base_t_w)_m)->get_before();
			elseif constexpr(was_not_an_ill_form(declvalue(base_t_w).get_before()))
				return ((base_t_w)_m).get_before();
		}
	public:
		constexpr void swap_with(this_t&a)noexcept_as(swap(declvalue(base_t_rw&),declvalue(base_t_rw&))){swap(_m,a._m);}
		//
		constexpr same_base_t()noexcept=default;
		template<typename build_base_t_T> requires(construct<base_t_rw>.able<build_base_t_T>)
		constexpr same_base_t(build_base_t_T&&a)noexcept(construct<base_t_rw>.nothrow<build_base_t_T>):_m(a){}
		constexpr same_base_t(const this_t&a)noexcept(construct<base_t_rw>.nothrow<const base_t_rw>):_m(a._m){}
		constexpr same_base_t(this_t&&a)noexcept_as(construct<base_t_rw>.nothrow<base_t_rw&&>):_m(move(a._m)){}
		template<typename other_T,typename other_base_t> requires(construct<base_t_rw>.able<other_base_t>)
		constexpr same_base_t(const same_base_t<other_T,other_base_t>&a)noexcept(construct<base_t_rw>.nothrow<other_base_t>):_m(a._m){}
		~same_base_t()noexcept(destruct.nothrow<base_t_rw>)=default;
		[[nodiscard]]constexpr bool		  operator==(const this_t& a)const noexcept_as(declvalue(base_t_rw) == declvalue(base_t_rw)){ return _m == a._m; }
		[[nodiscard]]constexpr value_t*   operator->()noexcept(is_handle_getter_noexcept()){ return handle_getter(); }
		[[nodiscard]]constexpr value_type operator*()noexcept_as(is_value_getter_noexcept()){ return value_getter(); }
		[[nodiscard]]constexpr			  operator value_t*()noexcept_as(declvalue(this_t).operator->()){ return operator->(); }
		[[nodiscard]]constexpr bool		  operator==(value_t* a)const noexcept(is_handle_getter_noexcept()&&noexcept(pointer_equal(declvalue(value_t*),declvalue(value_t*)))){ return pointer_equal(handle_getter(),a); }
		template<typename other_value_t,typename other_base_t_w> requires(equal.able<base_t_rw,other_base_t_w>)
		[[nodiscard]]constexpr bool operator==(const same_base_t<other_value_t,other_base_t_w>& a)const noexcept(equal.nothrow<base_t_rw,other_base_t_w>){
			return equal(_m,a._m);
		}
		template<typename other_value_t,typename other_base_t_w>
		[[nodiscard]]constexpr auto operator<=>(const same_base_t<other_value_t,other_base_t_w>& a)const noexcept(compare.nothrow<base_t_rw,other_base_t_w>){
			if constexpr(compare.able<base_t_rw,other_base_t_w>)
				return compare(_m,a._m);
		}
	};
	template<typename value_t,typename base_t_w>
	inline void swap(same_base_t<value_t,base_t_w>&a,same_base_t<value_t,base_t_w>&b)noexcept_as(a.swap_with(b))
	{a.swap_with(b);}

	template<typename value_t,typename base_t_w>
	struct base_iterator_t:same_base_t<value_t,base_t_w>{
		typedef base_iterator_t<value_t,base_t_w> this_t;
		typedef same_base_t<value_t,base_t_w> base_t;
	protected:
		using base_t::is_before_getter_noexcept;
		using base_t::is_next_getter_noexcept;
		using base_t::before_getter;
		using base_t::next_getter;
		typedef remove_cv<base_t_w> base_t_rw;
	public:
		using base_t::base_t;
		constexpr this_t&operator=(const base_t_rw&a)&noexcept(copy_assign.nothrow<base_t_rw>){copy_assign(base_t::_m,a);return*this;}
		constexpr this_t&operator=(base_t_rw&&a)&noexcept(move_assign.nothrow<base_t_rw>){move_assign(base_t::_m,a);return*this;}
		constexpr this_t&operator++()&noexcept(is_next_getter_noexcept()){return*this=next_getter();}
		constexpr this_t&operator--()&noexcept(is_before_getter_noexcept()){return*this=before_getter();}
		constexpr this_t operator++(int)&noexcept_as(this_t(++declvalue(this_t&))){auto a=*this;operator++();return a;}
		constexpr this_t operator--(int)&noexcept_as(this_t(--declvalue(this_t&))){auto a=*this;operator--();return a;}
		constexpr this_t operator+(ptrdiff_t num)const noexcept_as(this_t(--declvalue(this_t&)),++declvalue(this_t&)){
			if constexpr(type_info<::std::remove_pointer_t<base_t_w>> == type_info<value_t>)
				return base_t::_m+num;
			else{
				auto a=*this;
				if(num>0)
					while(num--)
						++a;
				else
					while(num++)
						--a;
				return a;
			}
		}
		constexpr this_t operator-(ptrdiff_t num)const noexcept_as(declvalue(this_t&)+0){ return *this+(-num); }
	};
	template<typename value_t,typename base_t_w>
	base_iterator_t(base_iterator_t<value_t,base_t_w>) -> base_iterator_t<value_t,base_t_w>;
	template<typename value_t,typename base_t_w>
	base_iterator_t(value_t*) -> base_iterator_t<value_t,base_t_w>;

	template<typename value_t,typename base_t_w=value_t*>
	using iterator_t = base_iterator_t<remove_cvref<value_t>,base_t_w>;
	template<typename value_t,typename base_t_w=const value_t*>
	using const_iterator_t = base_iterator_t<const remove_cvref<value_t>,base_t_w>;
	template<typename value_t,typename base_t_w=value_t*>
	using reverse_iterator_t = base_iterator_t<remove_cvref<value_t>,reverse_base_t<base_t_w>>;
	template<typename value_t,typename base_t_w=const value_t*>
	using reverse_const_iterator_t = base_iterator_t<const remove_cvref<value_t>,reverse_base_t<base_t_w>>;
}
using iterator_n::base_iterator_t;
using iterator_n::iterator_t;
using iterator_n::const_iterator_t;
using iterator_n::reverse_iterator_t;
using iterator_n::reverse_const_iterator_t;

//file_end

