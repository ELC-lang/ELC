//defs.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct base_frefstream:virtual noexcept_stream,instance_struct<base_frefstream>{
protected:
	FILE*_file;
public:
	base_frefstream(FILE*file):_file(file){}
	virtual int_t seek(seek_type dir,int_t off)noexcept override{
		switch (dir){
			case beg:
				::std::fseek(_file,(long)off,SEEK_SET);
				break;
			case end:
				::std::fseek(_file,(long)off,SEEK_END);
				break;
			case cur:
				::std::fseek(_file,(long)off,SEEK_CUR);
				break;
			default:
				return -1;
		}
		return ::std::ftell(_file);
	}
	virtual int_t tell()noexcept override{
		return ::std::ftell(_file);
	}
	virtual void seek_to(int_t pos)noexcept override{
		::std::fseek(_file,(long)pos,SEEK_SET);
	}
	virtual void sync()noexcept override{
		::std::fflush(_file);
	}
	virtual void flush()noexcept override{
		::std::fflush(_file);
	}
	virtual void close()noexcept override{
		::std::fclose(_file);
	}
};
struct base_fstream:base_frefstream,instance_struct<base_fstream>{
private:
	typedef base_frefstream base_t;
	typedef base_fstream this_t;
	override_instance_struct;
public:
	using base_t::base_t;

	virtual ~base_fstream()noexcept override{
		if(base_t::_file)
			::std::fclose(base_t::_file);
	}
};
template<class char_T,class base_fstream_T>
struct base_text_ifstream_wrapper:base_fstream_T,virtual noexcept_text_istream<char_T>,instance_struct<base_text_ifstream_wrapper<char_T,base_fstream_T>>{
private:
	typedef base_fstream_T base_t;
	typedef base_text_ifstream_wrapper<char_T,base_fstream_T> this_t;
	override_instance_struct;
public:
	using base_t::base_t;

	virtual size_t read(char_T*buf,size_t size)noexcept override{
		return ::std::fread(buf,sizeof(char_T),size,base_t::_file);
	}
};
template<class char_T,class base_fstream_T>
struct base_text_ofstream_wrapper:base_fstream_T,virtual noexcept_text_ostream<char_T>,instance_struct<base_text_ofstream_wrapper<char_T,base_fstream_T>>{
private:
	typedef base_fstream_T base_t;
	typedef base_text_ofstream_wrapper<char_T,base_fstream_T> this_t;
	override_instance_struct;
public:
	using base_t::base_t;

	virtual void write(const char_T*buf,size_t size)noexcept override{
		::std::fwrite(buf,sizeof(char_T),size,base_t::_file);
	}
};
template<class char_T>
using base_text_ifstream=base_text_ifstream_wrapper<char_T,base_fstream>;
template<class char_T>
using base_text_ofstream=base_text_ofstream_wrapper<char_T,base_fstream>;
template<class char_T>
using base_text_irefstream=base_text_ifstream_wrapper<char_T,base_frefstream>;
template<class char_T>
using base_text_orefstream=base_text_ofstream_wrapper<char_T,base_frefstream>;

distinctive inline struct output_stream_t:base_text_orefstream<char_t>,instance_struct<output_stream_t>{
private:
	typedef base_text_orefstream<char_t> base_t;
	typedef output_stream_t this_t;
	override_instance_struct;
public:
	output_stream_t()noexcept:base_t(stdout){
		APIs::streams::init_output_stream();
	}
	virtual void write(const char_t*buf,size_t size)noexcept override{
		APIs::streams::write_text_to_stream(base_t::_file,buf,size);
	}
}out{};
distinctive inline struct error_stream_t:base_text_orefstream<char_t>,instance_struct<error_stream_t>{
private:
	typedef base_text_orefstream<char_t> base_t;
	typedef error_stream_t this_t;
	override_instance_struct;
public:
	error_stream_t()noexcept:base_t(stderr){
		APIs::streams::init_error_stream();
	}
	virtual void write(const char_t*buf,size_t size)noexcept override{
		APIs::streams::write_text_to_stream(base_t::_file,buf,size);
	}
}err{};
distinctive inline struct input_stream_t:base_text_irefstream<char_t>,instance_struct<input_stream_t>{
private:
	typedef base_text_irefstream<char_t> base_t;
	typedef input_stream_t this_t;
	override_instance_struct;
public:
	input_stream_t()noexcept:base_t(stdin){
		APIs::streams::init_input_stream();
	}
	virtual size_t read(char_t*buf,size_t size)noexcept override{
		return APIs::streams::read_text_from_stream(base_t::_file,buf,size);
	}
}in{};

//file_end

