//_body.hpp
//at namespace elc
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4250)
#endif
enum seek_type{beg,end,cur};
no_vtable_struct base_stream{
	virtual ~base_stream()=default;
	virtual void*seek(seek_type,int_t)=0;
	virtual void*tell(){return this->seek(cur,0);}
	virtual void seek_to(void*)=0;
	virtual void sync()=0;
	virtual void flush()=0;
	virtual void close()=0;
};
no_vtable_struct base_ostream:virtual base_stream{
	virtual ~base_ostream()=default;
	virtual void write(const byte*buf,size_t size)=0;
	template<class T>
	void write(const T*v,size_t size=1){
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct base_istream:virtual base_stream{
	virtual ~base_istream()=default;
	virtual size_t read(byte*buf,size_t size)=0;
	template<class T>
	size_t read(T*v,size_t size=1){
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct base_iostream:virtual base_istream,virtual base_ostream{};


no_vtable_struct noexcept_stream:virtual base_stream{
	virtual ~noexcept_stream()noexcept override=default;
	virtual void*seek(seek_type,int_t)noexcept override=0;
	virtual void*tell()noexcept override{return this->seek(cur,0);}
	virtual void seek_to(void*)noexcept override=0;
	virtual void sync()noexcept override=0;
	virtual void flush()noexcept override=0;
	virtual void close()noexcept override=0;
};
no_vtable_struct noexcept_ostream:virtual base_ostream,virtual noexcept_stream{
	virtual void write(const byte*buf,size_t size)noexcept override=0;
	template<class T>
	void write(const T*v,size_t size=1)noexcept{
		this->write(cast_to_data(v),size*sizeof(T));
	}
};
no_vtable_struct noexcept_istream:virtual base_istream,virtual noexcept_stream{
	virtual size_t read(byte*buf,size_t size)noexcept override=0;
	template<class T>
	size_t read(T*v,size_t size=1)noexcept{
		return this->read(cast_to_data(v),size*sizeof(T))/sizeof(T);
	}
};
no_vtable_struct noexcept_iostream:virtual base_iostream,virtual noexcept_ostream,virtual noexcept_istream{};
static_assert(noexcept(((noexcept_iostream*)(nullptr))->~noexcept_iostream()));
static_assert(noexcept(((noexcept_iostream*)(nullptr))->sync()));

//string streams
no_vtable_struct string_istream: virtual base_istream{};
no_vtable_struct string_ostream: virtual base_ostream{};
no_vtable_struct string_iostream: virtual string_istream,virtual string_ostream,virtual base_iostream{};
//noexcept_
no_vtable_struct noexcept_string_istream: virtual noexcept_istream,virtual string_istream{};
no_vtable_struct noexcept_string_ostream: virtual noexcept_ostream,virtual string_ostream{};
no_vtable_struct noexcept_string_iostream: virtual noexcept_string_istream,virtual noexcept_string_ostream,virtual noexcept_iostream,virtual string_iostream{};


//data streams
no_vtable_struct data_istream: virtual base_istream{};
no_vtable_struct data_ostream: virtual base_ostream{};
no_vtable_struct data_iostream: virtual data_istream,virtual data_ostream,virtual base_iostream{};
//noexcept_
no_vtable_struct noexcept_data_istream: virtual noexcept_istream,virtual data_istream{};
no_vtable_struct noexcept_data_ostream: virtual noexcept_ostream,virtual data_ostream{};
no_vtable_struct noexcept_data_iostream: virtual noexcept_data_istream,virtual noexcept_data_ostream,virtual noexcept_iostream,virtual data_iostream{};

#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

//file_end

