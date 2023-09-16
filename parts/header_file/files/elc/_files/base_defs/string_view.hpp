//string_view.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
//仅是声明
namespace constexpr_str_n{
	template<typename char_T>
	struct constexpr_str_t;
}
using constexpr_str_n::constexpr_str_t;
//重载range操作
//仅是声明以及size_t包装
namespace range_n{
	template<typename T>
	[[nodiscard]]constexpr T* in_range(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = in_range(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* in_range_but_reverse(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t in_range_but_reverse_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = in_range_but_reverse(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_first_of(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_first_of_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = find_first_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_last_of(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_last_of_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = find_last_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_first_not_of(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr size_t find_first_not_of_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = find_first_not_of(pattern, range);
		if(result)
			return result - range.data();
		else
			return npos;
	}
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range);
	//若成功找到不匹配的数据项，返回其距离开头的步数，若未找到，返回npos
	template<typename T>
	[[nodiscard]]constexpr T* find_last_not_of_size_t(const constexpr_str_t<T>&pattern,const array_like_view_t<T>&range){
		auto result = find_last_not_of(pattern,range);
		if(result)
			return result - range.begin();
		else
			return npos;
	}
}
//开始定义
namespace string_view_n{
	/// array_like_view_t的特化类型，表明这是字符串
	template<typename char_T>
	struct string_view_t:array_like_view_t<const char_T>{
		static constexpr size_t npos=bit::npos;
		typedef array_like_view_t<const char_T> base_t;
		typedef char_T*string_ptr_t;
		typedef const char_T*const_string_ptr_t;
		//构造函数
		using base_t::base_t;
		constexpr string_view_t(const_string_ptr_t str)noexcept:string_view_t(array_end_by_zero_t<const char_T>(str)){}
		//成员函数
		[[nodiscard]]constexpr const char_T*str()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr const char_T*data()const noexcept{return base_t::cbegin();}
		[[nodiscard]]constexpr operator const char_T*()const noexcept{return str();}
		//substr
		[[nodiscard]]constexpr string_view_t substr(size_t pos,size_t len)const noexcept{
			return string_view_t(base_t::cbegin()+pos,len);
		}
		[[nodiscard]]constexpr string_view_t substr(size_t pos)const noexcept{
			return string_view_t(base_t::cbegin()+pos,base_t::size()-pos);
		}
		[[nodiscard]]constexpr string_view_t substr(size_t pos,const char_T*end)const noexcept{
			return string_view_t(base_t::cbegin()+pos,end-base_t::cbegin());
		}
		[[nodiscard]]constexpr string_view_t substr(const char_T*begin,size_t len)const noexcept{
			return string_view_t(begin,len);
		}
		[[nodiscard]]constexpr string_view_t substr(const char_T*begin)const noexcept{
			return string_view_t(begin,base_t::cend()-begin);
		}
		[[nodiscard]]constexpr string_view_t substr(const char_T*begin,const char_T*end)const noexcept{
			return string_view_t(begin,end-begin);
		}
		//starts_with
		[[nodiscard]]constexpr bool starts_with(const char_T ch)const noexcept{
			return base_t::size()&&base_t::cbegin()[0]==ch;
		}
		[[nodiscard]]constexpr bool starts_with(string_view_t str)const noexcept{
			if(str.size()>base_t::size())
				return false;
			return equal(base_t::cbegin(),str.cbegin(),str.size());
		}
		[[nodiscard]]constexpr bool starts_with(const char_T*str)const noexcept{
			size_t i = 0;
			const auto len = base_t::size();
			while(i<len&&str[i])
				if(base_t::cbegin()[i]!=str[i])
					return false;
				else
					++i;
			return str[i]==0;
		}
		//find
		[[nodiscard]]constexpr size_t find(const char_T ch)const{
			return in_range_size_t(ch, *this);
		}
		[[nodiscard]]constexpr size_t reverse_find(const char_T ch)const{
			return in_range_but_reverse_size_t(ch, *this);
		}
		[[nodiscard]]constexpr size_t find(string_view_t str)const{
			return in_range_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t reverse_find(string_view_t str)const{
			return in_range_but_reverse_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find(const constexpr_str_t<char_T>&str)const{
			return in_range_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t reverse_find(const constexpr_str_t<char_T>&str)const{
			return in_range_but_reverse_size_t(str, *this);
		}
		//others by range_n::find_last_of_size_t etc
		[[nodiscard]]constexpr size_t find_first_of(const char_T ch)const{
			return range_n::find_first_of_size_t(array_like_view_t<const char_T>(&ch, 1), *this);
		}
		[[nodiscard]]constexpr size_t find_last_of(const char_T ch)const{
			return range_n::find_last_of_size_t(array_like_view_t<const char_T>(&ch, 1), *this);
		}
		[[nodiscard]]constexpr size_t find_first_not_of(const char_T ch)const{
			return range_n::find_first_not_of_size_t(array_like_view_t<const char_T>(&ch, 1), *this);
		}
		[[nodiscard]]constexpr size_t find_last_not_of(const char_T ch)const{
			return range_n::find_last_not_of_size_t(array_like_view_t<const char_T>(&ch, 1), *this);
		}
		[[nodiscard]]constexpr size_t find_first_of(string_view_t str)const{
			return range_n::find_first_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_last_of(string_view_t str)const{
			return range_n::find_last_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_first_not_of(string_view_t str)const{
			return range_n::find_first_not_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_last_not_of(string_view_t str)const{
			return range_n::find_last_not_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_first_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_first_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_last_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_last_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_first_not_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_first_not_of_size_t(str, *this);
		}
		[[nodiscard]]constexpr size_t find_last_not_of(const constexpr_str_t<char_T>&str)const{
			return range_n::find_last_not_of_size_t(str, *this);
		}
	};
	typedef string_view_t<char_t>string_view;
}
using string_view_n::string_view_t;
using string_view_n::string_view;

//file_end

