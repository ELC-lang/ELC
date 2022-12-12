//_body.hpp
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
#if !defined(ELC_APIS_char_set)
	#define ELC_APIS_char_set
	#include "../../header_file/files/elc/base_defs"
	/// @brief 文字编码相关函数
	namespace elc::char_set{
		#include "../../_share/_defs.hpp"

		using namespace ::elc::defs;//string_view_t

		#include "marcos/_defs.hpp"
		#include "constexprs.hpp"
		#include "props_data.hpp"
		#include "convert.hpp"
		#include "isX.hpp"
		#include "marcos/_undefs.hpp"

		#include "../../_share/_undefs.hpp"
	}
#endif

//file_end

