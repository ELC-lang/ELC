//overhead.hpp
//at namespace elc::defs::memory::alloc_n::default_method
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace overhead_n{
	template<class T>
	struct overhead_helper_t{
		constexpr static size_t align=max(alignof(T),alignof(size_t));//保证最高对齐需求
		constexpr static size_t offset_value=(size_t(sizeof(size_t)/align)+bool(sizeof(size_t)%align))*align;//等同于sizeof(size_t)-sizeof(size_t)%align+(sizeof(size_t)%align?align:0)
		//暨，偏移量既要大于sizeof(size_t)以装下overhead，又要是align的整数倍以保证T实例的对齐被满足
		static_assert(offset_value >= sizeof(size_t));
	};
	template<class T>
	constexpr overhead_helper_t<T>overhead_helper{};

	template<class T>
	constexpr size_t correct_size(size_t a){
		return a+overhead_helper<T>.offset_value;
	}
	template<class T>
	constexpr size_t correct_align(type_info_t<T>){
		return overhead_helper<T>.align;
	}
	template<class T>
	inline T*correct_pointer(pointer a)noexcept{
		return reinterpret_cast<T*>(reinterpret_cast<byte*>(a)+overhead_helper<T>.offset_value);
	}
	template<class T>
	inline pointer recorrect_pointer(T*a)noexcept{
		return reinterpret_cast<pointer>(reinterpret_cast<byte*>(a)-overhead_helper<T>.offset_value);
	}
	inline void set_overhead(pointer a,size_t size){
		*reinterpret_cast<size_t*>(a)=size;
	}
	inline size_t get_overhead(pointer a){
		return*reinterpret_cast<size_t*>(a);
	}
}

//file_end

