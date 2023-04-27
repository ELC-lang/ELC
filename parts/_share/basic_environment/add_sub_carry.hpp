//add_sub_carry.hpp
/*
未完成的elc解释器_share文件
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
#if HAS_MSVC_INTRINSICS
	template<unsigned_basic_integer_type T>
	force_inline bool _msvc_builtin_add_carry(bool c_in, T a, T b, T*out)noexcept{
		if constexpr(sizeof(T) == 8)
			return _addcarry_u64(c_in, a, b, out);
		elseif constexpr(sizeof(T) == 4)
			return _addcarry_u32(c_in, a, b, out);
		elseif constexpr(sizeof(T) == 2)
			return _addcarry_u16(c_in, a, b, out);
		elseif constexpr(sizeof(T) == 1)
			return _addcarry_u8(c_in, a, b, out);
	}
	template<unsigned_basic_integer_type T>
	force_inline bool _msvc_builtin_sub_borrow(bool b_in, T a, T b, T*out)noexcept{
		if constexpr(sizeof(T) == 8)
			return _subborrow_u64(b_in, a, b, out);
		elseif constexpr(sizeof(T) == 4)
			return _subborrow_u32(b_in, a, b, out);
		elseif constexpr(sizeof(T) == 2)
			return _subborrow_u16(b_in, a, b, out);
		elseif constexpr(sizeof(T) == 1)
			return _subborrow_u8(b_in, a, b, out);
	}
#endif
// add carry
template<unsigned_basic_integer_type T>
constexpr force_inline bool add_carry(T a, T b, T& c)noexcept{
	if(!in_consteval){
		#if HAS_GCC_BUILTINS
			return __builtin_add_overflow(a, b, &c);
		#elif HAS_MSVC_INTRINSICS
			return _msvc_builtin_add_carry(0, a, b, &c);
		#elif HAS_X86_ASM
			//x86/x86-64
			__asm__ (
				"add %2, %1\n\t"
				"setc %0\n\t"
				: "=r"(c)
				: "r"(a), "r"(b)
			);
			return c;
		#elif HAS_ARM_ASM || HAS_ARM64_ASM
			//arm/arm64
			__asm__ (
				"adds %1, %1, %2\n\t"
				"adc %0, xzr, xzr\n\t"
				: "=r"(c)
				: "r"(a), "r"(b)
			);
			return c;
		#endif
	}
	c = a + b;
	return c < a;
}
template<unsigned_basic_integer_type T>
constexpr force_inline T add_carry(T a, T b, bool& is_overflow)noexcept{
	#if HAS_MSVC_INTRINSICS
		if(!in_consteval){
			T c;is_overflow=_msvc_builtin_add_carry(is_overflow, a, b, &c);
			return c;
		}
		else
	#endif
	{
		T c=a+is_overflow;
		if(c){
			is_overflow=add_carry(c,b,c);
			return c;
		}
		else{
			//现在我们得知a+is_overflow的结果为0:
			//若is_overflow为true，溢出确实发生了，我们需要将is_overflow设置为true，所以什么都不用管
			//若is_overflow为false，溢出没有发生，我们需要将is_overflow设置为false，我们仍然什么都不用管
			return b;
		}
	}
}
template<unsigned_basic_integer_type T>
constexpr force_inline T add_carry(T a, bool& is_overflow)noexcept{
	#if HAS_MSVC_INTRINSICS
		if(!in_consteval){
			T c;is_overflow=_msvc_builtin_add_carry(is_overflow, a, T{}, &c);
			return c;
		}
		else
	#endif
	{
		T c=a+is_overflow;
		if(c)//若c为0，保持is_overflow不变（原理见上）
			is_overflow=false;
		return c;
	}
}
// sub borrow
template<unsigned_basic_integer_type T>
constexpr force_inline bool sub_borrow(T a, T b, T& c)noexcept{
	if(!in_consteval){
		#if HAS_GCC_BUILTINS
			return __builtin_sub_overflow(a, b, &c);
		#elif HAS_MSVC_INTRINSICS
			return _msvc_builtin_sub_borrow(0, a, b, &c);
		#elif HAS_X86_ASM
			//x86/x86-64
			__asm__ (
				"sub %2, %1\n\t"
				"setc %0\n\t"
				: "=r"(c)
				: "r"(a), "r"(b)
			);
			return c;
		#elif HAS_ARM_ASM || HAS_ARM64_ASM
			//arm/arm64
			__asm__ (
				"subs %1, %1, %2\n\t"
				"sbc %0, xzr, xzr\n\t"
				: "=r"(c)
				: "r"(a), "r"(b)
			);
			return c;
		#endif
	}
	c = a - b;
	return c > a;
}
template<unsigned_basic_integer_type T>
constexpr force_inline T sub_borrow(T a, T b, bool& is_underflow)noexcept{
	#if HAS_MSVC_INTRINSICS
		if(!in_consteval){
			T c;is_underflow=_msvc_builtin_sub_borrow(is_underflow, a, b, &c);
			return c;
		}
		else
	#endif
	{
		T c=b+is_underflow;
		if(c){
			is_underflow=sub_borrow(a,c,c);
			return c;
		}
		else{
			//现在我们得知b+is_underflow的结果为0:
			//若is_underflow为true，溢出肯定该发生，我们需要将is_underflow设置为true，所以什么都不用管
			//若is_underflow为false，溢出不会发生，我们需要将is_underflow设置为false，我们仍然什么都不用管
			return a;
		}
	}
}
template<unsigned_basic_integer_type T>
constexpr force_inline T sub_borrow(T a, bool& is_underflow)noexcept{
	#if HAS_MSVC_INTRINSICS
		if(!in_consteval){
			T c;is_underflow=_msvc_builtin_sub_borrow(is_underflow, a, T{}, &c);
			return c;
		}
		else
	#endif
	{
		if(is_underflow){
			is_underflow=!a;
			return a-1;
		}
		else
			return a;
	}
}


//file_end

