//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器base_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
push_and_disable_msvc_warning(26447);//noexcept警告diss
//wrap std::istream to elc::noexcept_text_istream
template<class char_T,class traits=::std::char_traits<char_T>>
struct std_istream_wrap:noexcept_text_istream<char_T>,instance_struct<std_istream_wrap<char_T,traits>>{
	typedef ::std::basic_istream<char_T,traits> stream_t;
	stream_t& _stream;
	std_istream_wrap(stream_t&stream)noexcept:_stream(stream){}
private:
	void clear_badbit()noexcept{
		_stream.clear(_stream.rdstate()&~::std::ios_base::badbit);
	}
public:
	virtual ~std_istream_wrap()noexcept override=default;
	virtual int_t seek(seek_type dir,int_t off)noexcept override{
		typename stream_t::iostate state = stream_t::goodbit;
		const typename stream_t::sentry isok(_stream);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				switch(dir){
					case beg:
						_stream.seekg(off);
						break;
					case cur:
						_stream.seekg(off,_stream.cur);
						break;
					case end:
						_stream.seekg(off,_stream.end);
						break;
				}
				return tell();
			}catch(...){
				state |= stream_t::badbit;
			}
		}
		_stream.setstate(state);
		return -1;
	}
	virtual int_t tell()noexcept override{
		typename stream_t::iostate state = stream_t::goodbit;
		const typename stream_t::sentry isok(_stream);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				const typename stream_t::pos_type base_p;
				return int_t(_stream.tellg()-base_p);
			}catch(...){
				state |= stream_t::badbit;
			}
		}
		_stream.setstate(state);
		return -1;
	}
	virtual void seek_to(int_t pos)noexcept override{
		const typename stream_t::pos_type base_p;
		_stream.seekg(base_p+pos);
	}
	virtual void sync()noexcept override{
		_stream.sync();
	}
	virtual void flush()noexcept override{
		_stream.sync();
	}
	virtual void close()noexcept override{
		_stream.sync();
	}
	virtual size_t read(char_T*buf,size_t size)noexcept override{
		typename stream_t::iostate state = stream_t::goodbit;
		const typename stream_t::sentry isok(_stream);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				_stream.read(buf,size);
				return size_t(_stream.gcount());
			}catch(...){
				state |= stream_t::badbit;
			}
		}
		_stream.setstate(state);
		return 0;
	}
};
//wrap std::ostream to elc::noexcept_text_ostream
template<class char_T,class traits=::std::char_traits<char_T>>
struct std_ostream_wrap:noexcept_text_ostream<char_T>,instance_struct<std_ostream_wrap<char_T,traits>>{
	typedef ::std::basic_ostream<char_T,traits> stream_t;
	stream_t& _stream;
	std_ostream_wrap(stream_t&stream)noexcept:_stream(stream){}
private:
	void clear_badbit()noexcept{
		_stream.clear(_stream.rdstate()&~::std::ios_base::badbit);
	}
public:
	virtual ~std_ostream_wrap()noexcept override=default;
	virtual int_t seek(seek_type dir,int_t off)noexcept override{
		typename stream_t::iostate state = stream_t::goodbit;
		const typename stream_t::sentry isok(_stream);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				switch(dir){
					case beg:
						_stream.seekp(off);
						break;
					case cur:
						_stream.seekp(off,_stream.cur);
						break;
					case end:
						_stream.seekp(off,_stream.end);
						break;
				}
				return tell();
			}catch(...){
				state |= stream_t::badbit;
			}
		}
		_stream.setstate(state);
		return -1;
	}
	virtual int_t tell()noexcept override{
		const typename stream_t::pos_type base_p;
		return int_t(_stream.tellp()-base_p);
	}
	virtual void seek_to(int_t pos)noexcept override{
		const typename stream_t::pos_type base_p;
		_stream.seekp(base_p+pos);
	}
	virtual void sync()noexcept override{
		_stream.flush();
	}
	virtual void flush()noexcept override{
		_stream.flush();
	}
	virtual void close()noexcept override{
		_stream.flush();
	}
	virtual void write(const char_T*buf,size_t size)noexcept override{
		typename stream_t::iostate state = stream_t::goodbit;
		const typename stream_t::sentry isok(_stream);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				_stream.write(buf,size);
			}catch(...){
				state |= stream_t::badbit;
			}
		}
		_stream.setstate(state);
	}
};
pop_msvc_warning();

//inject operator<< of elc::array_like_view_t to std::ostream
BREAK_NAMESPACE//elc::defs
INTER_NAMESPACE(base::array_like_n)//elc::defs::base::array_like_n

template<class char_T,class traits>
auto&operator<<(
	::std::basic_ostream<remove_cvref<char_T>,traits>&stream,
	const array_like_view_t<char_T>&view
)noexcept{
	stream_n::std_ostream_wrap wrap(stream);
	wrap << view;
	return stream;
}

BREAK_NAMESPACE//elc::defs
INTER_NAMESPACE(stream_n)//elc::defs::stream_n

//file_end

