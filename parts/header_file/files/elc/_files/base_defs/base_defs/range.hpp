//range.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
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
		constexpr range_t(const T begin,const T end):_begin(begin),_end(end){}
		constexpr range_t(note::from_t<const T>begin,note::to_t<const T>end):_begin(begin),_end(end+1){}
		constexpr range_t(note::to_t<const T>end,note::from_t<const T>begin):_begin(begin),_end(end+1){}
		constexpr range_t(const T begin,note::size_t<const T>size):_begin(begin),_end(begin+size()){}
		constexpr range_t(const T begin,note::size_t<::std::size_t>size):_begin(begin),_end(begin+size()){}

		constexpr size_t size()noexcept{return _end-_begin;}
		constexpr auto end()noexcept{return _end;}
		constexpr auto begin()noexcept{return _begin;}
		constexpr auto end()const noexcept{ return _end; }
		constexpr auto begin()const noexcept{ return _begin; }

		template <class U> requires(type_info<T>.can_convert_to<U>)
		constexpr operator range_t<U>()const noexcept(type_info<T>.can_nothrow_convert_to<U>){
			return {_begin,_end};
		}
	};

	template<class T>
	[[nodiscard]]inline auto size_of_array_like(range_t<T*>&a)noexcept{return a.size();}
	template<class T>
	[[nodiscard]]inline auto size_of_array_like(range_t<const T*>&a)noexcept{return a.size();}

	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(range_t<T*>&a)noexcept{return a.begin();}
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(range_t<const T*>&a)noexcept{return a.begin();}

	//in_range
	template<typename T>
	[[nodiscard]]constexpr bool in_range(T pattern,const range_t<T>range){//算术类型或指针
		return pattern>=range.begin() && pattern<range.end();
	}
	template<typename T>
	[[nodiscard]]constexpr bool in_range(T*pattern,const range_t<byte*>range){
		return reinterpret_cast<const void*>(pattern)>=range.begin() && reinterpret_cast<const void*>(pattern)<range.end();
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range(T&pattern,array_like_view_t<T>range){
		for(auto&i : range){
			if(i==pattern)
				return addressof(i);
		}
		return nullptr;
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range(array_like_view_t<T>pattern,array_like_view_t<T>range){
		//数据串匹配by steve02081504.
		//若成功找到匹配的数据串，返回其开头，若未找到，返回nullptr
		size_t off_set=0;
		size_t matching_off_set=1;

		while(true){
			matching_off_set=1;
			while(pattern.end()[0-matching_off_set]==range.begin()[off_set+pattern.size()-matching_off_set])
				if(matching_off_set==pattern.size())
					return addressof(range.begin()[off_set]);
				else
					matching_off_set++;
			if(off_set+pattern.size() >= range.size())
				return nullptr;
			auto tmp = in_range(range.begin()[off_set+pattern.size()], pattern);
			if(!tmp)
				tmp=pattern.begin();
			off_set+=pattern.end()-tmp;
		}
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(T&pattern,array_like_view_t<T>range){
		for(auto&i : range|::std::views::reverse){
			if(i==pattern)
				return addressof(i);
		}
		return nullptr;
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(array_like_view_t<T>pattern,array_like_view_t<T>range){
		//反向数据串匹配by steve02081504.
		//若成功找到匹配的数据串，返回其开头，若未找到，返回nullptr
		size_t off_set=0;
		size_t matching_off_set=0;

		while(true){
			matching_off_set=0;
			while(pattern.begin()[matching_off_set]==range.end()[0-(off_set+pattern.size()-matching_off_set)])
				if(matching_off_set==pattern.size()-1)
					return addressof(range.end()[0-(off_set+pattern.size())]);
				else
					matching_off_set++;
			if(off_set+pattern.size() >= range.size())
				return nullptr;
			auto tmp = in_range_but_reverse(range.end()[0-(off_set+pattern.size()+1)], pattern);
			if(!tmp)
				tmp=pattern.end()-1;
			off_set+=tmp-pattern.begin();
			off_set+=1;
		}
	}
	template<typename T>
	struct match_pattern{
		array_like_view_t<T>_pattern;
		typedef unsigned char index_type;

		size_t skip_table[number_of_possible_values_per<index_type>];
		size_t radical_skip_table[number_of_possible_values_per<index_type>];
		index_type pre_index_table[number_of_possible_values_per<index_type>];

		index_type get_index_of(T&ch)noexcept{
			return index_type(hash(ch) % number_of_possible_values_per<index_type>);
		}
		void build_table(array_like_view_t<T>pattern)noexcept{
			size_t m=pattern.size();
			for(size_t i=0;i<number_of_possible_values_per<index_type>;i++){
				skip_table[i]=radical_skip_table[i]=m;
			}
			skip_table[pattern[0]]=radical_skip_table[pattern[0]]=m-1;//单独处理pattern[0]的情况
			for(size_t i=1;i<m-1;i++){
				index_type index=get_index_of(pattern[i]);//radical_skip_table[index]表示pattern中倒数第二次出现的index到pattern末尾的距离
				radical_skip_table[index]=skip_table[index];//当index在pattern中出现0次或1次时，radical_skip_table[index]等于模式串长度m
				skip_table[index]=m-i-1;//skip数组的定义与BMH算法相同
				pre_index_table[index]=get_index_of(pattern[i-1]);
			}
		}
		match_pattern(array_like_view_t<T>pattern)noexcept:_pattern(pattern){
			build_table(_pattern);
		}
		[[nodiscard]]constexpr T* match(array_like_view_t<T>range)noexcept{
			size_t m=_pattern.size();
			size_t n= range.size();
			size_t i= m-1;
			while(i<n){
				size_t k=i;
				ptrdiff_t j=m-1;//k记录text中每次从右至左开始比较的起始位置
				while((j>=0)&&(_pattern[j]==range[i])){
					i--;j--;
				}
				if(j==-1)
					return addressof(range[i+1]);//在text[i+1]处匹配成功
				index_type index_k=get_index_of(range[k]);
				if(get_index_of(range[k-1])!=pre_index_table[index_k])
					i=k+radical_skip_table[index_k];//采用激进策略移动文本指针
				else
					i=k+skip_table[index_k];
			}
			return nullptr;//匹配失败
		}
	};
}
using range_n::range_t;
using range_n::in_range;
using range_n::in_range_but_reverse;

//file_end

