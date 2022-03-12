//lib_loader.hpp
//at namespace elc
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace lib_loader_n{
	using namespace elc::APIs::library_load;

	struct library_info_t;
	typedef comn_ptr_t<library_info_t> library_handle_t;

	struct library_info_t:type_info_t<library_info_t>::template_name
		with_common_attribute<ref_able,never_in_array>,build_by_get_only{
	private:
		library_handle _m;
		string _name;
		string _error;
	public:
		constexpr explicit library_info_t(special_init_t)noexcept{
			_m=library_handle{0};
			_error=locale::str::lib_load::null_lib;
			attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero();
		}
		explicit library_info_t(string lib_name)noexcept{
			_name=lib_name;
			_m=load_library(lib_name);

			if(!_m)
				_error=get_load_error();
		}
		~library_info_t()noexcept{
			free_library(_m);
		}
		[[nodiscard]]explicit operator bool()const noexcept{
			return bool(_m);
		}
		[[nodiscard]]string get_error()noexcept{
			return _error;
		}
		[[nodiscard]]string get_name()noexcept{
			return _name;
		}

		[[nodiscard]]void*get_symbol(string symbol_name)noexcept{
			return APIs::library_load::get_symbol(_m,symbol_name);
		}
		template<class symbol_t>
		[[nodiscard]]symbol_t&get_symbol_as(string symbol_name)noexcept{
			return*reinterpret_cast<symbol_t*>(get_symbol(symbol_name));
		}
		template<class Func_t>
		[[nodiscard]]function_t<Func_t>get_symbol_as_function(string symbol_name)noexcept;
	};
	[[nodiscard]]inline bool the_pointer_to_bool(const library_info_t*a)noexcept{
		return a->operator bool();
	}
	distinctive inline library_info_t null_lib{special_init};
	[[nodiscard]]inline constexpr library_info_t*the_get_null_ptr(const library_info_t*)noexcept{return&null_lib;}

	/*
	base_symbol_t
	容纳一个library_handle_t以引用计数lib资源
	同时容纳自身的符号名以方便反向查找名称
	*/
	struct base_symbol_t{
		library_handle_t _lib;
		string _name;
	};
	/*
	symbol_t
	继承function_data_warpper_t的operator()以可转换为function_t
	继承base_symbol_t以对lib的引用计数和包含更多相关信息
	*/
	template<class Func_t>
	struct symbol_t;
	#if !defined(_MSC_VER)
	template<class Ret_t,class...Args_t,bool nothrow>
	struct symbol_t<Ret_t(Args_t...)noexcept(nothrow)>:function_data_warpper_t<Ret_t(*)(Args_t...)noexcept(nothrow)>,base_symbol_t{
		symbol_t(library_handle_t lib,string symbol_name,Ret_t(*self)(Args_t...)noexcept(nothrow))noexcept:
		function_data_warpper_t<Ret_t(*)(Args_t...)noexcept(nothrow)>(self),base_symbol_t(lib,symbol_name){}
	};
	#else
	//MSVC，我滴垃圾堆
	template<class Ret_t,class...Args_t>
	struct symbol_t<Ret_t(Args_t...)>:function_data_warpper_t<Ret_t(*)(Args_t...)>,base_symbol_t{
		symbol_t(library_handle_t lib,string symbol_name,Ret_t(*self)(Args_t...))noexcept:
		function_data_warpper_t<Ret_t(*)(Args_t...)>(self),base_symbol_t(lib,symbol_name){}
	};
	template<class Ret_t,class...Args_t>
	struct symbol_t<Ret_t(Args_t...)noexcept>:function_data_warpper_t<Ret_t(*)(Args_t...)noexcept>,base_symbol_t{
		symbol_t(library_handle_t lib,string symbol_name,Ret_t(*self)(Args_t...)noexcept)noexcept:
		function_data_warpper_t<Ret_t(*)(Args_t...)noexcept>(self),base_symbol_t(lib,symbol_name){}
	};
	#endif

	template<class Func_t>
	[[nodiscard]]inline function_t<Func_t>library_info_t::get_symbol_as_function(string symbol_name)noexcept{
		auto func_p=&get_symbol_as<Func_t>(symbol_name);
		if(!func_p)
			return null_ptr;
		else
			return symbol_t<Func_t>(this,symbol_name,func_p);
	}

	[[nodiscard]]inline library_handle_t load_lib(string lib_name)noexcept{
		return get<library_info_t>(lib_name);
	}
	template<class Func_t>
	[[nodiscard]]inline library_handle_t get_library_handle_form_symbol(function_t<Func_t> symbol)noexcept{
		auto symbol_info=symbol.get_as<base_symbol_t>();
		if(symbol_info.fail())
			return null_ptr;
		else
			return symbol_info.get_ref()._lib;
	}
	template<class Func_t>
	[[nodiscard]]inline string get_symbol_name_form_symbol(function_t<Func_t> symbol)noexcept{
		auto symbol_info=symbol.get_as<base_symbol_t>();
		if(symbol_info.fail())
			return {};
		else
			return symbol_info.get_ref()._name;
	}
}

//file_end

