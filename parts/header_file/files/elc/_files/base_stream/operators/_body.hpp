//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器base_stream文件
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
//text_stream_t

//operator<< of template<class char_T>struct text_ostream_t
template<text_ostream_class text_ostream_T,class char_T> requires(type_info<char_T> == type_info<typename remove_cvref<text_ostream_T>::char_type>)
decltype(auto)operator<<(text_ostream_T&&stream,const char_T ch)noexcept(noexcept_text_ostream_class<text_ostream_T>){
	stream.write(&ch,1);
	return stream;
}
template<text_ostream_class text_ostream_T,class char_T> requires(type_info<char_T> == type_info<typename remove_cvref<text_ostream_T>::char_type>)
decltype(auto)operator<<(text_ostream_T&&stream,array_like_view_t<const char_T>str)noexcept(noexcept_text_ostream_class<text_ostream_T>){
	stream.write(str.data(),str.size());
	return stream;
}
template<text_ostream_class text_ostream_T,class char_T> requires(type_info<char_T> == type_info<typename remove_cvref<text_ostream_T>::char_type>)
decltype(auto)operator<<(text_ostream_T&&stream,const char_T*str)noexcept(noexcept_text_ostream_class<text_ostream_T>){
	return stream << array_end_by_zero_t<const char_T>{str};
}
//operator>> of template<class char_T>struct text_istream_t
template<text_istream_class text_istream_T,class char_T> requires(type_info<char_T> == type_info<typename remove_cvref<text_istream_T>::char_type>)
decltype(auto)operator>>(text_istream_T&stream,char_T&ch)noexcept(noexcept_text_istream_class<text_istream_T>){
	stream.read(&ch,1);
	return stream;
}

//data_stream of any is_trivially_copyable

//operator<< of template<class T>struct data_ostream_t
template<data_ostream_class data_ostream_T,class T> requires ::std::is_trivially_copyable_v<T>
decltype(auto)operator<<(data_ostream_T&&stream,const T&data)noexcept(noexcept_data_ostream_class<data_ostream_T>){
	stream.write(&data,1);
	return stream;
}
template<data_ostream_class data_ostream_T,class T,size_t N> requires ::std::is_trivially_copyable_v<T>
decltype(auto)operator<<(data_ostream_T&&stream,const T(&data)[N])noexcept(noexcept_data_ostream_class<data_ostream_T>){
	stream.write(data,N);
	return stream;
}
//operator>> of template<class T>struct data_istream_t
template<data_istream_class data_istream_T,class T> requires ::std::is_trivially_copyable_v<T>
decltype(auto)operator>>(data_istream_T&&stream,T&data)noexcept(noexcept_data_istream_class<data_istream_T>){
	stream.read(&data,1);
	return stream;
}
template<data_istream_class data_istream_T,class T,size_t N> requires ::std::is_trivially_copyable_v<T>
decltype(auto)operator>>(data_istream_T&&stream,T(&data)[N])noexcept(noexcept_data_istream_class<data_istream_T>){
	stream.read(data,N);
	return stream;
}

//flush
inline constexpr struct flush_t{
	template<ostream_class ostream_T>
	constexpr decltype(auto)operator()(ostream_T&stream)const noexcept(noexcept_ostream_class<ostream_T>){
		stream.flush();
		return stream;
	}
}flush{};

//endline
inline constexpr struct endline_t{
	template<text_ostream_class text_ostream_T,class char_T=typename remove_cvref<text_ostream_T>::char_type>
	constexpr decltype(auto)operator()(text_ostream_T& stream)const noexcept(noexcept_text_ostream_class<text_ostream_T>){
		stream << char_T{'\n'};
		stream.flush();
		return stream;
	}
	//对于换行的读入特殊处理
	template<text_istream_class text_istream_T,class char_T=typename remove_cvref<text_istream_T>::char_type>
	constexpr decltype(auto)operator()(text_istream_T& stream)const noexcept(noexcept_text_istream_class<text_istream_T>){
		char_T ch;
		while(stream>>ch){
			if(ch==char_T{'\n'})
				break;
			if(ch==char_T{'\r'}){
				if(!stream.waitting_for_data() && stream>>ch)
					if(ch!=char_T{'\n'})//可能的\r\n结尾
						stream.unread(&ch,1);
				break;
			}
		}
		return stream;
	}
}endline{};

//operator<< of functions
template<class callable_T,ostream_class stream_T> requires(invoke<callable_T>.able<stream_T>)
decltype(auto)operator<<(stream_T&&stream,callable_T&&callable)noexcept(invoke<callable_T>.nothrow<stream_T>){
	return callable(stream);
}
//operator>> of functions
template<class callable_T,istream_class stream_T> requires(invoke<callable_T>.able<stream_T>)
decltype(auto)operator>>(stream_T&&stream,callable_T&&callable)noexcept(invoke<callable_T>.nothrow<stream_T>){
	return callable(stream);
}

template<text_ostream_class text_ostream_T,class T>
decltype(auto) base_array_output(text_ostream_T&&stream,const T*array,size_t size)noexcept(noexcept_text_ostream_class<text_ostream_T>){
	typedef typename remove_cvref<text_ostream_T>::char_type char_T;
	stream << char_T{'['};
	for(auto i=array;i!=array+size;++i){
		if(i!=array)
			stream << char_T{','};
		stream << *i;
	}
	return stream << char_T{']'};
}
//operator<< of array_t with text_stream
template<text_ostream_class text_ostream_T,class T>
decltype(auto)operator<<(text_ostream_T&&stream,const array_t<T>&array)noexcept(noexcept_text_ostream_class<text_ostream_T>){
	return base_array_output(stream,array.data(),array.size());
}
//operator<< of navie array with text_stream
template<text_ostream_class text_ostream_T,class T,size_t N> requires(type_info<typename remove_cvref<text_ostream_T>::char_type> != type_info<T>)
decltype(auto)operator<<(text_ostream_T&&stream,const T(&array)[N])noexcept(noexcept_text_ostream_class<text_ostream_T>){
	return base_array_output(stream,array,N);
}

//file_end

