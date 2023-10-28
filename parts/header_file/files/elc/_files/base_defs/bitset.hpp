//data.hpp
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
//bitset
template<size_t bit_num>
struct bitset:byteset<ceil_div(bit_num,bitnum_of(byte))>{
	static constexpr size_t data_size=ceil_div(bit_num,bitnum_of(byte));
	typedef byteset<data_size> base_t;
	typedef bitset<bit_num> this_t;
	using base_t::base_t;

	byte _data[data_size];

	class arec_t{
		this_t* _to;
		size_t _index;
		size_t _quot,_mod;
	public:
		arec_t(this_t*to,size_t index)noexcept:_to(to),_index(index){
			auto info=divmod(_index,bitnum_of(byte));
			_quot=info.quot;
			_mod=info.mod;
		}
		constexpr operator bool()const noexcept{
			return _to->_data[_quot]&(1<<_mod);
		}
		constexpr auto&operator=(bool b)noexcept{
			if(b)
				_to->_data[_quot]|=(byte)1<<_mod;
			else
				_to->_data[_quot]&=~byte(1<<_mod);
			return*this;
		}
		constexpr auto&operator&=(bool b)noexcept{
			if(!b)
				_to->_data[_quot]&=~byte(1<<_mod);
			return*this;
		}
		constexpr auto&operator|=(bool b)noexcept{
			if(b)
				_to->_data[_quot]|=(byte)1<<_mod;
			return*this;
		}
		constexpr auto&operator^=(bool b)noexcept{
			if(b)
				_to->_data[_quot]^=(byte)1<<_mod;
			return*this;
		}
	};
private:
	struct iterator_base_t{
		this_t*_to;
		ptrdiff_t _index;		//rend: -1，使用size_t也不是不可以（标准允许无符号整数溢出），但是operator<=>会出问题
		[[nodiscard]]constexpr iterator_base_t	get_before()const noexcept{ return{_to,_index-1}; }
		[[nodiscard]]constexpr iterator_base_t	get_next()const noexcept{ return{_to,_index+1}; }
		[[nodiscard]]arec_t						get_value()noexcept{ return (*_to)[_index]; }
		[[nodiscard]]const arec_t				get_value()const noexcept{ return (*add_const(_to))[_index]; }
		[[nodiscard]]constexpr bool operator==(const iterator_base_t& a)const noexcept{ return _to==a._to && _index==a._index; }
		[[nodiscard]]constexpr auto operator<=>(const iterator_base_t& a)const noexcept{ return _to==a._to ? _index<=>a._index : partial_ordering::unordered; }
	};
	[[nodiscard]]iterator_base_t get_iterator_data_at(ptrdiff_t index)const noexcept{ return iterator_base_t{(this_t*)this,index}; }
public:
	typedef iterator_t<bool,iterator_base_t>				iterator;
	typedef const_iterator_t<bool,const iterator_base_t>	const_iterator;
	constexpr iterator begin()noexcept{ return get_iterator_data_at(0); }
	constexpr iterator end()noexcept{ return get_iterator_data_at(-1); }
	constexpr const_iterator begin()const noexcept{ return get_iterator_data_at(0); }
	constexpr const_iterator end()const noexcept{ return get_iterator_data_at(-1); }
	constexpr const_iterator cbegin()const noexcept{ return get_iterator_data_at(0); }
	constexpr const_iterator cend()const noexcept{ return get_iterator_data_at(-1); }
	constexpr arec_t operator[](size_t index)noexcept{return {this,index};}
	constexpr const arec_t operator[](size_t index)const noexcept{return {remove_const(this),index};}
	constexpr auto operator~()const noexcept{
		this_t tmp;
		for(size_t i=0;i<data_size;++i)
			tmp._data[i]=~_data[i];
		return tmp;
	}
	constexpr auto&operator&=(const this_t&b)noexcept{
		for(size_t i=0;i<data_size;++i)
			_data[i]&=b._data[i];
		return*this;
	}
	constexpr auto&operator|=(const this_t&b)noexcept{
		for(size_t i=0;i<data_size;++i)
			_data[i]|=b._data[i];
		return*this;
	}
	constexpr auto&operator^=(const this_t&b)noexcept{
		for(size_t i=0;i<data_size;++i)
			_data[i]^=b._data[i];
		return*this;
	}
	template<unsigned_integer_type T>
	constexpr auto&operator=(T n)noexcept{
		typedef unsigned_specific_size_t<sizeof(byte)> arithmetic_byte_t;
		constexpr size_t mod_base=number_of_possible_values_per<arithmetic_byte_t>;
		for(size_t i=0;i<data_size;++i){
			auto result=divmod(n,mod_base);
			_data[i]=byte(arithmetic_byte_t(move(result.mod)));
			n=T(move(result.quot));
		}
		return*this;
	}
	template<unsigned_integer_type T>
	constexpr bitset(T n)noexcept{*this=n;}
	template<unsigned_integer_type T>
	explicit constexpr operator T()const noexcept{
		typedef unsigned_specific_size_t<sizeof(byte)> arithmetic_byte_t;
		constexpr size_t mod_base=number_of_possible_values_per<arithmetic_byte_t>;
		T result{};
		for(size_t i=data_size-1;i!=npos;--i){
			result*=mod_base;
			result+=arithmetic_byte_t(_data[i]);
		}
		return result;
	}
	constexpr auto&operator<<=(size_t n)noexcept{
		const auto [move_step,offset]=divmod(n,bitnum_of(byte));
		byte carry{};
		for(size_t i=data_size-move_step-1;i!=npos;--i)
			carry=swap(_data[i],byte((_data[i]<<offset)|carry))>>(bitnum_of(byte)-offset);
		for(size_t i=0;i<move_step;++i)
			_data[i]=byte{};
		return*this;
	}
	constexpr auto&operator>>=(size_t n)noexcept{
		const auto [move_step,offset]=divmod(n,bitnum_of(byte));
		byte carry{};
		for(size_t i=move_step;i!=data_size;++i)
			carry=swap(_data[i],byte((_data[i]>>offset)|carry))<<(bitnum_of(byte)-offset);
		for(size_t i=data_size-move_step;i!=data_size;++i)
			_data[i]=byte{};
		return*this;
	}
	constexpr auto operator<<(size_t n)const noexcept{return this_t(*this)<<=n;}
	constexpr auto operator>>(size_t n)const noexcept{return this_t(*this)>>=n;}
	constexpr auto operator&(const this_t&b)const noexcept{return this_t(*this)&=b;}
	constexpr auto operator|(const this_t&b)const noexcept{return this_t(*this)|=b;}
	constexpr auto operator^(const this_t&b)const noexcept{return this_t(*this)^=b;}
	constexpr auto operator==(const this_t&b)const noexcept{
		for(size_t i=0;i<data_size-1;++i)
			if(_data[i]!=b._data[i])
				return false;
		const auto mod=bit_num%bitnum_of(byte);
		return (_data[data_size-1]>>mod)==(b._data[data_size-1]>>mod);
	}
};

//file_end

