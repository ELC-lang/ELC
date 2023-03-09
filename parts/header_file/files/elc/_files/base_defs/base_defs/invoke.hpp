//invoke.hpp
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
template<typename T>
struct invoke_t{
	/*!
	Determines if a type is invocable.

	@tparam T The type to check.
	@tparam Args The types of the arguments to the type.

	@returns True if the type is invocable, false otherwise.
	*/
	template<class...Args>
	static constexpr bool able= ::std::is_invocable_v<T,Args...>;
	/*!
	Determines if a type is invocable and nothrow.

	@tparam T The type to check.
	@tparam Args The types of the arguments to the type.

	@returns True if the type is nothrow invocable, false otherwise.
	*/
	template<class...Args>
	static constexpr bool nothrow= ::std::is_nothrow_invocable_v<T,Args...>;

	//和期待类型
	template<class Ret_t>
	struct with_return_type_t{
		template<class...Args>
		static constexpr bool able= ::std::is_invocable_r_v<Ret_t,T,Args...>;
		template<class...Args>
		static constexpr bool nothrow= ::std::is_nothrow_invocable_r_v<Ret_t,T,Args...>;

		/*!
		A function that returns a lambda that applies a function to a value.

		@param rest The arguments to the function.

		@returns A lambda that applies the function to a value.
		*/
		template<class...Args> requires able<Args...>
		decltype(auto)_as(Args&&...rest)const noexcept(nothrow<Args...>){//invoke<T>.with_return_type<Ret_t>._as(...)
			return lambda_with_catch(rest...)(const T&a)noexcept(nothrow<Args...>){return a(forward<Args>(rest)...);};
		}
	};
	template<class Ret_t>
	static constexpr with_return_type_t<Ret_t>with_return_type{};

	/*!
	A function that returns a lambda that applies a function to a value.

	@param rest The arguments to the function.

	@returns A lambda that applies the function to a value.
	*/
	template<class...Args> requires able<Args...>
	decltype(auto)_as(Args&&...rest)const noexcept(nothrow<Args...>){//invoke<T>._as(...)
		return lambda_with_catch(rest...)(const T&a)noexcept(nothrow<Args...>){return a(forward<Args>(rest)...);};
	}
};
template<typename T>
constexpr invoke_t<T>invoke{};

//file_end

