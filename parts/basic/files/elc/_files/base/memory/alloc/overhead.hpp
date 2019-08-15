//overhead.hpp
//at namespace elc::memory::alloc_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace overhead_n{
	constexpr size_t correct_size(size_t a){
		return a+sizeof(size_t);
	}
	constexpr size_t correct_align(size_t a){
		return max(a,alignof(size_t));
	}
	inline pointer correct_pointer(pointer a)noexcept{
		return reinterpret_cast<size_t*>(a)+1;
	}
	inline pointer recorrect_pointer(pointer a)noexcept{
		return reinterpret_cast<size_t*>(a)-1;
	}
	inline void set_overhead(pointer a,size_t size){
		*reinterpret_cast<size_t*>(a)=size;
	}
	inline size_t get_overhead(pointer a){
		return*reinterpret_cast<size_t*>(a);
	}
}
