//isX.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/* Checks if ch is a lower case letter.*/
constexpr inline bool islower(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return(GET_CATEGORY(props)==U_LOWERCASE_LETTER);
}

/* Checks if ch is an upper case letter.*/
constexpr inline bool isupper(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return(GET_CATEGORY(props)==U_UPPERCASE_LETTER);
}

/* Checks if ch is a title case letter; usually upper case letters.*/
constexpr inline bool istitle(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return(GET_CATEGORY(props)==U_TITLECASE_LETTER);
}

/* Checks if ch is a decimal digit. */
constexpr inline bool isdigit(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

constexpr inline bool isxdigit(char_t c)noexcept{
	uint32_t props;

	/* check ASCII and Fullwidth ASCII a-fA-F */
	if(
		(c<=0x66 && c>=0x41 && (c<=0x46 || c>=0x61)) ||
		(c>=0xff21 && c<=0xff46 && (c<=0xff26 || c>=0xff41))
	){
		return true;
	}

	GET_PROPS(c, props);
	return(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

/* Checks if the Unicode character is a letter.*/
constexpr inline bool isalpha(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&U_GC_L_MASK)!=0);
}

/* Checks if c is a letter or a decimal digit */
constexpr inline bool isalnum(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&(U_GC_L_MASK|U_GC_ND_MASK))!=0);
}

/* Checks if ch is a unicode character with assigned character type.*/
constexpr inline bool isdefined(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return(GET_CATEGORY(props)!=0);
}

/* Checks if the Unicode character is a base form character that can take a diacritic.*/
constexpr inline bool isbase(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&(U_GC_L_MASK|U_GC_N_MASK|U_GC_MC_MASK|U_GC_ME_MASK))!=0);
}

/* Checks if the Unicode character is a control character.*/
constexpr inline bool iscntrl(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&(U_GC_CC_MASK|U_GC_CF_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK))!=0);
}

constexpr inline bool isISOControl(char_t c)noexcept{
	return (uint32_t)c<=0x9f && (c<=0x1f || c>=0x7f);
}

/* Some control characters that are used as space. */
#define IS_THAT_CONTROL_SPACE(c) \
	(c<=0x9f && ((c>=TAB && c<=CR) || (c>=0x1c && c <=0x1f) || c==0x85))

/* Checks if the Unicode character is a space character.*/
constexpr inline bool isspace(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&U_GC_Z_MASK)!=0 || IS_THAT_CONTROL_SPACE(c));
}

#undef IS_THAT_CONTROL_SPACE

constexpr inline bool isblank(char_t c)noexcept{
	if((uint32_t)c<=0x9f){
		return c==9 || c==0x20; /* TAB or SPACE */
	}else{
		/* Zs */
		uint32_t props;
		GET_PROPS(c, props);
		return(GET_CATEGORY(props)==U_SPACE_SEPARATOR);
	}
}

/* Checks if the Unicode character is printable.*/
constexpr inline bool isprint(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	/* comparing ==0 returns false for the categories mentioned */
	return((CAT_MASK(props)&U_GC_C_MASK)==0);
}

constexpr inline bool isgraph(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	/* comparing ==0 returns false for the categories mentioned */
	return((CAT_MASK(props)&
					(U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CS_MASK|U_GC_CN_MASK|U_GC_Z_MASK))
				   ==0);
}

constexpr inline bool ispunct(char_t c)noexcept{
	uint32_t props;
	GET_PROPS(c, props);
	return((CAT_MASK(props)&U_GC_P_MASK)!=0);
}

//file_end

