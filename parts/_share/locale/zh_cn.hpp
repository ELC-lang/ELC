//zh_cn.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/*
locale说明
elc使用的字符串.
调整这些,实现快速本地化.
*/
namespace elc::defs::locale::func{

}
namespace elc::defs::locale::str{
	inline namespace gc{
		auto gc_fail=es"gc失败";
		auto empty_gc_method=es"gc方法为空时gc被调用";
	}
	inline namespace code_convert{
		auto setlocale_error=es"setlocale出错";
		auto code_convert_error=es"编码转换出错";
	}
	inline namespace exception{
		auto unknow_error=es"未知错误";
	}
	inline namespace base_read{
		auto ending_of_expr_beyond_expectations=es"表达式的结尾超出预期";
		auto expr_incomplete=es"表达式未完成";
		auto symbol_is_undefined=es"此符号未定义";
		auto expr_too_outrageous=es"表达式过于离谱";
	}
}

//file_end

