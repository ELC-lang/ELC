//convert.hpp
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
typedef string_view_t<char8_t>string_view_u8_t;
typedef string_view_t<char16_t>string_view_u16_t;
typedef string_view_t<char32_t>string_view_u32_t;

inline constexpr size_t utf16_to_utf8_code_size  = 2;
inline constexpr size_t utf32_to_utf8_code_size  = 4;
inline constexpr size_t utf32_to_utf16_code_size = 2;

template<class char_T>
inline constexpr size_t to_u32_code_size=sizeof(char32_t)/sizeof(char_T);

inline namespace convert_impl{
	inline constexpr size_t decode_utf8(char32_t *out, const char8_t *in, size_t in_size_max)noexcept{
		push_and_disable_msvc_warning(26494);//未初始化警告diss
		char8_t code1, code2, code3, code4;
		pop_msvc_warning();
		code1 = *in++;
		if(code1 < 0x80){ /* 1-byte sequence */
			*out = code1;
			return 1;
		}
		elseif(code1 < 0xC2)
			return npos;
		elseif(code1 < 0xE0){ /* 2-byte sequence */
			if(in_size_max < 2)
				return npos;
			code2 = *in++;
			if((code2 & 0xC0) != 0x80)
				return npos;
			*out = (code1 << 6) + code2 - 0x3080;
			return 2;
		}
		elseif(code1 < 0xF0){ /* 3-byte sequence */
			if(in_size_max < 3)
				return npos;
			code2 = *in++;
			if((code2 & 0xC0) != 0x80)
				return npos;
			if(code1 == 0xE0 && code2 < 0xA0)
				return npos;
			code3 = *in++;
			if((code3 & 0xC0) != 0x80)
				return npos;
			*out = (code1 << 12) + (code2 << 6) + code3 - 0xE2080;
			return 3;
		}
		elseif(code1 < 0xF5){ /* 4-byte sequence */
			if(in_size_max < 4)
				return npos;
			code2 = *in++;
			if((code2 & 0xC0) != 0x80)
				return npos;
			if(code1 == 0xF0 && code2 < 0x90)
				return npos;
			if(code1 == 0xF4 && code2 >= 0x90)
				return npos;
			code3 = *in++;
			if((code3 & 0xC0) != 0x80)
				return npos;
			code4 = *in++;
			if((code4 & 0xC0) != 0x80)
				return npos;
			*out = (code1 << 18) + (code2 << 12) + (code3 << 6) + code4 - 0x3C82080;
			return 4;
		}
		else
			return npos;
	}
	inline constexpr size_t get_decode_utf8_size(const char8_t* in)noexcept{
		if(*in < 0x80) /* 1-byte sequence */
			return 1;
		elseif(*in < 0xC2)
			return npos;
		elseif(*in < 0xE0) /* 2-byte sequence */
			return 2;
		elseif(*in < 0xF0) /* 3-byte sequence */
			return 3;
		elseif(*in < 0xF5) /* 4-byte sequence */
			return 4;
		else
			return npos;
	}
	inline constexpr size_t decode_utf16(char32_t *out, const char16_t *in, size_t in_size_max)noexcept{
		const char16_t code1 = *in++;
		if(code1 >= 0xD800 && code1 < 0xDC00){ /* surrogate pair */
			if(in_size_max < 2)
				return npos;
			const char16_t code2 = *in++;
			if(code2 >= 0xDC00 && code2 < 0xE000){
				*out = (code1 << 10) + code2 - 0x35FDC00;
				return 2;
			}
			return npos;
		}
		*out = code1;
		return 1;
	}
	inline constexpr size_t get_decode_utf16_size(const char16_t* in)noexcept{
		if(*in >= 0xD800 && *in < 0xDC00) /* surrogate pair */
			return 2;
		else
			return 1;
	}
	inline constexpr size_t encode_utf8(char8_t *out, char32_t in)noexcept{
		if(in < 0x80){
			*out++ = (char8_t)in;
			return 1;
		}
		elseif(in < 0x800){
			*out++ = char8_t((in >> 6) + 0xC0);
			*out++ = char8_t((in & 0x3F) + 0x80);
			return 2;
		}
		elseif(in < 0x10000){
			*out++ = char8_t((in >> 12) + 0xE0);
			*out++ = ((in >> 6) & 0x3F) + 0x80;
			*out++ = (in & 0x3F) + 0x80;
			return 3;
		}
		elseif(in < 0x110000){
			*out++ = char8_t((in >> 18) + 0xF0);
			*out++ = ((in >> 12) & 0x3F) + 0x80;
			*out++ = ((in >> 6) & 0x3F) + 0x80;
			*out++ = (in & 0x3F) + 0x80;
			return 4;
		}
		return npos;
	}
	inline constexpr size_t get_encode_utf8_size(char32_t in)noexcept{
		if(in < 0x80)
			return 1;
		elseif(in < 0x800)
			return 2;
		elseif(in < 0x10000)
			return 3;
		elseif(in < 0x110000)
			return 4;
		else
			return npos;
	}
	inline constexpr size_t encode_utf16(char16_t *out, char32_t in)noexcept{
		if(in < 0x10000){
			*out++ = char16_t(in);
			return 1;
		}
		elseif(in < 0x110000){
			*out++ = char16_t((in >> 10) + 0xD7C0);
			*out++ = (in & 0x3FF) + 0xDC00;
			return 2;
		}
		return npos;
	}
	inline constexpr size_t get_encode_utf16_size(char32_t in)noexcept{
		if(in < 0x10000)
			return 1;
		elseif(in < 0x110000)
			return 2;
		else
			return npos;
	}
}
template<typename in_char_t, typename out_char_t>
struct code_convert_result{
	bool _success;
	string_view_t<in_char_t> _processed_input;
	string_view_t<out_char_t> _processed_output;
public:
	inline constexpr code_convert_result(bool success, string_view_t<in_char_t> processed_input, string_view_t<out_char_t> processed_output)noexcept:_success(success),_processed_input(processed_input),_processed_output(processed_output){}
	inline constexpr bool success()const noexcept{ return _success; }
	inline constexpr string_view_t<in_char_t> processed_input()const noexcept{ return _processed_input; }
	inline constexpr string_view_t<out_char_t> processed_output()const noexcept{ return _processed_output; }
	inline constexpr explicit operator bool()const noexcept{ return success(); }
};
inline constexpr auto get_utf8_to_utf16_size(const char8_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		char32_t code;
		const auto size = decode_utf8(&code, in, end - in);
		if(size == npos)
			return npos;
		const auto tmp = get_encode_utf16_size(code);
		if(tmp == npos)
			return npos;
		out_size += tmp;
		in += size;
	}
}
inline constexpr auto get_utf8_to_utf16_size(string_view_u8_t in)noexcept{
	return get_utf8_to_utf16_size(in.data(), in.size());
}
inline constexpr auto utf8_to_utf16(char16_t *out, const char8_t *in, size_t len)noexcept{
	const auto end = in + len;
	push_and_disable_msvc_warning(26494);//未初始化警告diss
	char32_t code;
	pop_msvc_warning();
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char8_t, char16_t>(success, string_view_t<char8_t>(in_start, in - in_start), string_view_t<char16_t>(out_start, out - out_start));
	};
	while(in < end){
		const auto size = decode_utf8(&code, in, end - in);
		if(size == npos)
			return result_builder(false);
		auto tmp = encode_utf16(out, code);
		if(tmp == npos)
			return result_builder(false);
		out += tmp;
		in += size;
	}
	return result_builder(true);
}
inline constexpr auto utf8_to_utf16(char16_t *out, string_view_u8_t in)noexcept{
	return utf8_to_utf16(out, in.data(), in.size());
}
inline constexpr auto get_utf8_to_utf32_size(const char8_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		const auto tmp = get_decode_utf8_size(in);
		if(tmp == npos)
			return npos;
		in += tmp;
		out_size++;
	}
	return out_size;
}
inline constexpr auto get_utf8_to_utf32_size(string_view_u8_t in)noexcept{
	return get_utf8_to_utf32_size(in.data(), in.size());
}
inline constexpr auto utf8_to_utf32(char32_t *out, const char8_t *in, size_t len)noexcept{
	const auto end = in + len;
	push_and_disable_msvc_warning(26494);//未初始化警告diss
	char32_t code;
	pop_msvc_warning();
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char8_t, char32_t>(success, string_view_t<char8_t>(in_start, in - in_start), string_view_t<char32_t>(out_start, out - out_start));
	};
	while(in < end){
		const auto size = decode_utf8(&code, in, end - in);
		if(size == npos)
			return result_builder(false);
		*out++ = code;
		in += size;
	}
	return result_builder(true);
}
inline constexpr auto utf8_to_utf32(char32_t *out, string_view_u8_t in)noexcept{
	return utf8_to_utf32(out, in.data(), in.size());
}
inline constexpr auto get_utf16_to_utf8_size(const char16_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		char32_t code;
		const auto size = decode_utf16(&code, in, end - in);
		if(size == npos)
			return npos;
		const auto tmp = get_encode_utf8_size(code);
		if(tmp == npos)
			return npos;
		out_size += tmp;
		in += size;
	}
	return out_size;
}
inline constexpr auto get_utf16_to_utf8_size(string_view_u16_t in)noexcept{
	return get_utf16_to_utf8_size(in.data(), in.size());
}
inline constexpr auto utf16_to_utf8(char8_t *out, const char16_t *in, size_t len)noexcept{
	const auto end = in + len;
	push_and_disable_msvc_warning(26494);//未初始化警告diss
	char32_t code;
	pop_msvc_warning();
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char16_t, char8_t>(success, string_view_t<char16_t>(in_start, in - in_start), string_view_t<char8_t>(out_start, out - out_start));
	};
	while(in < end){
		const auto size = decode_utf16(&code, in, end - in);
		if(size == npos)
			return result_builder(false);
		auto tmp = encode_utf8(out, code);
		if(tmp == npos)
			return result_builder(false);
		out += tmp;
		in += size;
	}
	return result_builder(true);
}
inline constexpr auto utf16_to_utf8(char8_t *out, string_view_u16_t in)noexcept{
	return utf16_to_utf8(out, in.data(), in.size());
}
inline constexpr auto get_utf16_to_utf32_size(const char16_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		char32_t code;
		const auto size = decode_utf16(&code, in, end - in);
		if(size == npos)
			return npos;
		in += size;
		out_size++;
	}
	return out_size;
}
inline constexpr auto get_utf16_to_utf32_size(string_view_u16_t in)noexcept{
	return get_utf16_to_utf32_size(in.data(), in.size());
}
inline constexpr auto utf16_to_utf32(char32_t *out, const char16_t *in, size_t len)noexcept{
	const auto end = in + len;
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char16_t, char32_t>(success, string_view_t<char16_t>(in_start, in - in_start), string_view_t<char32_t>(out_start, out - out_start));
	};
	while(in < end){
		char32_t code;
		const auto size = decode_utf16(&code, in, end - in);
		if(size == npos)
			return result_builder(false);
		*out++ = code;
		in += size;
	}
	return result_builder(true);
}
inline constexpr auto utf16_to_utf32(char32_t *out, string_view_u16_t in)noexcept{
	return utf16_to_utf32(out, in.data(), in.size());
}
inline constexpr auto get_utf32_to_utf8_size(const char32_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		const auto tmp = get_encode_utf8_size(*in);
		if(tmp == npos)
			return npos;
		out_size += tmp;
		++in;
	}
	return out_size;
}
inline constexpr auto get_utf32_to_utf8_size(string_view_u32_t in)noexcept{
	return get_utf32_to_utf8_size(in.data(), in.size());
}
inline constexpr auto utf32_to_utf8(char8_t *out, const char32_t *in, size_t len)noexcept{
	const auto end = in + len;
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char32_t, char8_t>(success, string_view_t<char32_t>(in_start, in - in_start), string_view_t<char8_t>(out_start, out - out_start));
	};
	while(in < end){
		auto tmp = encode_utf8(out, *in);
		if(tmp == npos)
			return result_builder(false);
		out += tmp;
		++in;
	}
	return result_builder(true);
}
inline constexpr auto utf32_to_utf8(char8_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf8(out, in.data(), in.size());
}
inline constexpr auto get_utf32_to_utf16_size(const char32_t *in, size_t len)noexcept{
	size_t out_size = 0;
	const auto end = in + len;
	while(in < end){
		const auto tmp = get_encode_utf16_size(*in);
		if(tmp == npos)
			return npos;
		out_size += tmp;
		++in;
	}
	return out_size;
}
inline constexpr auto get_utf32_to_utf16_size(string_view_u32_t in)noexcept{
	return get_utf32_to_utf16_size(in.data(), in.size());
}
inline constexpr auto utf32_to_utf16(char16_t *out, const char32_t *in, size_t len)noexcept{
	const auto end = in + len;
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char32_t, char16_t>(success, string_view_t<char32_t>(in_start, in - in_start), string_view_t<char16_t>(out_start, out - out_start));
	};
	while(in < end){
		auto tmp = encode_utf16(out, *in);
		if(tmp == npos)
			return result_builder(false);
		out += tmp;
		++in;
	}
	return result_builder(true);
}
inline constexpr auto utf32_to_utf16(char16_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf16(out, in.data(), in.size());
}
inline constexpr auto utf32_to_utf8(char32_t ch, char8_t *out)noexcept{
	return utf32_to_utf8(out, &ch, 1);
}
inline constexpr auto utf32_to_utf16(char32_t ch, char16_t *out)noexcept{
	return utf32_to_utf16(out, &ch, 1);
}
inline constexpr auto utf16_to_utf8(char16_t ch, char8_t *out)noexcept{
	return utf16_to_utf8(out, &ch, 1);
}
inline constexpr void utf16_to_utf32(char32_t *out, char16_t in)noexcept=delete;
inline constexpr void utf8_to_utf16(char16_t *out, char8_t in)noexcept=delete;

