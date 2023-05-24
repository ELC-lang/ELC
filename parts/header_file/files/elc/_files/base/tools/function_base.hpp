//function_base.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
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
namespace function_base_n{
	#if !defined(_MSC_VER)
	template<typename>
	struct function_type_getter_helper{};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)&noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const&noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename T>
	[[nodiscard]]constexpr auto function_type_getter_conditional_helper()noexcept{
		if constexpr(::std::is_function_v<remove_cvref<T>>)
			return type_info<remove_cvref<T>>;
		elseif constexpr(is_pointer<T> && ::std::is_function_v<remove_cvref<::std::remove_pointer_t<T>>>)//不支持多级函数指针：懒得写（其实挺简单的），而且function_t若支持多级函数指针的推导指引会很不安全
			return function_type_getter_conditional_helper<::std::remove_pointer_t<T>>();
		else
			return type_info<type_name function_type_getter_helper<decltype(&T::operator())>::type>;
	}
	template<typename T>
	struct function_type_getter{
		typedef decltype(function_type_getter_conditional_helper<T>())::template_name type type;
	};

	/*
	获取一个callable类型的“函数类型”
	如T(int)返回char，其函数类型便是char(int)
	*/
	template<typename T>
	using get_function_type=function_type_getter<T>::type;

	/*
	用处：容纳一个callable类型（除过函数类型，但可以是其指针）并提供operator()
	小包装工具，能在意想不到的地方发挥效果（比如要额外携带数据的callable类型定义！见"../../lib_loader/lib_loader.hpp"）
	*/
	template<class T,class Func_t=get_function_type<T>>
	class function_data_wrapper_t;
	template<class T,bool nothrow,class Ret_t,class...Args_t>
	struct function_data_wrapper_t<T,Ret_t(Args_t...)noexcept(nothrow)>{
		static_assert(!::std::is_function_v<remove_cvref<T>>);

		T _value;

		function_data_wrapper_t(T a)noexcept(construct<T>.nothrow<T>):_value(a){}
		~function_data_wrapper_t()noexcept(destruct.nothrow<T>)=default;
		Ret_t operator()(Args_t...args)noexcept(nothrow || invoke<T>.with_return_type<Ret_t>.nothrow<Args_t...>){
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.able<Args_t...>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		Ret_t operator()(Args_t...args)const noexcept(nothrow || invoke<const T>.with_return_type<Ret_t>.nothrow<Args_t...>){
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.able<Args_t...>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		[[nodiscard]]auto&get_data()noexcept{return _value;}
	};
	#else
	//MSVC，我滴垃圾堆
	template<typename>
	struct function_type_getter_helper{};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)&>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const&>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)&noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_type_getter_helper<Ret_t(T::*)(Args_t...)const&noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename T>
	[[nodiscard]]constexpr auto function_type_getter_conditional_helper()noexcept{
		if constexpr(::std::is_function_v<remove_cvref<T>>)
			return type_info<remove_cvref<T>>;
		elseif constexpr(is_pointer<T> && ::std::is_function_v<remove_cvref<::std::remove_pointer_t<T>>>)//不支持多级函数指针：懒得写（其实挺简单的），而且function_t若支持多级函数指针的推导指引会很不安全
			return function_type_getter_conditional_helper<::std::remove_pointer_t<T>>();
		else
			return type_info<type_name function_type_getter_helper<decltype(&T::operator())>::type>;
	}
	template<typename T>
	struct function_type_getter{
		typedef decltype(function_type_getter_conditional_helper<T>())::template_name type type;
	};

	/*
	获取一个callable类型的“函数类型”
	如T(int)返回char，其函数类型便是char(int)
	*/
	template<typename T>
	using get_function_type=function_type_getter<T>::type;

	/*
	用处：容纳一个callable类型（除过函数类型，但可以是其指针）并提供operator()
	小包装工具，能在意想不到的地方发挥效果（比如要额外携带数据的callable类型定义！见"../../lib_loader/lib_loader.hpp"）
	*/
	template<class T,class Func_t=get_function_type<T>>
	class function_data_wrapper_t;
	template<class T,class Ret_t,class...Args_t>
	struct function_data_wrapper_t<T,Ret_t(Args_t...)noexcept>{
		static_assert(!::std::is_function_v<remove_cvref<T>>);

		T _value;

		function_data_wrapper_t(T a)noexcept(construct<T>.nothrow<T>):_value(a){}
		~function_data_wrapper_t()noexcept(destruct.nothrow<T>)=default;
		Ret_t operator()(Args_t...args)noexcept{
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.able<Args_t...>)
				template_error("the return type of T was wrong.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.nothrow<Args_t...>)
				template_warning("this T may throw an exception.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		Ret_t operator()(Args_t...args)const noexcept{
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.able<Args_t...>)
				template_error("the return type of T was wrong.");
			if constexpr(!invoke<T>.with_return_type<Ret_t>.nothrow<Args_t...>)
				template_warning("this T may throw an exception.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		[[nodiscard]]auto&get_data()noexcept{return _value;}
	};
	/*
	用处：容纳一个callable类型（除过函数类型，但可以是其指针）并提供operator()
	小包装工具，能在意想不到的地方发挥效果（比如要额外携带数据的callable类型定义！见"../../lib_loader/lib_loader.hpp"）
	*/
	template<class T,class Ret_t,class...Args_t>
	struct function_data_wrapper_t<T,Ret_t(Args_t...)>{
		static_assert(!::std::is_function_v<remove_cvref<T>>);

		T _value;

		function_data_wrapper_t(T a)noexcept(construct<T>.nothrow<T>):_value(a){}
		~function_data_wrapper_t()noexcept(destruct.nothrow<T>)=default;
		Ret_t operator()(Args_t...args)noexcept(invoke<T>.with_return_type<Ret_t>.nothrow<Args_t...>){
			//BLOCK:constexpr checks
			//if constexpr(!invoke<T>.able<Args_t...>)
			//workaround of https://developercommunity.visualstudio.com/t/name-lookup-problem-after-update-1740/10189035
			if constexpr(!::std::is_invocable_v<T,Args_t...>)
				template_error("this T can\'t becall as args.");
			//if constexpr(!invoke<T>.with_return_type<Ret_t>.able<Args_t...>)
			//workaround of https://developercommunity.visualstudio.com/t/name-lookup-problem-after-update-1740/10189035
			if constexpr(!::std::is_invocable_r_v<Ret_t,T,Args_t...>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		Ret_t operator()(Args_t...args)const noexcept(invoke<const T>.with_return_type<Ret_t>.nothrow<Args_t...>){
			//BLOCK:constexpr checks
			//if constexpr(!invoke<const T>.able<Args_t...>)
			//workaround of https://developercommunity.visualstudio.com/t/name-lookup-problem-after-update-1740/10189035
			if constexpr(!::std::is_invocable_v<const T, Args_t...>)
				template_error("this T can\'t becall as args.");
			//if constexpr(!invoke<const T>.with_return_type<Ret_t>.able<Args_t...>)
			//workaround of https://developercommunity.visualstudio.com/t/name-lookup-problem-after-update-1740/10189035
			if constexpr(!::std::is_invocable_r_v<Ret_t,const T,Args_t...>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			if constexpr(type_info<Ret_t> != type_info<void>)
				return _value(forward<Args_t>(args)...);
			else
				_value(forward<Args_t>(args)...);
		}
		[[nodiscard]]auto&get_data()noexcept{return _value;}
		[[nodiscard]]const auto&get_data()const noexcept{return _value;}
	};
	#endif
}
using function_base_n::get_function_type;
using function_base_n::function_data_wrapper_t;

//file_end

