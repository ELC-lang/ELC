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

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)&>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const>
	{using type=Ret_t(Args_t...);};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const&>
	{using type=Ret_t(Args_t...);};
	
	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)&noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const noexcept>
	{using type=Ret_t(Args_t...)noexcept;};

	template<typename Ret_t,typename T,typename...Args_t>
	struct function_t_guide_helper<Ret_t(T::*)(Args_t...)const&noexcept>
	{using type=Ret_t(Args_t...)noexcept;};
	//BLOCK_END

	//BLOCK:function_t推导指引
	template<class Ret_t,class...Args_t>
	function_t(Ret_t(*)(Args_t...))->function_t<Ret_t(Args_t...)>;
	template<class Ret_t,class...Args_t>
	function_t(Ret_t(*)(Args_t...)noexcept)->function_t<Ret_t(Args_t...)noexcept>;
	template<typename T,typename Func_t=typename function_t_guide_helper<decltype(&T::operator())>::type>
	function_t(T)->function_t<Func_t>;
	//BLOCK_END

	//BLOCK:may_throw_in_destruct_function_t推导指引
	template<class Ret_t,class...Args_t>
	may_throw_in_destruct_function_t(Ret_t(*)(Args_t...))->may_throw_in_destruct_function_t<Ret_t(Args_t...)>;
	template<class Ret_t,class...Args_t>
	may_throw_in_destruct_function_t(Ret_t(*)(Args_t...)noexcept)->may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept>;
	template<typename T,typename Func_t=typename function_t_guide_helper<decltype(&T::operator())>::type>
	may_throw_in_destruct_function_t(T)->may_throw_in_destruct_function_t<Func_t>;
	//BLOCK_END
}

//file_end