//

//utf32 to utf32 for compatible
inline constexpr auto utf32_to_utf32(char32_t *out, const char32_t *in, size_t len)noexcept{
	const auto end = in + len;
	//for return
	const auto out_start = out;
	const auto in_start = in;
	auto result_builder = exlambda(bool success)noexcept{
		return code_convert_result<char32_t, char32_t>(success, string_view_t<char32_t>(in_start, in - in_start), string_view_t<char32_t>(out_start, out - out_start));
	};
	while(in < end){
		*out++ = *in++;
	}
	return result_builder(true);
}
inline constexpr auto utf32_to_utf32(char32_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf32(out, in.data(), in.size());
}
inline constexpr auto utf32_to_utf32(char32_t ch, char32_t *out)noexcept{
	return utf32_to_utf32(out, &ch, 1);
}

//utf32 to auto
force_inline constexpr auto utf32_to_auto(char32_t *out, const char32_t *in, size_t len)noexcept{
	return utf32_to_utf32(out, in, len);
}
force_inline constexpr auto utf32_to_auto(char16_t *out, const char32_t *in, size_t len)noexcept{
	return utf32_to_utf16(out, in, len);
}
force_inline constexpr auto utf32_to_auto(char8_t *out, const char32_t *in, size_t len)noexcept{
	return utf32_to_utf8(out, in, len);
}
force_inline constexpr auto utf32_to_auto(char32_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf32(out, in.data(), in.size());
}
force_inline constexpr auto utf32_to_auto(char16_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf16(out, in.data(), in.size());
}
force_inline constexpr auto utf32_to_auto(char8_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf8(out, in.data(), in.size());
}
force_inline constexpr auto utf32_to_auto(char32_t ch, char32_t *out)noexcept{
	return utf32_to_utf32(out, &ch, 1);
}
force_inline constexpr auto utf32_to_auto(char32_t ch, char16_t *out)noexcept{
	return utf32_to_utf16(out, &ch, 1);
}
force_inline constexpr auto utf32_to_auto(char32_t ch, char8_t *out)noexcept{
	return utf32_to_utf8(out, &ch, 1);
}

