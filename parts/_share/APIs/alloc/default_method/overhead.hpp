//overhead.hpp
//at namespace elc::defs::memory::alloc_n::default_method
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace overhead_n{
	using namespace elc::defs;
	constexpr size_t overhead_get_align(size_t align){
		return max(align,alignof(size_t));//保证最高对齐需求
	}
	constexpr size_t overhead_get_offset_value(size_t align){
		align=overhead_get_align(align);
		return (size_t(sizeof(size_t)/align)+bool(sizeof(size_t)%align))*align;//等同于sizeof(size_t)-sizeof(size_t)%align+(sizeof(size_t)%align?align:0)
		//暨，偏移量既要大于sizeof(size_t)以装下overhead，又要是align的整数倍以保证T实例的对齐被满足
	}
	constexpr size_t correct_size(size_t size,size_t align){
		return size+overhead_get_offset_value(align);
	}
	constexpr size_t correct_align(size_t align){
		return overhead_get_align(align);
	}
	inline pointer correct_pointer(pointer a,size_t align){
		return apply_off_set(a,overhead_get_offset_value(align));
	}
	inline pointer recorrect_pointer(pointer a,size_t align){
		return unapply_off_set(a,overhead_get_offset_value(align));
	}
	inline const_pointer recorrect_pointer(const_pointer a, size_t align) {
		return unapply_off_set(a,overhead_get_offset_value(align));
	}
	inline void set_overhead(pointer a,size_t size){
		*reinterpret_cast<size_t*>(a)=size;
	}
	inline size_t get_overhead(const_pointer a) {
		return*reinterpret_cast<const size_t*>(a);
	}
}

//file_end

