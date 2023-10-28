//other.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
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
class empty_type{};

template<typename T,typename U>
inline auto&assign(T&&a,U&&b)noexcept_as(a=b){//为绕过条件内赋值时の警告而使用
	a=b;
	return a;
}

//fucking nodiscard.
constexpr struct discard_t{
	template<typename T>
	force_inline constexpr void operator=(T&&)const noexcept{}
	template<typename T>
	force_inline constexpr void operator()(T&&)const noexcept{}
}discard{};

template<typename T>
distinctive inline T const_default_value_of{};//?

enable_adl(the_destroy);
template<typename T>
static void destroy(T*a)noexcept{
	if constexpr(was_not_an_ill_form(the_destroy(a)))
		the_destroy(a);
	else
		template_error("Please overload the function the_destroy in the namespace where this type is defined.");
}

constexpr struct do_nothing_t{
	template<class...Args>
	void operator()(Args&&...)noexcept{}
	constexpr bool operator==(const do_nothing_t&)noexcept{return true;}
	template<typename T>
	constexpr bool operator==(T&)noexcept{return false;}
}do_nothing;

//swap
template<typename T1,typename T2>
inline constexpr_as_auto void swap(T1&a,T2&b)noexcept_as_auto{
	using ::std::move;
	auto tmp=move(b);
	b=move(a);
	a=move(tmp);
}
/*
如何不用中间变量swap两个值？
方法一：
a=a+b
b=a-b
a=a-b
方法二：
a^=b^=a^=b
道理我都懂，可是还是用中间变量快些
XD
更多方法欢迎补充
*/
template<typename T1,typename T2>
inline constexpr_as_auto [[nodiscard]]T1 swap(T1&a,const T2&b)noexcept_as_auto{
	using ::std::move;
	T1 tmp=move(a);
	a=b;
	return tmp;
}

template<typename T>
struct times_provider_t{
	typedef times_provider_t<T> this_t;
	T loop_times=0u;
	T loop_end_value;
	struct iterator_t{
		this_t* _m;
		constexpr iterator_t& operator++()noexcept{
			++_m->loop_times;
			return*this;
		}
		constexpr this_t& operator*()noexcept{
			return*_m;
		}
		constexpr bool operator==(auto)noexcept{
			return _m->loop_times==_m->loop_end_value;
		}
	};
	constexpr iterator_t begin(){
		return {this};
	}
	constexpr auto end(){return 0;}
	constexpr times_provider_t(const T&v)noexcept:loop_end_value(v){}
};

//file_end

