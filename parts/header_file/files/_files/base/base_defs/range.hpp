//range.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace range_n{
	//range_t
	template<typename T>
	struct range_t{
		T _begin,_end;
		//_begin<=_end
		constexpr range_t(const T begin,const T end):_begin(begin),_end(end+1){}
		constexpr range_t(note::from_t<const T>begin,note::to_t<const T>end):_begin(begin),_end(end+1){}
		constexpr range_t(note::to_t<const T>end,note::from_t<const T>begin):_begin(begin),_end(end+1){}
		constexpr range_t(const T begin,note::size_t<const T>size):_begin(begin),_end(begin+size()){}
		constexpr range_t(const T begin,note::size_t<::std::size_t>size):_begin(begin),_end(begin+size()){}

		constexpr auto size()noexcept{return _end-_begin;}
		constexpr auto end()noexcept{return _end;}
		constexpr auto begin()noexcept{return _begin;}
	};

	template<class T>
	inline auto size_of_array_like(range_t<T*>&a)noexcept{return a.size();}
	template<class T>
	inline auto size_of_array_like(range_t<const T*>&a)noexcept{return a.size();}

	template<class T>
	inline auto begin_of_array_like(range_t<T*>&a)noexcept{return a.begin();}
	template<class T>
	inline auto begin_of_array_like(range_t<const T*>&a)noexcept{return a.begin();}

	//in_range
	template<typename T>
	constexpr bool in_range(const T a,const range_t<T>range){//算术类型或指针
		return a>=range.begin() && a<range.end();
	}
	template<typename T>
	constexpr bool in_range(const T*a,const range_t<void*>range){
		return reinterpret_cast<void*>(a)>=range.begin() && reinterpret_cast<void*>(a)<range.end();
	}
	template<typename T,typename T_,enable_if(is_array_like_for<T,T_>)>
	constexpr T* in_range(const T&a,T_&&range){
		auto tmp=begin_of_array_like(range);
		auto end=end_of_array_like(range);
		while(tmp!=end){
			if(*tmp==a)
				return tmp;
			tmp++;
		}
		return nullptr;
	}
	template<typename T,typename T_,typename T__,enable_if(is_array_like_for<T,T_>&&is_array_like_for<T,T__>)>
	constexpr T* in_range(T_&&a,T__&&range){
		//数据串匹配by steve02081504.
		//若成功找到匹配的数据串，返回其开头，若未找到，返回nullptr
		size_t off_set=0;
		size_t matching_off_set=1;

		auto a_begin=begin_of_array_like(a);
		auto a_size=size_of_array_like(a);
		auto a_end=end_of_array_like(a);
		auto range_begin=begin_of_array_like(range);
		auto range_size=size_of_array_like(range);
		while(true){
			matching_off_set=1;
			while(a_end[-matching_off_set]==range_begin[off_set+a_size-matching_off_set])
				if(matching_off_set==a_size)
					return addressof(range_begin[off_set+a_size-matching_off_set]);
				else
					matching_off_set++;
			auto tmp=in_range(range_begin[off_set+a_size],a);
			if(!tmp)
				tmp=a_begin;
			off_set+=a_end-tmp;
			if(off_set + a_size >= range_size)
				return nullptr;
		}
	}
}
using range_n::range_t;
using range_n::in_range;

//file_end

