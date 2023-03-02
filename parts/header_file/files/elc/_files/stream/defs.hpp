//defs.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace base_streams_impl_n{
	using namespace elc::APIs::streams;
	using namespace base_fstream_n;

	#define inheriting_base_stream_operations_from(expr) \
	\
	virtual int_t seek(seek_type dir,int_t off)noexcept override{\
		return (expr).seek(dir,off);\
	}\
	virtual int_t tell()noexcept override{\
		return (expr).tell();\
	}\
	virtual bool seek_to(int_t pos)noexcept override{\
		return (expr).seek_to(pos);\
	}\
	virtual void sync()noexcept override{\
		(expr).sync();\
	}\
	virtual void flush()noexcept override{\
		(expr).flush();\
	}\
	virtual void close()noexcept override{\
		(expr).close();\
	}\

	#define inheriting_base_stream_operations_from_pointer(ptr) \
			inheriting_base_stream_operations_from(*ptr)

	#define inheriting_istream_operations_from(expr) \
			inheriting_base_stream_operations_from(expr)\
	\
	virtual bool is_end()noexcept override{\
		return (expr).is_end();\
	}\
	virtual bool waitting_for_data()noexcept override{\
		return (expr).waitting_for_data();\
	}\
	virtual size_t read(char_t*buf,size_t size)noexcept override{\
		return (expr).read(buf,size);\
	}\
	virtual void unread(const char_t*buf,size_t size)noexcept override{\
		(expr).unread(buf,size);\
	}\
	virtual explicit operator bool()noexcept override{\
		return (expr).operator bool();\
	}\

	#define inheriting_istream_operations_from_pointer(ptr) \
			inheriting_istream_operations_from(*ptr)

	#define inheriting_ostream_operations_from(expr) \
			inheriting_base_stream_operations_from(expr)\
	\
	virtual void write(const char_t*buf,size_t size)noexcept override{\
		(expr).write(buf,size);\
	}\

	#define inheriting_ostream_operations_from_pointer(ptr) \
			inheriting_ostream_operations_from(*ptr)


	template<class char_T>
	struct not_unreadable_code_cvted_text_irefstream:virtual noexcept_not_unreadable_text_istream_t<char_t>,instance_struct<not_unreadable_code_cvted_text_irefstream<char_T>>{
	private:
		typedef noexcept_not_unreadable_text_istream_t<char_t> base_t;
		typedef not_unreadable_code_cvted_text_irefstream this_t;
		override_instance_struct;

		noexcept_text_istream_t<char_T>*_base;
	public:
		explicit not_unreadable_code_cvted_text_irefstream(noexcept_text_istream_t<char_T>&base)noexcept:_base(&base){}
		virtual ~not_unreadable_code_cvted_text_irefstream()noexcept override=default;

		inheriting_base_stream_operations_from_pointer(_base);

		virtual bool is_end()noexcept override{
			return _base->is_end();
		}
		virtual bool waitting_for_data()noexcept override{
			return _base->waitting_for_data();
		}
		virtual size_t read(char_t*buf,size_t size)noexcept override{
			constexpr size_t this_code_convert_buf_size=char_set::to_u32_code_size<char_T>;
			char_T code_convert_buf[this_code_convert_buf_size];
			size_t readed=0;
			while(readed<size){
				size_t readed_this_time=_base->read(code_convert_buf,this_code_convert_buf_size);
				if(readed_this_time==0)
					break;
				auto result=char_set::auto_to_utf32(buf+readed,code_convert_buf,readed_this_time);
				if(!result)
					die_with(locale::str::code_convert_error);
				readed+=result.processed_output().size();
				auto size_processed_input=result.processed_input().size();
				_base->unread(code_convert_buf+size_processed_input,readed_this_time-size_processed_input);
			}
			return readed;
		}
	};
	template<class char_T>
	using code_cvted_text_irefstream=unreadable_wrap<not_unreadable_code_cvted_text_irefstream<char_T>>;
	template<class char_T>
	struct code_cvted_text_orefstream:virtual noexcept_text_ostream_t<char_t>,instance_struct<code_cvted_text_orefstream<char_T>>{
	private:
		typedef noexcept_text_ostream_t<char_t> base_t;
		typedef code_cvted_text_orefstream this_t;
		override_instance_struct;

		noexcept_text_ostream_t<char_T>*_base;
	public:
		explicit code_cvted_text_orefstream(noexcept_text_ostream_t<char_T>&base)noexcept:_base(&base){}
		virtual ~code_cvted_text_orefstream()noexcept override=default;

		inheriting_base_stream_operations_from_pointer(_base);

		virtual void write(const char_t*buf,size_t size)noexcept override{
			constexpr size_t this_code_convert_buf_size=char_set::to_u32_code_size<char_T>;
			char_T code_convert_buf[this_code_convert_buf_size];
			size_t writed=0;
			while(writed<size){
				auto result=char_set::utf32_to_auto(buf[writed],code_convert_buf);
				if(!result)
					die_with(locale::str::code_convert_error);
				_base->write(code_convert_buf,result.processed_output().size());
				writed++;
			}
		}
	};

	struct terminal_irefstream_impl:virtual noexcept_text_istream_t<char_t>,instance_struct<terminal_irefstream_impl>{
	private:
		typedef noexcept_text_istream_t<char_t> base_t;
		typedef terminal_irefstream_impl this_t;
		override_instance_struct;

		system_terminal_irefstream _in;
		code_cvted_text_irefstream<base_io_char_type> _code_cvt{_in};
	public:
		terminal_irefstream_impl(handle_type handle)noexcept:_in(handle){}
		virtual ~terminal_irefstream_impl()noexcept override=default;

		inheriting_istream_operations_from(_code_cvt);
	};
	struct terminal_orefstream_impl:virtual noexcept_text_ostream_t<char_t>,instance_struct<terminal_orefstream_impl>{
	private:
		typedef noexcept_text_ostream_t<char_t> base_t;
		typedef terminal_orefstream_impl this_t;
		override_instance_struct;

		system_terminal_orefstream _out;
		code_cvted_text_orefstream<base_io_char_type> _code_cvt{_out};
	public:
		terminal_orefstream_impl(handle_type handle)noexcept:_out(handle){}
		virtual ~terminal_orefstream_impl()noexcept override=default;

		inheriting_ostream_operations_from(_code_cvt);
	};
	//文件流读入（自句柄读写）实现
	//目前懒得写各种编码转换，所以目前只假设编码utf-8。
	struct file_irefstream_impl:virtual noexcept_text_istream_t<char_t>,instance_struct<file_irefstream_impl>{
	private:
		typedef noexcept_text_istream_t<char_t> base_t;
		typedef file_irefstream_impl this_t;
		override_instance_struct;

		base_text_irefstream<char8_t> _in;
		code_cvted_text_irefstream<char8_t> _code_cvt{_in};
	public:
		file_irefstream_impl(handle_type handle)noexcept:_in(handle){}
		virtual ~file_irefstream_impl()noexcept override=default;

		inheriting_istream_operations_from(_code_cvt);
	};
	//文件流写出（自句柄读写）实现
	//目前懒得写各种编码转换，所以目前只假设编码utf-8。
	struct file_orefstream_impl:virtual noexcept_text_ostream_t<char_t>,instance_struct<file_orefstream_impl>{
	private:
		typedef noexcept_text_ostream_t<char_t> base_t;
		typedef file_orefstream_impl this_t;
		override_instance_struct;

		base_text_orefstream<char8_t> _out;
		code_cvted_text_orefstream<char8_t> _code_cvt{_out};
	public:
		file_orefstream_impl(handle_type handle)noexcept:_out(handle){}
		virtual ~file_orefstream_impl()noexcept override=default;

		inheriting_ostream_operations_from(_code_cvt);
	};
	//句柄io流
	struct handle_istream_impl:virtual noexcept_text_istream_t<char_t>,instance_struct<handle_istream_impl>{
	private:
		typedef noexcept_text_istream_t<char_t> base_t;
		typedef handle_istream_impl this_t;
		override_instance_struct;

		noexcept_text_istream_t<char_t>*_base;
	public:
		handle_istream_impl(handle_type handle)noexcept{
			using defs::get;//貌似msvc在这里有bug
			if(handle.is_terminal())
				_base=get<terminal_irefstream_impl>(handle);
			else
				_base=get<file_irefstream_impl>(handle);
		}
		virtual ~handle_istream_impl()noexcept override{
			unget(_base);
		}

		inheriting_istream_operations_from_pointer(_base);
	};
	struct handle_ostream_impl:virtual noexcept_text_ostream_t<char_t>,instance_struct<handle_ostream_impl>{
	private:
		typedef noexcept_text_ostream_t<char_t> base_t;
		typedef handle_ostream_impl this_t;
		override_instance_struct;

		noexcept_text_ostream_t<char_t>*_base;
	public:
		handle_ostream_impl(handle_type handle)noexcept{
			if(handle.is_terminal())
				_base=get<terminal_orefstream_impl>(handle);
			else
				_base=get<file_orefstream_impl>(handle);
		}
		virtual ~handle_ostream_impl()noexcept override{
			unget(_base);
		}

		inheriting_ostream_operations_from_pointer(_base);
	};

	distinctive inline auto ori_base_in_handle{init_input_stream()};
	distinctive inline auto ori_base_out_handle{init_output_stream()};
	distinctive inline auto ori_base_err_handle{init_error_stream()};

	distinctive inline handle_ostream_impl out_impl{ori_base_out_handle};
	distinctive inline handle_ostream_impl err_impl{ori_base_err_handle};
	//由于in需要回显，所以需要多一层定义以处理回显相关事宜
	distinctive inline struct in_impl_t:noexcept_text_istream_t<char_t>,instance_struct<in_impl_t>{
	private:
		handle_istream_impl _in{ori_base_in_handle};
		noexcept_text_ostream_t<char_t>*_tie_with;
		size_t _unread_size=0;//避免重复回显
	public:
		in_impl_t()noexcept:_tie_with(&out_impl){}
		virtual ~in_impl_t()noexcept override=default;

		inheriting_base_stream_operations_from(_in);

		virtual size_t read(char_t*buf,size_t size)noexcept override{
			const size_t ret=_in.read(buf,size);
			if(_tie_with)
				if(ret>_unread_size){
					const size_t tie_size=ret-_unread_size;
					//回显中需要将回车换行、回车转换为换行
					string_view str{buf+size-tie_size,tie_size};
					while(str.size()){
						const auto pos=str.find(ec('\r'));
						if(pos==string_view::npos){
							*_tie_with<<str;
							break;
						}
						else{
							_tie_with->write(str.data(),pos);
							if(pos+1<str.size()&&str[pos+1]==ec('\n'))
								str=str.substr(pos+2);
							else
								str=str.substr(pos+1);
							*_tie_with << ec('\n');
						}
					}
				}
			if(ret<=_unread_size)
				_unread_size-=ret;
			else
				_unread_size=0;
			return ret;
		}
		virtual bool is_end()noexcept override{
			return _in.is_end();
		}
		virtual bool waitting_for_data()noexcept override{
			return _in.waitting_for_data();
		}
		virtual explicit operator bool()noexcept override{
			return _in.operator bool();
		}
		virtual void unread(const char_t*buf,size_t size)noexcept override{
			_in.unread(buf,size);
			_unread_size+=size;
		}
		void tie_with(noexcept_text_ostream_t<char_t>*stream)noexcept{
			_tie_with=stream;
		}
		noexcept_text_ostream_t<char_t>*tie_with()noexcept{
			return _tie_with;
		}
	}in_impl;

	#undef inheriting_base_stream_operations_from_pointer
	#undef inheriting_base_stream_operations_from

	#undef inheriting_istream_operations_from_pointer
	#undef inheriting_istream_operations_from

	#undef inheriting_ostream_operations_from_pointer
	#undef inheriting_ostream_operations_from
}
inline auto&out=base_streams_impl_n::out_impl;
inline auto&err=base_streams_impl_n::err_impl;
inline auto&in=base_streams_impl_n::in_impl;

//file_end

