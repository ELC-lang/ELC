//compare.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//end_by_zero_t
/*
用于标志某一数据串以0结尾
*/
inline constexpr struct end_by_zero_t{}end_by_zero;
//just_an_part_t
/*
用于标志某一数据串并没有完结
*/
inline constexpr struct just_an_part_t{}just_an_part;
//equal：值相等.
/*
	相等比较任意支持比较的类型或其数组——简易封装.
	用法:
	判断两值是否可比较。
	equal.able<类型1,类型2（可选，默认为类型1）>		->	bool
	判断将两者进行比较时是否会抛出异常。
	equal.nothrow<类型,类型2（可选，默认为类型1）>		->	bool

	equal(T1,T2)										->	auto

	equal(T1[N1],T2[N2])								->	bool
	equal(T1*,size1,T2*,size2)							->	bool
	equal(T1*,T2*,size)									->	bool
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
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,size_t size1,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(size1--){
			if(*a!=*b || *b==U{0})
				return false;
			a++;
			b++;
		}
		return *b==U{0};
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,end_by_zero_t,U*b,size_t size2)const noexcept(nothrow<T,U>){
		return operator()(b,size2,a,end_by_zero);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,end_by_zero_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(true){
			if(*a!=*b)
				return false;
			if(*a==T{0})
				return true;
			a++;
			b++;
		}
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr bool operator()(T*a,size_t size1,just_an_part_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(size1--){
			if(*a!=*b || *b==U{0})
				return false;
			a++;
			b++;
		}
		return true;
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
	compare.r_able：判断两者是否可进行三路比较。
	compare.r_able<类型1,类型2(可选，默认与1相同)>		返回bool。
compare.able：判断两者是否可进行compare操作。
	compare.able<类型1,类型2(可选，默认与1相同)>		返回bool。
compare.nothrow：判断将两者进行三路比较是否会抛出异常。
	compare.nothrow<类型1,类型2（可选，默认与1相同）>		返回bool。

	compare(T1,T2)
	compare(T1[N1],T2[N2])
	compare(T1*,size1,T2*,size2)
	compare(T1*,T2*,size)
	返回类型语义上是三路比较的结果类型,具体类型视情况而定.
    compare.lexicographical()以字典序进行三路比较。
	用法：
	compare.lexicographical(T1,T2)
	compare.lexicographical(T1[N1],T2[N2])
	compare.lexicographical(T1*,size1,T2*,size2)
	compare.lexicographical(T1*,T2*,size)
	返回类型语义上是三路比较的结果类型,具体类型视情况而定.
    compare.reverse()翻转三路比较的结果类型。
	用法：
	compare.reverse(partial_ordering)
	compare.reverse(weak_ordering)
	compare.reverse(strong_ordering)
	返回对应结果类型的反义，与零相等或无法比较时则返回其本身。
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
	[[nodiscard]]static constexpr auto base_call(T&&a,U&&b)noexcept(nothrow<T,U>){
		//在 <=> 不可用时以 < 和 == 为后备，优于直接 <=>
		if constexpr(r_able<T,U>)
			return a<=>b;
		else return a == b	? partial_ordering::equivalent	:
					a < b	? partial_ordering::less		:
					b < a	? partial_ordering::greater		:
							  partial_ordering::unordered	;
	}

	template<class T,class U=T>
	using type=decltype(base_call(declvalue(T),declvalue(U)));

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
		return strong_ordering::equivalent;
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
	[[nodiscard]]constexpr auto operator()(T*a,size_t size1,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(size1--){
			if(*b==U{0})
				return strong_ordering::greater;
			if(auto tmp=base_call(*a,*b); tmp!=0)
				return tmp;
			a++;
			b++;
		}
		return *b==U{0}?strong_ordering::equivalent:
						strong_ordering::less;
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T*a,end_by_zero_t,U*b,size_t size2)const noexcept(nothrow<T,U>){
		while(size2--){
			if(*a==T{0})
				return strong_ordering::less;
			if(auto tmp=base_call(*a,*b); tmp!=0)
				return tmp;
			a++;
			b++;
		}
		return *b==U{0}?strong_ordering::equivalent:
						strong_ordering::less;
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T*a,end_by_zero_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(true){
			if(*a==T{0})
				return *b==U{0}?strong_ordering::equivalent:
								strong_ordering::less;
			if(auto tmp=base_call(*a,*b); tmp!=0)
				return tmp;
			a++;
			b++;
		}
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto operator()(T*a,size_t size1,just_an_part_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		while(size1--){
			if(*b==U{0})
				return strong_ordering::greater;
			if(auto tmp=base_call(*a,*b); tmp!=0)
				return tmp;
			a++;
			b++;
		}
		return strong_ordering::equivalent;
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
	template<typename T,typename U>
	[[nodiscard]]constexpr auto lexicographical(T*a,size_t size1,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		return operator()(a,size1,b,end_by_zero);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto lexicographical(T*a,end_by_zero_t,U*b,size_t size2)const noexcept(nothrow<T,U>){
		return operator()(a,end_by_zero,b,size2);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto lexicographical(T*a,end_by_zero_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		return operator()(a,end_by_zero,b,end_by_zero);
	}
	template<typename T,typename U>
	[[nodiscard]]constexpr auto lexicographical(T*a,size_t size1,just_an_part_t,U*b,end_by_zero_t)const noexcept(nothrow<T,U>){
		return operator()(a,size1,just_an_part,b,end_by_zero);
	}
	//
	[[nodiscard]]constexpr auto reverse(partial_ordering odr)const noexcept{
		if(odr==partial_ordering::greater)
			return partial_ordering::less;
		elseif(odr==partial_ordering::less)
			return partial_ordering::greater;
		else
			return odr;
	}
	[[nodiscard]]constexpr auto reverse(weak_ordering odr)const noexcept{
		if(odr==weak_ordering::greater)
			return weak_ordering::less;
		elseif(odr==weak_ordering::less)
			return weak_ordering::greater;
		else
			return odr;
	}
	[[nodiscard]]constexpr auto reverse(strong_ordering odr)const noexcept{
		if(odr==strong_ordering::greater)
			return strong_ordering::less;
		elseif(odr==strong_ordering::less)
			return strong_ordering::greater;
		else
			return odr;
	}
}compare{};

//file_end

