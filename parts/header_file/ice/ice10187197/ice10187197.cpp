// ice10187197.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "../../files/elc/lib_loader"

int main(size_t argc,char**_argv) {
	using namespace elc;
	using namespace elc::defs;
	//build argv
	array_t<string> argv(note::size(argc));
	for(size_t i = 0; i < argc; ++i) {
		argv[i] = APIs::str_code_convert::to_char_t_str(_argv[i]);
	}
	
	function_t<void()noexcept> main_symbol=nullptr;
	{
		//load lib file at argv[1] and call "main" function
		auto exe = load_lib(argv[1]);
		main_symbol=exe->get_symbol_as_function<void()noexcept>(U"main");
		//exe->
	}
	//we can now throw the lib file away as it will be auto unloaded at the end of symbol's life time
	main_symbol();
}