//auto to utf32
force_inline constexpr auto auto_to_utf32(char32_t *out, const char32_t *in, size_t len)noexcept{
	return utf32_to_utf32(out, in, len);
}
force_inline constexpr auto auto_to_utf32(char32_t *out, const char16_t *in, size_t len)noexcept{
	return utf16_to_utf32(out, in, len);
}
force_inline constexpr auto auto_to_utf32(char32_t *out, const char8_t *in, size_t len)noexcept{
	return utf8_to_utf32(out, in, len);
}
force_inline constexpr auto auto_to_utf32(char32_t *out, string_view_u32_t in)noexcept{
	return utf32_to_utf32(out, in.data(), in.size());
}
force_inline constexpr auto auto_to_utf32(char32_t *out, string_view_u16_t in)noexcept{
	return utf16_to_utf32(out, in.data(), in.size());
}
force_inline constexpr auto auto_to_utf32(char32_t *out, string_view_u8_t in)noexcept{
	return utf8_to_utf32(out, in.data(), in.size());
}
force_inline constexpr auto auto_to_utf32(char32_t ch, char32_t *out)noexcept{
	return utf32_to_utf32(out, &ch, 1);
}
force_inline constexpr auto auto_to_utf32(char16_t ch, char32_t *out)noexcept{
	return utf16_to_utf32(out, &ch, 1);
}
force_inline constexpr auto auto_to_utf32(char8_t ch, char32_t *out)noexcept{
	return utf8_to_utf32(out, &ch, 1);
}

//file_end

