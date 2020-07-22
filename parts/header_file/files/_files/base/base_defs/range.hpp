//range.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace range_n{
	template<typename T>
	struct range_t{
		T _begin,_end;
		//_begin<=_end
		constexpr range_t(const T begin,const T end):_begin(begin),_end(end){}
		constexpr range_t(note::from_t<const T>begin,note::to_t<const T>end):_begin(begin),_end(end){}
		constexpr range_t(note::to_t<const T>end,note::from_t<const T>begin):_begin(begin),_end(end){}
		constexpr range_t(const T begin,note::size_t<const T>size):_begin(begin),_end(begin+size()){}
		constexpr range_t(const T begin,note::size_t<::std::size_t>size):_begin(begin),_end(begin+size()){}
	};
	template<typename T>
	constexpr bool in_range(const T a,const range_t<T>range){
		return a>=range._begin && a<=range._end;
	}
	template<typename T>
	constexpr bool in_range(const T*a,const range_t<void*>range){
		return reinterpret_cast<void*>(a)>=range._begin && reinterpret_cast<void*>(a)<=range._end;
	}
}
using range_n::in_range;

//file_end

