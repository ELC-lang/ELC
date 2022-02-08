// default.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
未完成的elc解释器dev文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include <elc/string>
#include <elc/core>
#include <elc/base_exception>

#include "../../files/elc/_files/_share/_defs.hpp"

namespace elc::defs{
	namespace namespace_n{
		using namespace core;
		inline struct namespace_t{
			ptr _namespace_root;
			weak_ptr _namespace_now;
			namespace_t()noexcept{
				_namespace_root = get<common_node>();
				auto&nmsp	    = *_namespace_root;
				_namespace_now  = &nmsp;
				#if defined(_MSC_VER)
					#pragma warning(push)
					#pragma warning(disable:26447)//may throw警告diss
				#endif
				nmsp[es".."]    = nmsp;
				nmsp[es"."]     = nmsp;
				nmsp[es"t"]     = t;
				nmsp[es"nil"]   = nil;
				nmsp[es"void"]  = the_void;
				#if defined(_MSC_VER)
					#pragma warning(pop)
				#endif
			}
			value get_symbol(const value index){
				return (*_namespace_now)[index];
			}
			value get_symbol(auto&& index){ return get_symbol(as_value(index)); }

			void inter_namespace(const value namespace_name){
				_namespace_now = get_symbol(namespace_name);
			}
			void inter_namespace(auto&& index){ inter_namespace(as_value(index)); }

			void leave_namespace(){ inter_namespace(es".."); }
		}names{};
	}
}
namespace elc{

}

#include "../../files/elc/_files/_share/_undefs.hpp"


#include <iostream>
#include <steve.h>

using namespace elc;

void ste::stst()
{
	
}


