//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
push_and_disable_msvc_warning(4250);
enum class seek_type{beg,end,cur};
using enum seek_type;
no_vtable_struct base_stream_t:abstract_base<base_stream_t>{
	virtual ~base_stream_t()=default;
	virtual int_t seek(seek_type,int_t)=0;
	virtual int_t tell(){return this->seek(cur,0);}
	virtual void seek_to(int_t)=0;
	virtual void sync()=0;
	virtual void flush()=0;
	virtual void close()=0;
};
no_vtable_struct base_istream_t:abstract_base<base_istream_t>,virtual base_stream_t{};
no_vtable_struct base_ostream_t:abstract_base<base_ostream_t>,virtual base_stream_t{};
no_vtable_struct base_iostream_t:abstract_base<base_iostream_t>,virtual base_ostream_t,virtual base_istream_t{};

//concept: any class that base on base_stream_t
template<class T>
concept stream_class=is_base_of<base_stream_t,T>;
template<class T>
concept istream_class=is_base_of<base_istream_t,T>;
template<class T>
concept ostream_class=is_base_of<base_ostream_t,T>;
template<class T>
concept iostream_class=is_base_of<base_iostream_t,T>;


no_vtable_struct noexcept_stream_t:abstract_base<noexcept_stream_t>,virtual base_stream_t{
	virtual int_t seek(seek_type,int_t)noexcept override=0;
	virtual int_t tell()noexcept override{return this->seek(cur,0);}
	virtual void seek_to(int_t)noexcept override=0;
	virtual void sync()noexcept override=0;
	virtual void flush()noexcept override=0;
	virtual void close()noexcept override=0;
};
no_vtable_struct noexcept_istream_t:abstract_base<noexcept_istream_t>,virtual base_istream_t,virtual noexcept_stream_t{};
no_vtable_struct noexcept_ostream_t:abstract_base<noexcept_ostream_t>,virtual base_ostream_t,virtual noexcept_stream_t{};
no_vtable_struct noexcept_iostream_t:abstract_base<noexcept_iostream_t>,virtual base_iostream_t,virtual noexcept_ostream_t,virtual noexcept_istream_t{};
static_assert(noexcept(((noexcept_iostream_t*)(nullptr))->~noexcept_iostream_t()));
static_assert(noexcept(((noexcept_iostream_t*)(nullptr))->sync()));

//concept: any class that base on noexcept_stream_t
template<class T>
concept noexcept_stream_class=is_base_of<noexcept_stream_t,T>;
template<class T>
concept noexcept_istream_class=is_base_of<noexcept_istream_t,T>;
template<class T>
concept noexcept_ostream_class=is_base_of<noexcept_ostream_t,T>;
template<class T>
concept noexcept_iostream_class=is_base_of<noexcept_iostream_t,T>;

//text streams
template<class char_T>
no_vtable_struct text_stream_t:abstract_base<text_stream_t<char_T>>,virtual base_stream_t{
	typedef char_T char_type;
};
template<class char_T>
no_vtable_struct text_istream_t:abstract_base<text_istream_t<char_T>>,virtual base_istream_t,virtual text_stream_t<char_T>{
	virtual size_t read(char_T*buf,size_t size)=0;
};
template<class char_T>
no_vtable_struct text_ostream_t:abstract_base<text_ostream_t<char_T>>,virtual base_ostream_t,virtual text_stream_t<char_T>{
	virtual void write(const char_T*buf,size_t size)=0;
};
template<class char_T>
no_vtable_struct text_iostream_t:abstract_base<text_iostream_t<char_T>>,virtual base_iostream_t,virtual text_istream_t<char_T>,virtual text_ostream_t<char_T>{};

//concept: any class that base on text_stream_t<char_t>
template<class T>
concept text_stream_class=is_base_of<text_stream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept text_istream_class=is_base_of<text_istream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept text_ostream_class=is_base_of<text_ostream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept text_iostream_class=is_base_of<text_iostream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept text_stream=is_base_of<text_stream_t<char_t>,T>;
template<class T>
concept text_istream=is_base_of<text_istream_t<char_t>,T>;
template<class T>
concept text_ostream=is_base_of<text_ostream_t<char_t>,T>;
template<class T>
concept text_iostream=is_base_of<text_iostream_t<char_t>,T>;

