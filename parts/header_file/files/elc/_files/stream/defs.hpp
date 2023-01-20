//defs.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace base_fstream_defs{
	using namespace elc::APIs::streams;
	typedef handle_type handle_t;

	struct base_frefstream:virtual noexcept_stream_t,instance_struct<base_frefstream>{
	protected:
		handle_t _file;
	public:
		explicit base_frefstream(handle_t file)noexcept:_file(file){}
		virtual int_t seek(seek_type dir,int_t off)noexcept override{
			return basic_seek_impl(_file,off,dir);
		}
		virtual int_t tell()noexcept override{
			return basic_tell_impl(_file);
		}
		virtual void seek_to(int_t pos)noexcept override{
			basic_seek_impl(_file,pos,seek_type::beg);
		}
		virtual void sync()noexcept override{
			basic_sync_impl(_file);
		}
		virtual void flush()noexcept override{
			basic_flush_impl(_file);
		}
		virtual void close()noexcept override{
			basic_close_impl(_file);
		}
		//fried get_handle_from
		friend handle_t get_handle_from(base_frefstream&stream)noexcept{
			return stream._file;
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
				base_t::close();
		}
	};
	template<class char_T,class base_fstream_T>
	struct base_text_ifstream_wrapper:base_fstream_T,virtual noexcept_text_istream_t<char_T>,instance_struct<base_text_ifstream_wrapper<char_T,base_fstream_T>>{
	private:
		typedef base_fstream_T base_t;
		typedef base_text_ifstream_wrapper<char_T,base_fstream_T> this_t;
		override_instance_struct;
	public:
		using base_t::base_t;

		virtual size_t read(char_T*buf,size_t size)noexcept override{
			return basic_read_impl(base_t::_file,buf,size);
		}
	};
	template<class char_T,class base_fstream_T>
	struct base_text_ofstream_wrapper:base_fstream_T,virtual noexcept_text_ostream_t<char_T>,instance_struct<base_text_ofstream_wrapper<char_T,base_fstream_T>>{
	private:
		typedef base_fstream_T base_t;
		typedef base_text_ofstream_wrapper<char_T,base_fstream_T> this_t;
		override_instance_struct;
	public:
		using base_t::base_t;

		virtual void write(const char_T*buf,size_t size)noexcept override{
			basic_write_impl(base_t::_file,buf,size);
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

	distinctive inline struct output_stream_impl_t:base_text_orefstream<char_t>,instance_struct<output_stream_impl_t>{
	private:
		typedef base_text_orefstream<char_t> base_t;
		typedef output_stream_impl_t this_t;
		override_instance_struct;
	public:
		output_stream_impl_t()noexcept:base_t(init_output_stream()){}
		virtual void write(const char_t*buf,size_t size)noexcept override{
			APIs::streams::write_text_to_terminal_stream(base_t::_file, buf, size);
		}
	}out_impl{};
	distinctive inline struct error_stream_impl_t:base_text_orefstream<char_t>,instance_struct<error_stream_impl_t>{
	private:
		typedef base_text_orefstream<char_t> base_t;
		typedef error_stream_impl_t this_t;
		override_instance_struct;
	public:
		error_stream_impl_t()noexcept:base_t(init_error_stream()){}
		virtual void write(const char_t*buf,size_t size)noexcept override{
			APIs::streams::write_text_to_terminal_stream(base_t::_file, buf, size);
		}
	}err_impl{};
	struct input_stream_iterface_t:virtual noexcept_text_istream_t<char_t>{
		virtual void echo_to(base_frefstream&)noexcept=0;
		virtual void tie_with(base_frefstream&)noexcept=0;
		virtual size_t no_echo_read(char_t*buf,size_t size)noexcept=0;
	};
	distinctive inline struct input_stream_impl_t:input_stream_iterface_t,base_text_irefstream<char_t>,instance_struct<input_stream_impl_t>{
	private:
		typedef base_text_irefstream<char_t> base_t;
		typedef input_stream_impl_t this_t;
		override_instance_struct;

		handle_t _echo_to,_tie_with;
	public:
		virtual void echo_to(base_frefstream&stream)noexcept override{
			_echo_to=get_handle_from(stream);
		}
		virtual void tie_with(base_frefstream&stream)noexcept override{
			_tie_with=get_handle_from(stream);
		}
		input_stream_impl_t()noexcept:base_t(init_input_stream()){
			echo_to(out_impl);
			tie_with(out_impl);
		}
		virtual size_t read(char_t*buf,size_t size)noexcept override{
			basic_flush_impl(_tie_with);
			return APIs::streams::read_text_from_terminal_stream(base_t::_file, buf, size, _echo_to);
		}
		virtual size_t no_echo_read(char_t*buf,size_t size)noexcept override{
			return APIs::streams::read_text_from_terminal_stream(base_t::_file, buf, size, nullptr);
		}
	}in_impl{};
}
inline auto&out=base_fstream_defs::out_impl;
inline auto&err=base_fstream_defs::err_impl;
inline auto&in=base_fstream_defs::in_impl;

//file_end

