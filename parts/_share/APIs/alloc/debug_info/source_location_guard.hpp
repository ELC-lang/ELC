//source_location_guard.hpp
//at namespace elc::APIs::alloc
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_DEBUG)
	distinctive inline ::std::stacktrace_entry operate_source_frame;
	distinctive inline struct source_location_info_t{
		const char*_file=nullptr;
		uint_least32_t _line=0;
		const char*file()noexcept{
			if(!_file){
				try{
					static ::std::string file_cache;
					file_cache=operate_source_frame.source_file();
					_file=file_cache.c_str();
				}
				catch(...){
				}
			}
			return _file;
		}
		uint_least32_t line()noexcept{
			if(!_line){
				try{
					_line=operate_source_frame.source_line();
				}
				catch(...){
				}
			}
			return _line;
		}
		void clear()noexcept{
			_file=nullptr;
			_line=0;
		}
	}operate_source_location;
	struct source_location_guard{
		bool is_set=false;
		source_location_guard(size_t lookup=0)noexcept{
			if(!operate_source_frame) {
				::std::stacktrace stack = ::std::stacktrace::current(/*skip*/lookup+2,/*max_depth*/1);
				if(stack.size()){
					operate_source_frame = stack[0];
					is_set				 = true;
				}
			}
		}
		~source_location_guard()noexcept{
			if(is_set){
				operate_source_frame = ::std::stacktrace_entry{};
				operate_source_location.clear();
			}
		}
	};
#else
	struct source_location_guard{
		source_location_guard(size_t lookup=0)noexcept{}
	};
#endif

//file_end

