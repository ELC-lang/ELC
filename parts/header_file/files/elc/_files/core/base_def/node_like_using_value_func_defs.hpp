//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:26440)//nothrow警告diss
#endif
[[nodiscard]]inline value node_like::be_eval(){
	return value(this);
}
[[nodiscard]]inline value node_like::be_call(ptr){
	return value(this);
}
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

[[nodiscard]]inline value node_like::operator[](const value index){
	return arec(index);
}

//file_end

