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
	//用以表示某类型的数值范围，仅可表示闭区间
	template<typename T>
	struct range_t{
		T _begin,_end;
		//_begin<=_end
		constexpr range_t(const T begin,const T end):_begin(begin),_end(end){}
		constexpr range_t(note::from_t<const T>begin,note::to_t<const T>end):_begin(begin),_end(end+1){}
		constexpr range_t(note::to_t<const T>end,note::from_t<const T>begin):_begin(begin),_end(end+1){}
		constexpr range_t(const T begin,note::size_t<const T>size):_begin(begin),_end(begin+size()){}
		constexpr range_t(const T begin,note::size_t<size_t>size):_begin(begin),_end(begin+size()){}

		constexpr size_t size()noexcept{return _end-_begin;}
		constexpr auto end()noexcept{return _end;}
		constexpr auto begin()noexcept{return _begin;}
		constexpr auto end()const noexcept{return _end;}
		constexpr auto begin()const noexcept{return _begin;}

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
	//判断一个值是否在某个范围内
	template<typename T>
	[[nodiscard]]constexpr bool in_range(T pattern,const range_t<T>range)noexcept_as(bool(declvalue(T)>=declvalue(const T) && declvalue(T)<=declvalue(const T))){//算术类型或指针
		return bool(pattern>=range.begin() && pattern<range.end());
	}
	//in_range
	//判断一个指针是否在某个byte指针范围内
	template<typename T>
	[[nodiscard]]constexpr bool in_range(T*pattern,const range_t<byte*>range)noexcept{
		return cast_to_data(pattern)>=range.begin() && cast_to_data(pattern)<range.end();
	}
	//match_pattern
	//BMH2改算法表头，用以实施头起始的快速子串匹配算法
	//大部分情况下应当编译时预构建或对运行时生成的实例进行缓存
	template<typename T>
	struct match_pattern{
		array_like_view_t<T>_pattern;
		typedef unsigned char index_type;

		size_t skip_table[number_of_possible_values_per<index_type>]{};
		size_t radical_skip_table[number_of_possible_values_per<index_type>]{};
		index_type pre_index_table[number_of_possible_values_per<index_type>]{};

		static constexpr index_type get_index_of(T&ch)noexcept{
			return index_type(hash(ch) % number_of_possible_values_per<index_type>);
		}
		constexpr void build_table(array_like_view_t<T>pattern)noexcept{
			const size_t m=pattern.size();
			if(!m)
				return;
			for(size_t i=0;i<number_of_possible_values_per<index_type>;i++){
				skip_table[i]=radical_skip_table[i]=m;
			}
			skip_table[get_index_of(pattern[0])]=radical_skip_table[get_index_of(pattern[0])]=m-1;//单独处理pattern[0]的情况
			for(size_t i=1;i<m-1;i++){
				index_type index=get_index_of(pattern[i]);//radical_skip_table[index]表示pattern中倒数第二次出现的index到pattern末尾的距离
				radical_skip_table[index]=skip_table[index];//当index在pattern中出现0次或1次时，radical_skip_table[index]等于模式串长度m
				skip_table[index]=m-i-1;//skip数组的定义与BMH算法相同
				pre_index_table[index]=get_index_of(pattern[i-1]);
			}
		}
		constexpr match_pattern(array_like_view_t<T>pattern)noexcept:_pattern(pattern){
			build_table(_pattern);
		}
		[[nodiscard]]constexpr T* match(array_like_view_t<T>range)const noexcept{
			size_t m = _pattern.size();
			if(!m)
				return range.begin();
			size_t n= range.size();
			size_t i= m-1;
			while(i<n){
				size_t k=i;
				ptrdiff_t j=m-1;//k记录text中每次从右至左开始比较的起始位置
				while((j>=0)&&(_pattern[j]==range[i])){
					i--;j--;
				}
				if(j==-1)
					return addressof(range[i+1]);//在range[i+1]处匹配成功
				index_type index_k=get_index_of(range[k]);
				if(get_index_of(range[k-1])!=pre_index_table[index_k])
					i=k+radical_skip_table[index_k];//采用激进策略移动文本指针
				else
					i=k+skip_table[index_k];
			}
			return nullptr;//匹配失败
		}
	};
	//reverse_match_pattern
	//反向BMH2改算法表头，用以实施尾起始的快速子串匹配算法
	//大部分情况下应当编译时预构建或对运行时生成的实例进行缓存
	template<typename T>
	struct reverse_match_pattern{
		array_like_view_t<T>_pattern;
		typedef unsigned char index_type;

		size_t skip_table[number_of_possible_values_per<index_type>]{};
		size_t radical_skip_table[number_of_possible_values_per<index_type>]{};
		index_type pre_index_table[number_of_possible_values_per<index_type>]{};

		static constexpr index_type get_index_of(T&ch)noexcept{
			return index_type(hash(ch) % number_of_possible_values_per<index_type>);
		}
		constexpr void build_table(array_like_view_t<T>pattern)noexcept{
			const size_t m=pattern.size();
			if(!m)
				return;
			for(size_t i=0;i<number_of_possible_values_per<index_type>;i++){
				skip_table[i]=radical_skip_table[i]=m;
			}
			skip_table[get_index_of(pattern[m-1])]=radical_skip_table[get_index_of(pattern[m-1])]=m-1;//单独处理pattern[m-1]的情况
			for(ptrdiff_t i=m-2;i>=0;i--){
				index_type index=get_index_of(pattern[i]);//radical_skip_table[index]表示pattern中倒数第二次出现的index到pattern末尾的距离
				radical_skip_table[index]=skip_table[index];//当index在pattern中出现0次或1次时，radical_skip_table[index]等于模式串长度m
				skip_table[index]=i;//skip数组的定义与BMH算法相同
				pre_index_table[index]=get_index_of(pattern[i+1]);
			}
		}
		constexpr reverse_match_pattern(array_like_view_t<T>pattern)noexcept:_pattern(pattern){
			build_table(_pattern);
		}
		[[nodiscard]]constexpr T* match(array_like_view_t<T>range)const noexcept{
			size_t m=_pattern.size();
			if(!m)
				return range.begin();
			size_t n= range.size();
			ptrdiff_t i= n-m;
			while(i>=0){
				size_t k=i;
				size_t j=0;//k记录text中每次从右至左开始比较的起始位置
				while((j<=m-1)&&(_pattern[j]==range[i])){
					i++;j++;
				}
				if(j==m)
					return addressof(range[k]);//在range[k]处匹配成功
				index_type index_k=get_index_of(range[k]);
				if(get_index_of(range[k+1])!=pre_index_table[index_k])
					i=k-radical_skip_table[index_k];//采用激进策略移动文本指针
				else
					i=k-skip_table[index_k];
			}
			return nullptr;//匹配失败
		}
	};
	suppress_msvc_warning(26475)//强制转换警告diss
	//npos
	//用以指定不存在的位置
	static constexpr size_t npos = size_t(-1);
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* in_range(T&pattern,array_like_view_t<T>range){
		for(auto&i : range){
			if(i==pattern)
				return addressof(i);
		}
		return nullptr;
	}
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_size_t(T&pattern,array_like_view_t<T>range){
		auto result = in_range(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//数据串匹配by steve02081504.
	//若成功找到匹配的数据串，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* in_range(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if (range.size() >= 512 && pattern.size() >= 11) {
			match_pattern tmp=pattern;
			return tmp.match(range);
		}

		size_t off_set=0;
		size_t matching_off_set=1;

		floop{
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
	//若成功找到匹配的数据串，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = in_range(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(T&pattern,array_like_view_t<T>range){
		for(auto&i : range|::std::views::reverse){
			if(i==pattern)
				return addressof(i);
		}
		return nullptr;
	}
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_but_reverse_size_t(T&pattern,array_like_view_t<T>range){
		auto result = in_range_but_reverse(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//反向数据串匹配by steve02081504.
	//若成功找到匹配的数据串，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if(range.size() >= 512 && pattern.size() >= 11) {
			reverse_match_pattern tmp = pattern;
			return tmp.match(range);
		}

		size_t off_set=0;
		size_t matching_off_set=0;

		floop{
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
	//若成功找到匹配的数据串，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_but_reverse_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = in_range_but_reverse(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}

	//bitmark_for_finds
	//bitmark提供了一种快速进行四种泛搜索的实现，前提是构建中的pattern中的每一项的usigned表示都在一个字节（表长）内。
	//若bitmark构建失败，程序应当使用正在进行的泛搜索的朴素实现版本。
	template<typename T>
	struct bitmark_for_finds {
		typedef unsigned char index_type;
		bool _bitmark[number_of_possible_values_per<index_type>]{};

		bitmark_for_finds()noexcept=default;
		[[nodiscard]]constexpr bool mark(array_like_view_t<T>pattern)noexcept{
			if constexpr(number_of_possible_values_per<T> > number_of_possible_values_per<index_type>){
				for(auto& i: pattern) {
					if(::std::make_unsigned_t<T>(i) >= number_of_possible_values_per<index_type>)
						return false;
					_bitmark[index_type(i)] = true;
				}
			}
			else{
				for(auto& i: pattern)
					_bitmark[index_type(i)] = true;
			}
			return true;
		}
		[[nodiscard]]constexpr bool operator[](T&index)const noexcept{
			if constexpr(number_of_possible_values_per<T> > number_of_possible_values_per<index_type>)
				if(::std::make_unsigned_t<T>(index) >= number_of_possible_values_per<index_type>)
					return false;
			return _bitmark[index_type(index)];
		}
	};
	//find_first_of的bitmark实现
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_of_bitmark(const bitmark_for_finds<T>&mark,array_like_view_t<T>range){
		for(auto&i : range){
			if(mark[i])
				return addressof(i);
		}
		return nullptr;
	}
	//find_first_of的朴素实现
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* base_find_first_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		for(auto&i : range){
			if(in_range(i,pattern))
				return addressof(i);
		}
		return nullptr;
	}
	//find_first_of
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if constexpr(::std::is_integral_v<T>) {
			bitmark_for_finds<T> mark;
			if(mark.mark(pattern))
				return find_first_of_bitmark(mark,range);
		}
		return base_find_first_of(pattern,range);
	}
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_first_of_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = find_first_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//find_last_of的bitmark实现
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of_bitmark(const bitmark_for_finds<T>&mark,array_like_view_t<T>range){
		for(auto& i: range|::std::views::reverse) {
			if(mark[i])
				return addressof(i);
		}
	}
	//find_last_of的朴素实现
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* base_find_last_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		for(auto&i : range|::std::views::reverse){
			if(in_range(i,pattern))
				return addressof(i);
		}
		return nullptr;
	}
	//find_last_of
	//若成功找到匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if constexpr(::std::is_integral_v<T>) {
			bitmark_for_finds<T> mark;
			if(mark.mark(pattern))
				return find_last_of_bitmark(mark,range);
		}
		return base_find_last_of(pattern,range);
	}
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_last_of_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = find_last_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//find_first_not_of的bitmark实现
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of_bitmark(const bitmark_for_finds<T>&mark,array_like_view_t<T>range){
		for(auto& i: range){
			if(!mark[i])
				return addressof(i);
		}
		return nullptr;
	}
	//find_first_not_of的朴素实现
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* base_find_first_not_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		for(auto&i : range){
			if(!in_range(i,pattern))
				return addressof(i);
		}
		return nullptr;
	}
	//find_first_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if constexpr(::std::is_integral_v<T>) {
			bitmark_for_finds<T> mark;
			if(mark.mark(pattern))
				return find_first_not_of_bitmark(mark,range);
		}
		return base_find_first_not_of(pattern,range);
	}
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_first_not_of_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = find_first_not_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
	//find_last_not_of的bitmark实现
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of_bitmark(const bitmark_for_finds<T>&mark,array_like_view_t<T>range){
		for(auto& i: range|::std::views::reverse){
			if(!mark[i])
				return addressof(i);
		}
		return nullptr;
	}
	//find_last_not_of的朴素实现
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* base_find_last_not_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		for(auto&i : range|::std::views::reverse){
			if(!in_range(i,pattern))
				return addressof(i);
		}
		return nullptr;
	}
	//find_last_not_of
	//若成功找到不匹配的数据项，返回其开头，若未找到，返回nullptr
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of(array_like_view_t<T>pattern,array_like_view_t<T>range){
		if constexpr(::std::is_integral_v<T>) {
			bitmark_for_finds<T> mark;
			if(mark.mark(pattern))
				return find_last_not_of_bitmark(mark,range);
		}
		return base_find_last_not_of(pattern,range);
	}
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_last_not_of_size_t(array_like_view_t<T>pattern,array_like_view_t<T>range){
		auto result = find_last_not_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
}
using range_n::range_t;
using range_n::in_range;
using range_n::in_range_size_t;
using range_n::in_range_but_reverse;
using range_n::in_range_but_reverse_size_t;
using range_n::find_first_of;
using range_n::find_last_of;
using range_n::find_first_not_of;
using range_n::find_last_not_of;

//file_end

