//deduction_guides.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	//BLOCK:推导指引助手
	template<typename>
	struct function_t_guide_helper{};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)&noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};

	template<typename Ret_t,typename T,typename...Args_t,bool nothrow>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const&noexcept(nothrow)>
	{using type=Ret_t(Args_t...)noexcept(nothrow);};
	//BLOCK_END

	//BLOCK:base_function_t推导指引
	template<class T,bool promise_nothrow_at_destruct>
	base_function_t(T)->base_function_t<T,promise_nothrow_at_destruct>;
	template<typename T,typename Func_t=typename function_t_guide_helper<decltype(&T::operator())>::type,bool promise_nothrow_at_destruct>
	base_function_t(T)->base_function_t<Func_t,promise_nothrow_at_destruct>;
	//BLOCK_END
}

//file_end

