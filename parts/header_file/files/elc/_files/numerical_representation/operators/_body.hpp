//_body.hpp
//at namespace elc::defs::numerical_representation_n
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<numerical_representation representation_T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream_t<char_t>>)
struct specified_numerical_representation_ostream_wrapper: virtual text_ostream_t<char_t>, instance_struct<specified_numerical_representation_ostream_wrapper<representation_T,stream_T>>{
	const representation_T& representation;
	stream_T& stream;

	specified_numerical_representation_ostream_wrapper(const representation_T& representation,stream_T& stream)noexcept:representation(representation),stream(stream){}

	virtual int_t seek(seek_type dir,int_t off)noexcept_as(stream.seek(seek_type::cur,0)) override{return stream.seek(dir,off);}
	virtual int_t tell()noexcept_as(stream.tell()) override{return stream.tell();}
	virtual void seek_to(int_t pos)noexcept_as(stream.seek_to(pos)) override{return stream.seek_to(pos);}
	virtual void sync()noexcept_as(stream.sync()) override{return stream.sync();}
	virtual void flush()noexcept_as(stream.flush()) override{return stream.flush();}
	virtual void close()noexcept_as(stream.close()) override{return stream.close();}
	virtual void write(const char_t*data,size_t size)noexcept_as(stream.write(data,size)) override{
		stream.write(data,size);
	}

	//获取包装的流
	stream_T& get_wrapped_stream()noexcept{return stream;}
	//获取数字表示方式
	const representation_T& get_numerical_representation()const noexcept{return representation;}
};
template<numerical_representation representation_T,class stream_T> requires(type_info<stream_T>.base_on<text_ostream_t<char_t>>)
inline auto operator<<(stream_T&stream,const representation_T& representation)noexcept_as(specified_numerical_representation_ostream_wrapper<representation_T,stream_T>(representation,stream)){
	return specified_numerical_representation_ostream_wrapper<representation_T,stream_T>{representation,stream};
}

template<numerical_representation representation_T,class stream_T> requires(type_info<stream_T>.base_on<text_istream_t<char_t>>)
struct specified_numerical_representation_istream_wrapper:virtual text_istream_t<char_t>,instance_struct<specified_numerical_representation_istream_wrapper<representation_T,stream_T>>{
	const representation_T& representation;
	stream_T& stream;

	specified_numerical_representation_istream_wrapper(const representation_T& representation,stream_T& stream)noexcept:representation(representation),stream(stream){}

	virtual int_t seek(seek_type dir,int_t off)noexcept_as(stream.seek(seek_type::cur,0)) override{return stream.seek(dir,off);}
	virtual int_t tell()noexcept_as(stream.tell()) override{return stream.tell();}
	virtual void seek_to(int_t pos)noexcept_as(stream.seek_to(pos)) override{return stream.seek_to(pos);}
	virtual void sync()noexcept_as(stream.sync()) override{return stream.sync();}
	virtual void flush()noexcept_as(stream.flush()) override{return stream.flush();}
	virtual void close()noexcept_as(stream.close()) override{return stream.close();}
	virtual void read(char_t*data,size_t size)noexcept_as(stream.read(data,size)) override{
		stream.read(data,size);
	}

	//获取包装的流
	stream_T& get_wrapped_stream()noexcept{return stream;}
	//获取数字表示方式
	const representation_T& get_numerical_representation()const noexcept{return representation;}
};
template<numerical_representation representation_T,class stream_T> requires(type_info<stream_T>.base_on<text_istream_t<char_t>>)
inline auto operator>>(stream_T&stream,const representation_T& representation)noexcept_as(specified_numerical_representation_istream_wrapper<representation_T,stream_T>(representation,stream)){
	return specified_numerical_representation_istream_wrapper<representation_T,stream_T>{representation,stream};
}

//file_end

