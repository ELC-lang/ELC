//die.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
APIs说明
elc依赖的基础函数.
调整这些,实现快速移植,快速优化与行为自定义.
*/
#if !defined(ELC_APIS_die)
	#define ELC_APIS_die
	#include "../default_data_type.hpp"
	namespace elc::APIs::die{
		[[noreturn]]void die_with([[maybe_unused]]const char_t*err_msg){
			//UF
			::std::exit(1);
		}
	}
	namespace elc::defs{
		using namespace elc::APIs::die;
	}
#endif

//file_end

