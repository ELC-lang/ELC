//bit.hpp
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
namespace bit{
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const auto R)noexcept;
	#define rot_base(opt,antiopt) static_cast<T>(static_cast<T>(v opt r) | static_cast<T>(v antiopt (d - r)))
	/// 位操作：循环右移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotr_nomod(const T v,const auto r)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		if constexpr(unsigned_type<decltype(r)>){
			if(r)
				return rot_base(>>,<<);
			else
				return v;
		}
		else{
			if(r>0)
				return rot_base(>>,<<);
			elseif(r==0)
				return v;
			else//r<0
				return rotl_nomod(v,0-r);
		}
	}
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const auto r)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		if constexpr(unsigned_type<decltype(r)>){
			if(r)
				return rot_base(<<,>>);
			else
				return v;
		}
		else{
			if(r>0)
				return rot_base(<<,>>);
			elseif(r==0)
				return v;
			else//r<0
				return rotr_nomod(v,0-r);
		}
	}
	#undef rot_base
	/// 位操作：循环右移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotr(const T v,const auto R)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		const auto r = mod(R,d);
		return rotr_nomod(v,r);
	}
	/// 位操作：循环左移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotl(const T v,const auto R)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		const auto r = mod(R,d);
		return rotl_nomod(v,r);
	}
	template<unsigned_basic_integer_type T>
	class rot_iterator{
		static constexpr auto rot_offset_npos = ::std::numeric_limits<T>::digits;//d
		size_t _offset;
	public:
		force_inline constexpr rot_iterator(size_t offset)noexcept:_offset(offset%rot_offset_npos){}
		force_inline constexpr rot_iterator&operator++()noexcept{
			_offset++;
			if(_offset==rot_offset_npos)
				_offset=0;
			return*this;
		}
		force_inline constexpr rot_iterator&operator--()noexcept{
			if(_offset==0)
				_offset=rot_offset_npos;
			_offset--;
			return*this;
		}
		force_inline constexpr rot_iterator operator++(int)noexcept{
			rot_iterator tmp(*this);
			operator++();
			return tmp;
		}
		force_inline constexpr rot_iterator operator--(int)noexcept{
			rot_iterator tmp(*this);
			operator--();
			return tmp;
		}
		[[nodiscard]]force_inline constexpr size_t value()const noexcept{
			return _offset;
		}
	};
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const rot_iterator<T>&r)noexcept{
		return rotl_nomod(v,r.value());
	}
	/// 位操作：循环右移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotr_nomod(const T v,const rot_iterator<T>&r)noexcept{
		return rotr_nomod(v,r.value());
	}
	/// 位操作：循环左移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotl(const T v,const rot_iterator<T>&r)noexcept{
		return rotl_nomod(v,r);
	}
	/// 位操作：循环右移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr auto rotr(const T v,const rot_iterator<T>&r)noexcept{
		return rotr_nomod(v,r);
	}
	/// 获取位数
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t get_bitnum(const T v)noexcept{
		if constexpr(sizeof(T)!=1){//优化
			typedef unsigned_specific_size_t<sizeof(T)/2> half_t;
			constexpr size_t half_bitnum = bitnum_of(half_t);
			const half_t high = half_t(v>>half_bitnum);
			if(high)
				return half_bitnum+get_bitnum(high);
			else
				return get_bitnum(half_t(v));
		}
		else
			//默认通用实现
			return v?get_bitnum(T(v>>1))+1:0;
	}
	/// 获取位数
	template<signed_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t get_bitnum(const T v)noexcept{
		return get_bitnum(abs(v));
	}
	/// countl_zero
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countl_zero(const T v)noexcept{
		if constexpr(sizeof(T)!=1){//优化
			typedef unsigned_specific_size_t<sizeof(T)/2> half_t;
			constexpr size_t half_bitnum = bitnum_of(half_t);
			const half_t high = half_t(v>>half_bitnum);
			if(high)
				return countl_zero(high);
			else
				return half_bitnum+countl_zero(half_t(v));
		}
		else
			//默认通用实现
			return bitnum_of(T)-get_bitnum(v);
	}
	/// countr_one
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countr_one(const T v)noexcept{
		if constexpr(sizeof(T)!=1){//优化
			typedef unsigned_specific_size_t<sizeof(T)/2> half_t;
			constexpr size_t half_bitnum = bitnum_of(half_t);
			const half_t low = half_t(v);
			if(low == half_t(-1))
				return half_bitnum+countr_one(half_t(v>>half_bitnum));
			else
				return countr_one(low);
		}
		else
			//默认通用实现
			return v&1?countr_one(T(v>>1))+1:0;
	}
	/// countl_one
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countl_one(const T v)noexcept{
		if constexpr(sizeof(T)!=1){//优化
			typedef unsigned_specific_size_t<sizeof(T)/2> half_t;
			constexpr size_t half_bitnum = bitnum_of(half_t);
			const half_t high = half_t(v>>half_bitnum);
			if(high == half_t(-1))
				return half_bitnum+countl_one(half_t(v));
			else
				return countl_one(high);
		}
		else
			//默认通用实现return countl_zero(T(~v));，不如下面这个（虽然只能用于char）
			return v&0x80?countl_one(T(v<<1))+1:0;
	}
	/// countr_zero
	template<unsigned_basic_integer_type T>
	[[nodiscard]]force_inline constexpr size_t countr_zero(const T v)noexcept{
		if constexpr(sizeof(T)!=1){//优化
			typedef unsigned_specific_size_t<sizeof(T)/2> half_t;
			constexpr size_t half_bitnum = bitnum_of(half_t);
			const half_t low = half_t(v);
			if(low)
				return countr_zero(low);
			else
				return half_bitnum+countr_zero(half_t(v>>half_bitnum));
		}
		else
			//默认通用实现
			return countr_one(T(~v));
	}
}
using bit::rotl;
using bit::rotr;
using bit::rotl_nomod;
using bit::rotr_nomod;
using bit::rot_iterator;
using bit::get_bitnum;
using bit::countl_zero;
using bit::countr_zero;
using bit::countl_one;
using bit::countr_one;

//file_end

