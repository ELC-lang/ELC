//equal.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//equal：值相等
constexpr struct equal_t{
	template<class T>
	static constexpr bool able= was_not_an_ill_form(declvalue(T&)==declvalue(T&));
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

//eq：同一对象
template<typename T>
inline auto is_eq(T&&a,T&&b)noexcept_as(&declvalue(T)==&declvalue(T)){
	return &a==&b;
}
template<typename T>
inline auto is_not_eq(T&&a,T&&b)noexcept_as(&declvalue(T)==&declvalue(T)){
	return!is_eq(a,b);
}

//compare：三路比较
constexpr struct compare_t{
	template<class T>
	static constexpr bool able= was_not_an_ill_form(declvalue(T&)<=>declvalue(T&));
	template<class T>
	static constexpr bool nothrow= noexcept(declvalue(T&)<=>declvalue(T&));

	//UF
	/*
	template<class T, class U>
	constexpr auto base_call(const T& a,const U& b){
		//compare_3way 在 <=> 不可用时以 < 和 == 为后备，优于直接 <=>
		return ::std::compare_3way(a,b);
	}
	*/

	template<typename T>
	inline auto operator()(T&&a,T&&b)const noexcept(nothrow<T>){
		return a<=>b;
	}
	template<typename T>
	inline auto operator()(T*a,T*b,size_t size)const noexcept(nothrow<T>){
		while(size--){
			if(auto tmp=*(a++)<=>*(b++); tmp!=0)
				return tmp;
		}
		return 0<=>0;
	}
	template<typename T,size_t N1,size_t N2>
	inline auto operator()(T(&a)[N1],T(&b)[N2])const noexcept(nothrow<T>){
		if constexpr(N1==N2)
			return operator()(a,b,N1);
		else{
			template_warning("N1!=N2");
			return N1<=>N2;
		}
	}
	template<typename T>
	inline auto operator()(T*a,size_t size1,T*b,size_t size2)const noexcept(nothrow<T>){
		decltype(declvalue(T&)<=>declvalue(T&)) tmp = size1<=>size2;
		if(tmp!=0)
			return tmp;
		else
			return operator()(a,b,size1);
	}
}compare{};

//file_end

