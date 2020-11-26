//lib_loader.hpp
//at namespace elc
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace lib_loader_n{
	struct library_info_t:ref_able<library_info_t>{
	private:
		library_handle _m;
		string _name;
		string _error;
	public:
		explicit library_info_t(string lib_name)noexcept{
			_name=move(name);
			_m=load_library(name);

			if(_m)
				error=get_load_error();
		}
		~library_info_t()noexcept{
			free_library(_m);
		}
		const string&get_error()noexcept{
			return _error;
		}
		const string&get_name()noexcept{
			return _name;
		}
		template<class function_t>
		function_t<function_t>get_symbol_as_function(){
			
		}
	};

	typedef comn_ptr_t<library_info_t> library_handle_t;
}

//file_end