//noexcept_
template<class char_T>
no_vtable_struct noexcept_text_istream_t:abstract_base<noexcept_text_istream_t<char_T>>,virtual noexcept_istream_t,virtual text_istream_t<char_T>{
	virtual size_t read(char_T*buf,size_t size)noexcept override=0;
};
template<class char_T>
no_vtable_struct noexcept_text_ostream_t:abstract_base<noexcept_text_ostream_t<char_T>>,virtual noexcept_ostream_t,virtual text_ostream_t<char_T>{
	virtual void write(const char_T*buf,size_t size)noexcept override=0;
};
template<class char_T>
no_vtable_struct noexcept_text_iostream_t:abstract_base<noexcept_text_iostream_t<char_T>>,
	virtual noexcept_iostream_t,virtual text_iostream_t<char_T>,
	virtual noexcept_text_istream_t<char_T>,virtual noexcept_text_ostream_t<char_T>{};

//concept: any class that base on noexcept_text_stream<char_t>
template<class T>
concept noexcept_text_istream_class=is_base_of<noexcept_text_istream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept noexcept_text_ostream_class=is_base_of<noexcept_text_ostream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept noexcept_text_iostream_class=is_base_of<noexcept_text_iostream_t<typename remove_cvref<T>::char_type>,T>;
template<class T>
concept noexcept_text_istream=is_base_of<noexcept_text_istream_t<char_t>,T>;
template<class T>
concept noexcept_text_ostream=is_base_of<noexcept_text_ostream_t<char_t>,T>;
template<class T>
concept noexcept_text_iostream=is_base_of<noexcept_text_iostream_t<char_t>,T>;

//data streams
no_vtable_struct data_istream_t:abstract_base<data_istream_t>,virtual base_istream_t{
	virtual size_t read(byte*buf,size_t size)=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	size_t read(T*v,size_t size=1){
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct data_ostream_t:abstract_base<data_ostream_t>,virtual base_ostream_t{
	virtual void write(const byte*buf,size_t size)=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	void write(const T*v,size_t size=1){
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct data_iostream_t:abstract_base<data_iostream_t>,virtual base_iostream_t,virtual data_istream_t,virtual data_ostream_t{};

//concept: any class that base on data_stream
template<class T>
concept data_istream_class=is_base_of<data_istream_t,T>;
template<class T>
concept data_ostream_class=is_base_of<data_ostream_t,T>;
template<class T>
concept data_iostream_class=is_base_of<data_iostream_t,T>;

//noexcept_
no_vtable_struct noexcept_data_istream_t:abstract_base<noexcept_data_istream_t>,virtual noexcept_istream_t,virtual data_istream_t{
	virtual size_t read(byte*buf,size_t size)noexcept override=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	size_t read(T*v,size_t size=1)noexcept{
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct noexcept_data_ostream_t:abstract_base<noexcept_data_ostream_t>,virtual noexcept_ostream_t,virtual data_ostream_t{
	virtual void write(const byte*buf,size_t size)noexcept override=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	void write(const T*v,size_t size=1)noexcept{
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct noexcept_data_iostream_t:abstract_base<noexcept_data_iostream_t>,
	virtual noexcept_iostream_t,virtual data_iostream_t,
	virtual noexcept_data_istream_t,virtual noexcept_data_ostream_t{};

//concept: any class that base on noexcept_data_stream
template<class T>
concept noexcept_data_istream_class=is_base_of<noexcept_data_istream_t,T>;
template<class T>
concept noexcept_data_ostream_class=is_base_of<noexcept_data_ostream_t,T>;
template<class T>
concept noexcept_data_iostream_class=is_base_of<noexcept_data_iostream_t,T>;

pop_msvc_warning();
//special streams
template<class char_T>
struct null_text_stream:noexcept_text_iostream_t<char_T>,instance_struct<null_text_stream<char_T>>{
	virtual int_t seek(seek_type,int_t)noexcept override{return 0;}
	virtual int_t tell()noexcept override{return 0;}
	virtual void seek_to(int_t)noexcept override{}
	virtual void sync()noexcept override{}
	virtual void flush()noexcept override{}
	virtual void close()noexcept override{}
	virtual void write(const char_T*,size_t)noexcept override{}
	virtual size_t read(char_T*,size_t)noexcept override{return 0;}
};
struct null_data_stream:noexcept_data_iostream_t,instance_struct<null_data_stream>{
	virtual int_t seek(seek_type,int_t)noexcept override{return 0;}
	virtual int_t tell()noexcept override{return 0;}
	virtual void seek_to(int_t)noexcept override{}
	virtual void sync()noexcept override{}
	virtual void flush()noexcept override{}
	virtual void close()noexcept override{}
	virtual void write(const byte*,size_t)noexcept override{}
	virtual size_t read(byte*,size_t)noexcept override{return 0;}
};

//file_end

