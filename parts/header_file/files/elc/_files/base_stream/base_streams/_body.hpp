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
no_vtable_struct base_stream:abstract_base<base_stream>{
	virtual ~base_stream()=default;
	virtual int_t seek(seek_type,int_t)=0;
	virtual int_t tell(){return this->seek(cur,0);}
	virtual void seek_to(int_t)=0;
	virtual void sync()=0;
	virtual void flush()=0;
	virtual void close()=0;
};
no_vtable_struct base_istream:abstract_base<base_istream>,virtual base_stream{};
no_vtable_struct base_ostream:abstract_base<base_ostream>,virtual base_stream{};
no_vtable_struct base_iostream:abstract_base<base_iostream>,virtual base_ostream,virtual base_istream{};


no_vtable_struct noexcept_stream:abstract_base<noexcept_stream>,virtual base_stream{
	virtual int_t seek(seek_type,int_t)noexcept override=0;
	virtual int_t tell()noexcept override{return this->seek(cur,0);}
	virtual void seek_to(int_t)noexcept override=0;
	virtual void sync()noexcept override=0;
	virtual void flush()noexcept override=0;
	virtual void close()noexcept override=0;
};
no_vtable_struct noexcept_istream:abstract_base<noexcept_istream>,virtual base_istream,virtual noexcept_stream{};
no_vtable_struct noexcept_ostream:abstract_base<noexcept_ostream>,virtual base_ostream,virtual noexcept_stream{};
no_vtable_struct noexcept_iostream:abstract_base<noexcept_iostream>,virtual base_iostream,virtual noexcept_ostream,virtual noexcept_istream{};
static_assert(noexcept(((noexcept_iostream*)(nullptr))->~noexcept_iostream()));
static_assert(noexcept(((noexcept_iostream*)(nullptr))->sync()));

//text streams
template<class char_T>
no_vtable_struct text_istream:abstract_base<text_istream<char_T>>,virtual base_istream{
	virtual size_t read(char_T*buf,size_t size)=0;
};
template<class char_T>
no_vtable_struct text_ostream:abstract_base<text_ostream<char_T>>,virtual base_ostream{
	virtual void write(const char_T*buf,size_t size)=0;
};
template<class char_T>
no_vtable_struct text_iostream:abstract_base<text_iostream<char_T>>,virtual base_iostream,virtual text_istream<char_T>,virtual text_ostream<char_T>{};
//noexcept_
template<class char_T>
no_vtable_struct noexcept_text_istream:abstract_base<noexcept_text_istream<char_T>>,virtual noexcept_istream,virtual text_istream<char_T>{
	virtual size_t read(char_T*buf,size_t size)noexcept override=0;
};
template<class char_T>
no_vtable_struct noexcept_text_ostream:abstract_base<noexcept_text_ostream<char_T>>,virtual noexcept_ostream,virtual text_ostream<char_T>{
	virtual void write(const char_T*buf,size_t size)noexcept override=0;
};
template<class char_T>
no_vtable_struct noexcept_text_iostream:abstract_base<noexcept_text_iostream<char_T>>,
	virtual noexcept_iostream,virtual text_iostream<char_T>,
	virtual noexcept_text_istream<char_T>,virtual noexcept_text_ostream<char_T>{};


//data streams
no_vtable_struct data_istream:abstract_base<data_istream>,virtual base_istream{
	virtual size_t read(byte*buf,size_t size)=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	size_t read(T*v,size_t size=1){
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct data_ostream:abstract_base<data_ostream>,virtual base_ostream{
	virtual void write(const byte*buf,size_t size)=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	void write(const T*v,size_t size=1){
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct data_iostream:abstract_base<data_iostream>,virtual base_iostream,virtual data_istream,virtual data_ostream{};
//noexcept_
no_vtable_struct noexcept_data_istream:abstract_base<noexcept_data_istream>,virtual noexcept_istream,virtual data_istream{
	virtual size_t read(byte*buf,size_t size)noexcept override=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	size_t read(T*v,size_t size=1)noexcept{
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct noexcept_data_ostream:abstract_base<noexcept_data_ostream>,virtual noexcept_ostream,virtual data_ostream{
	virtual void write(const byte*buf,size_t size)noexcept override=0;
	template<class T> requires ::std::is_trivially_copyable_v<T>
	void write(const T*v,size_t size=1)noexcept{
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct noexcept_data_iostream:abstract_base<noexcept_data_iostream>,
	virtual noexcept_iostream,virtual data_iostream,
	virtual noexcept_data_istream,virtual noexcept_data_ostream{};

pop_msvc_warning();

//file_end

