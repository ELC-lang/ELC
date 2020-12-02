//lib_loader.hpp
//at namespace elc
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace lib_loader_n{
	//UF:抽象等级太低
	struct library_info_t;
	typedef comn_ptr_t<library_info_t> library_handle_t;


	struct library_info_t:ref_able<library_info_t>{
	private:
		library_handle _m;
		string _name;
		string _error;
	public:
		explicit library_info_t(string lib_name)noexcept{
			_name=move(lib_name);
			_m=load_library(lib_name);

			if(_m)
				error=get_load_error();
		}
		~library_info_t()noexcept{
			free_library(_m);
		}
		[[nodiscard]]explicit operator bool()noexcept{
			return bool(_m);
		}
		[[nodiscard]]const string&get_error()noexcept{
			return _error;
		}
		[[nodiscard]]const string&get_name()noexcept{
			return _name;
		}

		[[nodiscard]]void*get_symbol(string symbol_name)noexcept{
			return get_symbol(_m,symbol_name);
		}
		template<class symbol_t>
		[[nodiscard]]symbol_t&get_symbol_as(string symbol_name)noexcept{
			return*reinterpret_cast<symbol_t*>(get_symbol(symbol_name));
		}
		template<class Func_t>
		[[nodiscard]]function_t<Func_t>get_symbol_as_function(string symbol_name)noexcept{
			auto func_p=&get_symbol_as<Func_t>(symbol_name);

			//出错检查：返回default的function_t（可正常调用）
			if(!func_p)return nullptr;

			/*
			call_t
			继承function_data_warpper_t的operator()以可转换为function_t
			容纳一个library_handle_t以引用计数lib资源
			*/
			struct call_t:function_data_warpper_t<Func_t>{
				library_handle_t;
			} f =
			{func_p,this};

			return static_cast<function_t<Func_t>>(f);
		}
	};
}

//file_end

