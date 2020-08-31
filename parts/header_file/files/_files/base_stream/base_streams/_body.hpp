//_body.hpp
//at namespace elc
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
enum seek_type{beg,end,cur};
struct base_stream{
	virtual ~base_stream()=default;
	virtual void*seek(seek_type,elint)=0;
	virtual void*tell(){return this->seek(cur,0);}
	virtual void seek_to(void*)=0;
	virtual void sync()=0;
};
struct base_ostream:virtual base_stream{
	virtual ~base_ostream()=default;
	virtual void write(const void*buf,size_t size)=0;
};
struct base_istream:virtual base_stream{
	virtual ~base_istream()=default;
	virtual size_t read(void*buf,size_t size)=0;
};
struct base_iostream:virtual base_istream,virtual base_ostream{};


struct noexcept_stream:virtual base_ostream{
	virtual ~noexcept_stream()noexcept override=default;
	virtual void*seek(seek_type,elint)noexcept override=0;
	virtual void*tell()noexcept override{return this->seek(cur,0);}
	virtual void seek_to(void*)noexcept override=0;
	virtual void sync()noexcept override=0;
};
struct noexcept_ostream:virtual base_ostream,virtual noexcept_stream{
	virtual void write(const void*buf,size_t size)noexcept override=0;
};
struct noexcept_istream:virtual base_istream,virtual noexcept_stream{
	virtual size_t read(void*buf,size_t size)noexcept override=0;
};
struct noexcept_iostream:virtual base_iostream,virtual noexcept_ostream,virtual noexcept_istream{};
static_assert(noexcept(((noexcept_iostream*)(nullptr))->~noexcept_iostream()));
static_assert(noexcept(((noexcept_iostream*)(nullptr))->sync()));

//file_end

