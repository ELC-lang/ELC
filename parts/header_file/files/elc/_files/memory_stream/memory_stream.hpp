//memory_stream.hpp
//at namespace elc::defs
/*
未完成的elc解释器memory_stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct memory_stream:noexcept_iostream{
	constexpr size_t buf_size=pow(10,BIT_POSSIBILITY);
	byte* _m;
	size_t _s;
	size_t _pos;
	memory_stream()noexcept{
		_m = get<byte>[buf_size]();
		_pos = 0;
		_s = 0;
	}
	~memory_stream()noexcept{
		unget(_m);
	}
	[[nodiscard]]size_t bufsize()const noexcept{
		return get_size_of_get(_m);
	}
	virtual void* seek(seek_type st,int_t offsize)noexcept override{
		switch(st){
			case beg:
				_pos = 0;
				return _m;
			case cur:
				_pos += (ptrdiff_t)offsize;
				return _m + _pos;
			case end:
				_pos = _s;
				return _m+_pos;
			default:
				return nullptr;
		}
	}
	virtual void seek_to(void*p)noexcept override{
		_pos = ((byte*)p)-_m;
	}
	virtual void sync()noexcept override{
		//do nothing
	}
	virtual void flush()noexcept override{
		//do nothing
	}
	virtual void close()noexcept override{
		unget(_m);
		_m = nullptr;
		_s = 0;
	}
	virtual void write(const byte*buf,size_t size)noexcept override{
		if(size>this->bufsize()-_pos)
			get_resize(_m,get_next_gold_size_to_resize_for_array(_pos+size));
		::std::memcpy(_m+_pos,buf,size);
		_pos += size;
		_s = max(_pos,_s);
	}
	virtual size_t read(byte*buf,size_t size)noexcept override{
		if(_pos+size>_s)
			size = _s-_pos;
		::std::memcpy(buf,_m+_pos,size);
		_pos += size;
		return size;
	}
};

//file_end

