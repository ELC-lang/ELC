//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//text_stream

//operator<< of template<class char_T>struct text_ostream
template<class text_ostream_T,class char_T=typename text_ostream_T::char_type> requires(type_info<text_ostream_T>.base_on<text_ostream<char_T>>)
auto&operator<<(text_ostream_T&stream,const char_T ch)noexcept(type_info<text_ostream_T>.base_on<noexcept_text_ostream<char_T>>){
	stream.write(&ch,1);
	return stream;
}
template<class text_ostream_T,class char_T=typename text_ostream_T::char_type> requires(type_info<text_ostream_T>.base_on<text_ostream<char_T>>)
auto&operator<<(text_ostream_T&stream,array_like_view_t<const char_T>str)noexcept(type_info<text_ostream_T>.base_on<noexcept_text_ostream<char_T>>){
	stream.write(str.data(),str.size());
	return stream;
}
template<class text_ostream_T,class char_T=typename text_ostream_T::char_type> requires(type_info<text_ostream_T>.base_on<text_ostream<char_T>>)
auto&operator<<(text_ostream_T&stream,const char_T*str)noexcept(type_info<text_ostream_T>.base_on<noexcept_text_ostream<char_T>>){
	return stream << array_end_by_zero_t<const char_T>{str};
}
//operator>> of template<class char_T>struct text_istream
template<class text_istream_T,class char_T=typename text_istream_T::char_type> requires(type_info<text_istream_T>.base_on<text_istream<char_T>>)
text_istream<char_T>&operator>>(text_istream<char_T>&stream,char_T&ch)noexcept(type_info<text_istream_T>.base_on<noexcept_text_istream<char_T>>){
	stream.read(&ch,1);
	return stream;
}

//data_stream of any is_trivially_copyable

//operator<< of template<class T>struct data_ostream
template<class data_ostream_T,class T> requires(type_info<data_ostream_T>.base_on<data_ostream> && ::std::is_trivially_copyable_v<T>)
auto&operator<<(data_ostream_T&stream,const T&data)noexcept(type_info<data_ostream_T>.base_on<noexcept_data_ostream>){
	stream.write(&data,1);
	return stream;
}
template<class data_ostream_T,class T,size_t N> requires(type_info<data_ostream_T>.base_on<data_ostream> && ::std::is_trivially_copyable_v<T>)
auto&operator<<(data_ostream_T&stream,const T(&data)[N])noexcept(type_info<data_ostream_T>.base_on<noexcept_data_ostream>){
	stream.write(data,N);
	return stream;
}
//operator>> of template<class T>struct data_istream
template<class data_istream_T,class T> requires(type_info<data_istream_T>.base_on<data_istream> && ::std::is_trivially_copyable_v<T>)
auto&operator>>(data_istream_T&stream,T&data)noexcept(type_info<data_istream_T>.base_on<noexcept_data_istream>){
	stream.read(&data,1);
	return stream;
}
template<class data_istream_T,class T,size_t N> requires(type_info<data_istream_T>.base_on<data_istream> && ::std::is_trivially_copyable_v<T>)
data_istream&operator>>(data_istream&stream,T(&data)[N])noexcept(type_info<data_istream_T>.base_on<noexcept_data_istream>){
	stream.read(data,N);
	return stream;
}

//flush
inline constexpr struct flush_t{
	template<class ostream_T>requires(type_info<ostream_T>.base_on<base_ostream>)
	constexpr decltype(auto)operator()(ostream_T&stream)const noexcept(type_info<ostream_T>.base_on<noexcept_ostream>){
		stream.flush();
		return stream;
	}
}flush{};

//endline
inline constexpr struct endline_t{
	template<class text_ostream_T,class char_T=typename text_ostream_T::char_type> requires(type_info<text_ostream_T>.base_on<text_ostream<char_T>>)
	constexpr decltype(auto)operator()(text_ostream_T& stream) const noexcept(type_info<text_ostream_T>.base_on<noexcept_text_ostream<char_T>>) {
		stream << char_T{'\n'};
		stream.flush();
		return stream;
	}
}endline{};

//operator<< of functions
template<class callable_T,class stream_T> requires(type_info<stream_T>.base_on<base_ostream> && invoke<callable_T>.able<stream_T&>)
decltype(auto)operator<<(stream_T&stream,callable_T&&callable)noexcept(invoke<callable_T>.nothrow<stream_T&>){
	return callable(stream);
}

//file_end

