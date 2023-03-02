//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器stream文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace base_fstream_n{
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
		virtual bool seek_to(int_t pos)noexcept override{
			return basic_seek_impl(_file,pos,seek_type::beg)==pos;
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
	struct base_not_unreadable_text_ifstream_wrapper:base_fstream_T,virtual noexcept_not_unreadable_text_istream_t<char_T>,instance_struct<base_not_unreadable_text_ifstream_wrapper<char_T,base_fstream_T>>{
	private:
		typedef base_fstream_T base_t;
		typedef base_not_unreadable_text_ifstream_wrapper<char_T,base_fstream_T> this_t;
		override_instance_struct;
	public:
		using base_t::base_t;
		typedef char_T char_type;

		virtual size_t read(char_T*buf,size_t size)noexcept override{
			return basic_read_impl(base_t::_file,buf,size);
		}
		virtual bool is_end()noexcept override{
			return is_eof(base_t::_file);
		}
		virtual bool waitting_for_data()noexcept override{
			return is_wait_input(base_t::_file);
		}
	};
	template<class char_T,class base_fstream_T>
	using base_text_ifstream_wrapper=unreadable_wrap<base_not_unreadable_text_ifstream_wrapper<char_T,base_fstream_T>>;
	template<class char_T,class base_fstream_T>
	struct base_text_ofstream_wrapper:base_fstream_T,virtual noexcept_text_ostream_t<char_T>,instance_struct<base_text_ofstream_wrapper<char_T,base_fstream_T>>{
	private:
		typedef base_fstream_T base_t;
		typedef base_text_ofstream_wrapper<char_T,base_fstream_T> this_t;
		override_instance_struct;

		using base_t::_file;
	public:
		using base_t::base_t;

		virtual void write(const char_T*buf,size_t size)noexcept override{
			auto var=before_terminal_write(_file);
			basic_write_impl(_file,buf,size);
			after_terminal_write(_file,var);
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

	struct system_terminal_irefstream:base_text_irefstream<base_io_char_type>,instance_struct<system_terminal_irefstream>{
	private:
		typedef base_text_irefstream<base_io_char_type> base_t;
		typedef system_terminal_irefstream this_t;
		override_instance_struct;

		using base_t::_file;
	protected:
		virtual size_t base_read_impl_for_wrapper(base_io_char_type*buf,size_t size)noexcept override{
			auto var=before_terminal_read(_file);
			const auto result=basic_read_for_terminal_impl(_file,buf,size);
			after_terminal_read(_file,var);
			return result;
		}
	public:
		using base_t::base_t;
	};
	struct system_terminal_orefstream:base_text_orefstream<base_io_char_type>,instance_struct<system_terminal_orefstream>{
	private:
		typedef base_text_orefstream<base_io_char_type> base_t;
		typedef system_terminal_orefstream this_t;
		override_instance_struct;
	protected:
		//ofstream没有缓冲区，所以覆写原有的write
		virtual void write(const base_io_char_type*buf,size_t size)noexcept override{
			basic_write_for_terminal_impl(base_t::_file,buf,size);
		}
	public:
		using base_t::base_t;
	};
}

//file_end

