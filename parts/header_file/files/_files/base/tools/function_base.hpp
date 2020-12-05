//function_base.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_base_n{
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
	struct function_type_getter{
		typedef conditional<
					::std::is_function_v<T>,
					T,
					conditional<
						is_pointer<T>&&::std::is_function_v<::std::remove_pointer_t<T>>,//不支持多级函数指针：懒得写（其实挺简单的），而且function_t若支持多级函数指针的推导指引会很不安全
						type_name function_type_getter<::std::remove_pointer_t<T>>::type,
						type_name function_type_getter_helper<decltype(&T::operator())>::type
					>
				> type;
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
	class function_data_warpper_t;
	template<class T,bool nothrow,class Ret_t,class...Args_t>
	struct function_data_warpper_t<T,Ret_t(Args_t...)noexcept(nothrow)>{
		static_assert(!::std::is_function_v<T>);

		T _value;

		function_data_warpper_t(T&a)noexcept(construct<T>.nothrow<T>):_value(a){}
		~function_data_warpper_t()noexcept(destruct.nothrow<T>)=default;
		Ret_t operator()(Args_t...args)noexcept(nothrow){
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(type_info<decltype(declvalue(T)(declvalue(Args_t)...))> != type_info<Ret_t>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			return _value(forward<Args_t>(args)...);
		}
		[[nodiscard]]auto&get_data()noexcept{return _value;}
	};
}
using function_base_n::get_function_type;
using function_base_n::function_data_warpper_t;

//file_end

