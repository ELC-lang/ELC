//equal.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
constexpr struct equal_t{
	template<class T>
	auto equality_comparable_helper(int) -> decltype(
		void(declvalue(T&) == declvalue(T&)),
		::std::true_type{});
	template<class>
	auto equality_comparable_helper(...) -> ::std::false_type;

	template<class T>
	static constexpr bool able= decltype(equality_comparable_helper<T>(0))::value;
	template<class T>
	static constexpr bool nothrow= noexcept(declvalue(T&)==declvalue(T&));

	template<typename T>
	inline auto operator()(T&&a,T&&b)const noexcept(nothrow<T>){
		return a==b;
	}
	template<typename T>
	inline bool operator()(T*a,T*b,size_t size)const noexcept(nothrow<T>){
		while(size--){
			if(*(a++)!=*(b++))
				return false;
		}
		return true;
	}
	template<typename T,size_t N1,size_t N2>
	inline bool operator()(T(&a)[N1],T(&b)[N2])const noexcept(nothrow<T>){
		if constexpr(N1==N2)
			return operator()(a,b,N1);
		else{
			template_warning("N1!=N2");
			return false;
		}
	}
	template<typename T>
	inline bool operator()(T*a,size_t size1,T*b,size_t size2)const noexcept(nothrow<T>){
		if(size1==size2)
			return false;
		else
			return operator()(a,b,size1);
	}
}equal{};


//file_end

