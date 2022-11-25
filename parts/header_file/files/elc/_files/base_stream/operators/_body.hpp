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
template<class char_T>
text_ostream<char_T>&operator<<(text_ostream<char_T>&stream,const char_T ch){
	stream.write(&ch,1);
	return stream;
}
template<class char_T>
noexcept_text_ostream<char_T>&operator<<(noexcept_text_ostream<char_T>&stream,const char_T ch)noexcept{
	stream.write(&ch,1);
	return stream;
}
template<class char_T>
text_ostream<char_T>&operator<<(text_ostream<char_T>&stream,array_like_view_t<const char_T>str){
	stream.write(str.data(),str.size());
	return stream;
}
template<class char_T>
noexcept_text_ostream<char_T>&operator<<(noexcept_text_ostream<char_T>&stream,array_like_view_t<const char_T>str)noexcept{
	stream.write(str.data(),str.size());
	return stream;
}
template<class char_T>
text_ostream<char_T>&operator<<(text_ostream<char_T>&stream,const char_T*str){
	return stream << array_end_by_zero_t<const char_T>{str};
}
template<class char_T>
noexcept_text_ostream<char_T>&operator<<(noexcept_text_ostream<char_T>&stream,const char_T*str)noexcept{
	return stream << array_end_by_zero_t<const char_T>{str};
}
//operator>> of template<class char_T>struct text_istream
template<class char_T>
text_istream<char_T>&operator>>(text_istream<char_T>&stream,char_T&ch){
	stream.read(&ch,1);
	return stream;
}
template<class char_T>
noexcept_text_istream<char_T>&operator>>(noexcept_text_istream<char_T>&stream,char_T&ch)noexcept{
	stream.read(&ch,1);
	return stream;
}

//data_stream of any is_trivially_copyable

//operator<< of template<class T>struct data_ostream
template<class T> requires ::std::is_trivially_copyable_v<T>
data_ostream&operator<<(data_ostream&stream,const T&data){
	stream.write(&data,1);
	return stream;
}
template<class T> requires ::std::is_trivially_copyable_v<T>
noexcept_data_ostream&operator<<(noexcept_data_ostream&stream,const T&data)noexcept{
	stream.write(&data,1);
	return stream;
}
template<class T,size_t N> requires ::std::is_trivially_copyable_v<T>
data_ostream&operator<<(data_ostream&stream,const T(&data)[N]){
	stream.write(data,N);
	return stream;
}
template<class T,size_t N> requires ::std::is_trivially_copyable_v<T>
noexcept_data_ostream&operator<<(noexcept_data_ostream&stream,const T(&data)[N])noexcept{
	stream.write(data,N);
	return stream;
}
//operator>> of template<class T>struct data_istream
template<class T> requires ::std::is_trivially_copyable_v<T>
data_istream&operator>>(data_istream&stream,T&data){
	stream.read(&data,1);
	return stream;
}
template<class T> requires ::std::is_trivially_copyable_v<T>
noexcept_data_istream&operator>>(noexcept_data_istream&stream,T&data)noexcept{
	stream.read(&data,1);
	return stream;
}
template<class T,size_t N> requires ::std::is_trivially_copyable_v<T>
data_istream&operator>>(data_istream&stream,T(&data)[N]){
	stream.read(data,N);
	return stream;
}
template<class T,size_t N> requires ::std::is_trivially_copyable_v<T>
noexcept_data_istream&operator>>(noexcept_data_istream&stream,T(&data)[N])noexcept{
	stream.read(data,N);
	return stream;
}

//file_end

