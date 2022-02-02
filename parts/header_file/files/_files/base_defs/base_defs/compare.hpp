//compare.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//equal：值相等.
/*
	相等比较任意支持比较的类型或其数组——简易封装.
	用法:
	equal(T1,T2)
	返回类型自动推断
	equal(T1[N1],T2[N2])
	equal(T1*,size1,T2*,size2)
	equal(T1*,T2*,size)
	通通返回bool.
*/
constexpr struct equal_t{
	template<class T,class U=T>
	static constexpr bool able= was_not_an_ill_form(declvalue(T)==declvalue(U));
	template<class T,class U=T>
	static constexpr bool nothrow= was_not_an_ill_form_and_noexcept(declvalue(T)==declvalue(U));

	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T&&a,U&&b)const noexcept(nothrow<T,U>){
		return a==b;
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,U*b,size_t size)const noexcept(nothrow<T,U>){
		while(size--){
			if(*(a++)!=*(b++))
				return false;
		}
		return true;
	}
	template<typename T,typename U,size_t N1,size_t N2>
	[[nodiscard]]constexpr bool operator()(T(&a)[N1],U(&b)[N2])const noexcept(nothrow<T,U>){
		if constexpr(N1==N2)
			return operator()(a,b,N1);
		else{
			template_warning("N1!=N2");
			return false;
		}
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,size_t size1,U*b,size_t size2)const noexcept(nothrow<T,U>){
		if(size1==size2)
			return operator()(a,b,size1);
		else
			return false;
	}
}equal{};

//eq：同一对象
template<typename T>
[[nodiscard]]constexpr auto is_eq(T&&a,T&&b)noexcept_as(&a==&b){
	return &a==&b;
}
template<typename T>
[[nodiscard]]constexpr auto is_not_eq(T&&a,T&&b)noexcept_as(!is_eq(a,b)){
	return!is_eq(a,b);
}

//compare：三路比较
/*
	若大小相同则以字典序三路比较任意支持比较的类型或其数组,并在 <=> 不可用时以 < 和 == 为后备.
	用法:
	compare(T1,T2)
	compare(T1[N1],T2[N2])
	compare(T1*,size1,T2*,size2)
	compare(T1*,T2*,size)
	返回类型语义上是三路比较的结果类型,具体类型视情况而定.
*/
constexpr struct compare_t{
	template<class T,class U=T>
	static constexpr bool r_able= was_not_an_ill_form(declvalue(T)<=>declvalue(U));

	template<class T,class U=T>
	static constexpr bool able= r_able<T,U> ||
								was_not_an_ill_form(
														declvalue(T)==declvalue(U),
														declvalue(T)<declvalue(U),
														declvalue(U)<declvalue(T)
													);
	template<class T,class U=T>
	static constexpr bool nothrow= was_not_an_ill_form_and_noexcept(declvalue(T)<=>declvalue(U)) ||
								   was_not_an_ill_form_and_noexcept(
														declvalue(T)==declvalue(U),
														declvalue(T)<declvalue(U),
														declvalue(U)<declvalue(T)
													);


	template<class T,class U>
	[[nodiscard]]constexpr auto base_call(T&&a,U&&b)const noexcept(nothrow<T,U>){
		//在 <=> 不可用时以 < 和 == 为后备，优于直接 <=>
		if constexpr(r_able<T,U>)
			return a<=>b;
		else return a == b	? 1.3<=>1.3	:
					a < b	? 1.3<=>1.7	:
					b < a	? 1.7<=>1.3	:
							  NAN<=>NAN	;
	}

	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T&&a,U&&b)const noexcept(nothrow<T,U>){
		return base_call(a,b);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T*a,U*b,size_t size)const noexcept(nothrow<T,U>){
		while(size--){
			if(auto tmp=base_call(*(a++),*(b++)); tmp!=0)
				return tmp;
		}
		return 0<=>0;
	}
	template<typename T,typename U,size_t N1,size_t N2>
	[[nodiscard]]constexpr auto operator()(T(&a)[N1],U(&b)[N2])const noexcept(nothrow<T,U>){
		if constexpr(N1==N2)
			return operator()(a,b,N1);
		else{
			template_warning("N1!=N2");
			return N1<=>N2;
		}
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T*a,size_t size1,U*b,size_t size2)const noexcept(nothrow<T,U>){
		decltype(operator()(a,b,size1)) tmp=size1<=>size2;
		if(tmp!=0)
			return tmp;
		else
			return operator()(a,b,size1);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto lexicographical(T*a,size_t size1,U*b,size_t size2)const noexcept(nothrow<T,U>){
		if(auto tmp=operator()(a,b,min(size1,size2)); tmp!=0)
			return tmp;
		else
			return size1<=>size2;
	}
	template<typename T,typename U,size_t N1,size_t N2>
	[[nodiscard]]constexpr auto lexicographical(T(&a)[N1],U(&b)[N2])const noexcept(nothrow<T,U>){
		return lexicographical(a,N1,b,N2);
	}
}compare{};

//file_end

