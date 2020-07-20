//_body.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
class gc_able{
	inline struct method_marker_t{
		method_marker_t()noexcept{
			gc.add_gc_method(type_info<T>);
		}
	}method_marker;
};
//?
