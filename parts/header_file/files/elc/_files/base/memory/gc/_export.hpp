//_export.hpp
//at namespace elc::defs::memory::gc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
BREAK_NAMESPACE//in namespace elc::defs
INTER_NAMESPACE(memory)
#define export using gc_n::
export gc;
export have_root;
export root_of;
export mark_able_for_gc;
export can_shrink;
#undef export
BREAK_NAMESPACE//in namespace elc::defs
INTER_NAMESPACE(memory::gc_n)

//file_end

