//ubigint.hpp
//at namespace elc::defs::bignum_n
/*
未完成的elc解释器bignum文件
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
class ubigint{
public:
	#if in_debug && !defined(ELC_SPEED_TEST)
		typedef unsigned char base_type;
	#else
		typedef unsigned_specific_size_fast_t<sizeof(uintmax_t)/2> base_type;
	#endif
private:
	typedef array_t<base_type> data_type;
	typedef array_like_view_t<const base_type> data_view_type;

	template<unsigned_basic_integer_type T>
	static constexpr size_t base_type_size_of=ceil_div(sizeof(T),sizeof(base_type));
	static constexpr auto base_type_mod=number_of_possible_values_per<base_type>;

	data_type _data;

	explicit ubigint(const data_type&data)noexcept:_data(data){}
	explicit ubigint(data_type&&data)noexcept:_data(move(data)){}
public:
	ubigint()noexcept = default;
	ubigint(const ubigint&)noexcept = default;
	ubigint(ubigint&&)noexcept = default;
	ubigint(const zero_t&)noexcept:ubigint(){}
	ubigint& operator=(const ubigint&)&noexcept = default;
	ubigint& operator=(ubigint&&)&noexcept = default;
	ubigint& operator=(const zero_t&)noexcept{
		_data.clear();
		return*this;
	}

	~ubigint() = default;
public:
	template<unsigned_basic_integer_type T>
	ubigint(T value)noexcept{
		constexpr auto size = base_type_size_of<T>;
		_data.resize(size);
		auto i=_data.begin();
		while(value){
			const auto info=divmod(value,base_type_mod);
			*i=base_type(info.mod);
			value=T(info.quot);
			i++;
		}
		const auto used_size=i-_data.begin();
		if(used_size!=size)
			_data.resize(used_size);
	}
	template<basic_float_type T>
	explicit ubigint(T&&value)noexcept{
		remove_cvref<T> int_part;
		discard(::std::modf(value,&int_part));
		const auto info=get_precision_and_exponent(int_part);
		*this=ubigint(info.precision);
		*this<<=info.exponent;
	}
public:
	template<arithmetic_type T>
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		if constexpr(type_info<T>!=type_info<ubigint>){
			if constexpr(has_min<T>){
				constexpr auto min_value=min(type_info<T>);
				if constexpr(min_value)
					if(*this<min_value)
						return false;
			}
			if constexpr(has_max<T>){
				constexpr auto max_value=max(type_info<T>);
				constexpr size_t max_byte_size=ceil_div(get_bitnum(max_value),bitnum_of(byte));
				if(_data.size_in_byte()>max_byte_size)
					return false;
				if constexpr(not all_bit_is_one(max_byte_size))
					if(*this>max_value)
						return false;
			}
		}
		return true;
	}
	template<arithmetic_type T>
	[[nodiscard]]T convert_to()const noexcept{
		if constexpr(type_info<T>!=type_info<ubigint>){
			T value=0;
			auto i=_data.rbegin();
			while(i!=_data.rend()){
				value=T(value*base_type_mod);
				value+=*i++;
			}
			return value;
		}
		else
			return *this;
	}

	//explicit operator T
	template<arithmetic_type T>
	[[nodiscard]]explicit operator T()&&noexcept{
		return move(*this).convert_to<T>();
	}
	template<arithmetic_type T>
	[[nodiscard]]explicit operator T()const&noexcept{
		return convert_to<T>();
	}
public:
	//rand所需
	friend void apply_basetype_to_head(ubigint&x,base_type value)noexcept{
		x._data.push_back(value);
	}
private:
	static void shrink_to_fit(data_type&a)noexcept{
		auto size=a.size();
		while(size--)
			if(a[size]!=0){
				a.resize(size+1);
				return;
			}
		a.clear();
	}
	void shrink_to_fit()noexcept{
		shrink_to_fit(_data);
	}
	[[nodiscard]]static data_view_type get_data_view_of_data(const base_type*data,size_t size)noexcept{
		return data_view_type{data,size};
	}
	[[nodiscard]]static data_view_type get_shrinked_data_view_of_data(const base_type*data,size_t size)noexcept{
		size_t i = size;
		while(i--)
			if(data[i]!=0)
				break;
		return data_view_type{data,i+1};
	}
	[[nodiscard]]static data_view_type get_data_view_of_data(const data_type&a)noexcept{
		return get_data_view_of_data(a.data(), a.size());
	}
	[[nodiscard]]static data_view_type get_shrinked_data_view_of_data(const data_type&a)noexcept{
		return get_shrinked_data_view_of_data(a.data(), a.size());
	}
	[[nodiscard]]data_view_type get_data_view()const noexcept{
		return get_data_view_of_data(_data);
	}
	[[nodiscard]]data_view_type get_shrinked_data_view()const noexcept{
		return get_shrinked_data_view_of_data(_data);
	}
	[[nodiscard]]static data_view_type re_shrink(data_view_type a)noexcept{
		return get_shrinked_data_view_of_data(a.data(), a.size());
	}
public:
	//operator==
	[[nodiscard]]bool operator==(const ubigint& other)const noexcept{
		return equal(get_data_view(),other.get_data_view());
	}
	template<integer_type T>
	[[nodiscard]]bool operator==(T other)const noexcept{
		if constexpr(signed_type<T>){
			if(is_negative(other))return false;
			return *this==to_unsigned_t<T>(other);
		}else{
			auto i=_data.begin();
			const auto end=_data.end();
			while(i!=end){
				if(other<*i)
					return false;
				other-=*i;
				other=T(other/base_type_mod);
				i++;
			}
			return other==0;
		}
	}
	//operator<=>
private:
	//由于低位在前，不能使用elc的默认compare
	[[nodiscard]]static auto compare(data_view_type a, data_view_type b)noexcept{
		if(const auto tmp=a.size()<=>b.size(); tmp!=0)
			return tmp;

		auto i=a.size();
		while(i--)
			if(const auto tmp=a[i]<=>b[i]; tmp!=0)
				return tmp;
		return strong_ordering::equivalent;
	}
	[[nodiscard]]static auto compare(const data_type&a, const data_type&b)noexcept{
		return compare(get_data_view_of_data(a), get_data_view_of_data(b));
	}
	[[nodiscard]]static auto compare(const data_type&a, data_view_type b)noexcept{
		return compare(get_data_view_of_data(a), b);
	}
	[[nodiscard]]static auto compare(data_view_type a, const data_type&b)noexcept{
		return compare(a, get_data_view_of_data(b));
	}
public:
	[[nodiscard]]auto operator<=>(const ubigint& other)const noexcept{
		return compare(get_data_view(),other.get_data_view());
	}
	template<integer_type T>
	[[nodiscard]]auto operator<=>(T other)const noexcept{
		if constexpr(signed_type<T>){
			if(is_negative(other))return strong_ordering::greater;
			return *this<=>to_unsigned_t<T>(other);
		}else{
			if constexpr(is_basic_type<T>)
				if(auto tmp=_data.size()<=>base_type_size_of<T>;tmp!=0)//根据位数快速判断大小
					return tmp;
			auto i=_data.begin();
			const auto end=_data.end();
			while(i!=end){
				if(other<*i)//this>other
					return strong_ordering::greater;
				other-=*i;
				other=T(other/base_type_mod);
				i++;
			}
			return other==0?strong_ordering::equivalent:strong_ordering::less;
		}
	}
	//调试用，返回一个可以输出到流的内部结构查看器
	[[nodiscard]]auto get_data_viewer()const noexcept{
		return lambda_with_catch(this)(auto&stream)noexcept->auto&{
			for(auto& i: _data)
				stream << i << U' ';
			return stream;
		};
	}
private:
	//operator+-*/%s
	typedef unsigned_specific_size_fast_t<2*sizeof(base_type)> calc_type;
	//+
	[[nodiscard]]static size_t get_safety_add_buf_size_diff(data_view_type a,data_view_type b)noexcept{
		//判断进位所需空间
		if(a.size()!=b.size()){
			auto i = a.size();
			while(i-- != b.size())//判断进位区是否没有足够的空间以至于需要进位
				if(a[i]!=max(type_info<base_type>))//任意一位不是最大值就不需要进位
					return 0;
			return 1;
		}
		else{
			//只需要判断最高位是否需要进位
			const auto res=calc_type(a.back())+calc_type(b.back())+1;//+1是因为次高位可能进位
			return static_cast<size_t>(res>>bitnum_of(base_type));
		}
	}
	[[nodiscard]]static size_t get_safety_add_buf_size(data_view_type a,data_view_type b)noexcept{
		return a.size()+get_safety_add_buf_size_diff(a,b);
	}
	[[nodiscard]]static size_t get_safety_add_buf_size_with_not_compared_buf(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())
			swap(a,b);
		return get_safety_add_buf_size(a,b);
	}
	[[nodiscard]]static data_type add_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())
			swap(a,b);
		if(!b.size())return data_type(a);
		auto base_size = a.size();
		const auto size_diff = get_safety_add_buf_size_diff(a,b);
		const auto size = base_size+size_diff;

		array_t<base_type> tmp(note::size(size));
		copy_assign[base_size](tmp.data(),a.data());
		copy_assign[size_diff](note::to(tmp.data()+base_size),base_type{0});
		add_to_base(tmp.data(),b);
		if(size_diff)
			shrink_to_fit(tmp);
		return tmp;
	}
	static void add_to_base(data_type&a,data_view_type b)noexcept{
		if(!b.size())return;
		const auto this_view = get_data_view_of_data(a);
		if(!this_view.size()){a=b;return;}
		const auto&other_view = b;
		const auto origin_size = this_view.size();
		const auto new_size = get_safety_add_buf_size_with_not_compared_buf(this_view,other_view);
		const auto size_diff = new_size - origin_size;
		if(size_diff)
			a.insert(origin_size,size_diff,base_type{0});
		add_to_base(a.data(),other_view);
		if(size_diff)
			shrink_to_fit(a);
	}
	static void add_to_base(data_type&a,const data_type&b)noexcept{
		add_to_base(a,get_data_view_of_data(b));
	}
	static void add_to_base(base_type*buf,data_view_type b)noexcept{
		bool is_overflows = 0;
		const auto end_size = b.size();
		auto other_begin = b.begin();

		for(size_t i=0;i<end_size;++i)
			*buf++ = add_carry(*buf,*other_begin++,is_overflows);
		while(is_overflows)
			*buf++ = add_carry(*buf,is_overflows);
	}
	static void add_to_base(base_type*buf,const data_type&b)noexcept{
		add_to_base(buf,get_data_view_of_data(b));
	}
	template<unsigned_basic_integer_type T>
	static void add_to_base(data_type&buf,T num)noexcept{
		//定义运算类型：若T比calc_type大，则使用T，否则使用calc_type
		using calc_t = conditional<(sizeof(T) > sizeof(calc_type)),T,calc_type>;
		//初始化
		calc_t tmp=num;
		base_type*iter=buf.begin();
		const auto end=buf.end();
		bool is_overflows = 0;
		while(iter!=end && tmp){
			tmp=add_carry(calc_t(*iter),tmp,is_overflows);
			*iter++=base_type(tmp);
			tmp>>=bitnum_of(base_type);
		}
		if(tmp){
			{
				const auto iter_pos=iter-buf.begin();
				buf.resize(buf.size()+ceil_div(sizeof(tmp),sizeof(base_type)));
				iter=buf.begin()+iter_pos;
			}
			while(tmp){
				*iter=add_carry(base_type(tmp),is_overflows);
				tmp>>=bitnum_of(base_type);
				++iter;
			}
			buf.resize(iter-buf.begin());
		}
	}
	/// 将b向前偏移offset个base_type后加到buf上，等价于buf+=b<<offset*bitnum_of(base_type)
	/// 用于乘法优化
	static void offset_add_to_base(base_type*buf,const data_type&b,size_t offset)noexcept{
		add_to_base(buf+offset,get_data_view_of_data(b));
	}
	static void offset_add_to_base(data_type&buf,data_view_type b,size_t offset)noexcept{
		//检查是否需要扩容
		if(buf.size()<=offset+b.size()){
			const auto size_now = buf.size();
			const auto size_need = offset+b.size()+1;//考虑进位
			const auto size_diff = size_need - size_now;
			buf.insert(size_now,size_diff,base_type{0});//扩容&填充0
		}
		offset_add_to_base(buf.data(),b,offset);
		shrink_to_fit(buf);
	}
	auto& offset_add_to_base(data_view_type b,size_t offset)noexcept{
		offset_add_to_base(_data,b,offset);
		return*this;
	}
	auto& offset_add_to_base(const data_type&b,size_t offset)noexcept{
		return offset_add_to_base(get_data_view_of_data(b),offset);
	}
	auto& offset_add_to_base(const ubigint&b,size_t offset)noexcept{
		return offset_add_to_base(b._data,offset);
	}
	//-
	[[nodiscard]]static data_type sub_base(data_view_type a,data_view_type b)noexcept{
		//调用方保证a>=b
		const auto size = a.size();

		array_t<base_type> tmp(note::size(size));
		copy_assign[size](tmp.data(),a.data());
		sub_with_base(tmp,b);//already shrink_to_fit ed
		return tmp;
	}
	static void sub_with_base(base_type*buf,data_view_type b)noexcept{
		//调用方保证a>=b
		bool is_overflows = 0;
		const auto end_size = b.size();
		auto other_begin = b.begin();

		for(size_t i=0;i<end_size;++i)
			*buf++ = sub_borrow(*buf,*other_begin++,is_overflows);
		while(is_overflows)
			*buf++ = sub_borrow(*buf,is_overflows);
	}
	static void sub_with_base(base_type*buf,const data_type&b)noexcept{
		sub_with_base(buf,get_data_view_of_data(b));
	}
	static void sub_with_base(data_type&buf,data_view_type b)noexcept{
		sub_with_base(buf.data(),b);
		shrink_to_fit(buf);
	}
	static void sub_with_base(data_type&buf,const data_type&b)noexcept{
		sub_with_base(buf,get_data_view_of_data(b));
	}
	[[nodiscard]]static data_type sub_base(data_view_type a,const data_type&b)noexcept{
		return sub_base(a,get_data_view_of_data(b));
	}
	[[nodiscard]]static data_type sub_base(const data_type&a,data_view_type b)noexcept{
		return sub_base(get_data_view_of_data(a),b);
	}
	[[nodiscard]]static data_type sub_base(const data_type&a,const data_type&b)noexcept{
		return sub_base(get_data_view_of_data(a),get_data_view_of_data(b));
	}
	//++
	//add_one_to_base
	static void add_one_to_base(data_type&buf)noexcept{
		base_type*iter=buf.begin();
		const auto end=buf.end();
		while(iter!=end){
			++*iter;
			if(*iter)
				return;
			++iter;
		}
		buf.resize(buf.size()+1);
		buf.back()=1;
	}
	void add_one_to_base()noexcept{
		add_one_to_base(_data);
	}
	//--
	//sub_one_from_base
	static void sub_one_from_base(data_type&buf)noexcept{
		base_type*iter=buf.begin();
		const auto end=buf.end();
		while(iter!=end){
			if(*iter){
				if((!--*iter) && iter==buf.rbegin())
					buf.resize(buf.size()-1);
				return;
			}
			*iter=max(type_info<base_type>);
		}
		//整个buf都是0，说明buf是0，不需要做任何操作
	}
	void sub_one_from_base()noexcept{
		sub_one_from_base(_data);
	}
	//*
	//这些东西比subview快，因为没有无用的检查
	//**不要使用subview替代**
	//shrink_of_end_zeros
	//去掉（数理上）末尾的（实现上）开头的0以减少乘法的次数
	[[nodiscard]]static size_t shrink_of_end_zeros(data_view_type&buf)noexcept{
		if(buf.empty())
			return 0;
		auto begin=buf.begin();
		const auto end=buf.end();
		while(begin!=end && !*begin)
			++begin;
		const size_t aret=begin-buf.begin();
		const size_t size=end-begin;
		buf=get_data_view_of_data(begin,size);
		return aret;
	}
	//unshrink_of_end_zeros
	//就是他妈的撤销
	[[nodiscard]]static data_view_type unshrink_of_end_zeros(data_view_type a,size_t zeros)noexcept{
		return get_data_view_of_data(a.begin()-zeros,a.size()+zeros);
	}
	//apply_shrink_of_end_zeros
	//对于data_type和data_view_type应用已经获得的zeros大小进行shrink
	static void apply_shrink_of_end_zeros(data_type&buf,size_t zeros)noexcept{
		if(zeros)
			buf.forward_resize(buf.size()-zeros);
	}
	static void apply_shrink_of_end_zeros(data_view_type&buf,size_t zeros)noexcept{
		if(zeros)
			buf=get_data_view_of_data(buf.begin()+zeros,buf.size()-zeros);
	}
	[[nodiscard]]static data_type muti_base(data_view_type a,base_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		muti_with_base(tmp.data(),a,b);
		shrink_to_fit(tmp);
		return tmp;
	}
	static void muti_with_base_no_optimisation(base_type*buf,data_view_type a,base_type b)noexcept{
		size_t i=0;
		calc_type num=0;
		while(i!=a.size()){
			num+=calc_type(a[i])*calc_type(b);
			*buf = base_type(num%base_type_mod);
			num/=base_type_mod;

			i++;
			buf++;
		}
		if(num)
			*buf = base_type(num);
		else
			*buf = base_type{0};
	}
	static void muti_with_base(base_type*buf,data_view_type a,base_type b)noexcept{
		if(!b){
			copy_assign[a.size()+1](note::to(buf),base_type{0});
			return;
		}
		{
			const auto zeros=shrink_of_end_zeros(a);
			copy_assign[zeros](note::to(buf),base_type{0});
			buf+=zeros;
		}
		return muti_with_base_no_optimisation(buf,a,b);
	}
	static void muti_with_base(base_type*buf,data_view_type a,data_view_type b)noexcept{
		{
			const auto zeros=shrink_of_end_zeros(a)+shrink_of_end_zeros(b);
			copy_assign[zeros](note::to(buf),base_type{0});
			buf+=zeros;
		}
		array_t<base_type> tmp(note::size(a.size()+1));
		size_t muti_scale=0;
		while(muti_scale!=b.size()){
			if(b[muti_scale]){
				muti_with_base_no_optimisation(tmp.data(),a,b[muti_scale]);
				add_to_base(buf+muti_scale,get_shrinked_data_view_of_data(tmp));
			}
			muti_scale++;
		}
	}
	[[nodiscard]]static data_type muti_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())swap(a,b);//大数在前循环数小
		array_t<base_type> tmp(note::size(a.size()+b.size()),0);
		muti_with_base(tmp.data(),a,b);
		shrink_to_fit(tmp);
		return tmp;
	}
	//分割乘法以提高效率
	[[nodiscard]]static data_type fast_muti_base(data_view_type a,data_view_type b)noexcept{
		constexpr auto fast_muti_base_threshold=1<<6;//对小于1000的随机数阶乘测试后选择此数
		if(min(a.size(),b.size())<fast_muti_base_threshold)
			return muti_base(a,b);
		//计算分割点
		const auto split_point=max(
			min((a.size()+1)/2,b.size()-1),
			min(a.size()-1,(b.size()+1)/2)
		);
		//拆成4个数
		const auto a_split_point=a.data()+split_point;
		const data_view_type a_low=get_shrinked_data_view_of_data(a.data(),split_point);
		const data_view_type a_high{a_split_point,a.size()-split_point};
		const auto b_split_point=b.data()+split_point;
		const data_view_type b_low=get_shrinked_data_view_of_data(b.data(),split_point);
		const data_view_type b_high{b_split_point,b.size()-split_point};
		//计算结果
		ubigint high{fast_muti_base(a_high,b_high)};
		ubigint low{fast_muti_base(a_low,b_low)};
		ubigint middle{fast_muti_base(add_base(a_high,a_low),add_base(b_high,b_low))};
		//合并结果
		middle -= high+low;
		return move(low.offset_add_to_base(high,split_point*2).
						offset_add_to_base(middle,split_point)._data);
	}
	[[nodiscard]]static data_type fast_muti_base(const data_type& a,const data_type& b)noexcept{
		return fast_muti_base(get_data_view_of_data(a),get_data_view_of_data(b));
	}
	[[nodiscard]]static data_type fast_muti_base(const data_type& a,data_view_type b)noexcept{
		return fast_muti_base(get_data_view_of_data(a),b);
	}
	[[nodiscard]]static data_type fast_muti_base(data_view_type a,const data_type& b)noexcept{
		return fast_muti_base(a,get_data_view_of_data(b));
	}
	[[nodiscard]]static data_type fast_muti_base(data_view_type a,const ubigint& b)noexcept{
		return fast_muti_base(a,b.get_data_view());
	}
	///
	//除法优化：在不需要余数的情况下可以进行的优化
	[[nodiscard]]static size_t get_no_mod_optimisation_size(const data_type&a,data_view_type b)noexcept{
		return get_no_mod_optimisation_size(get_data_view_of_data(a),b);
	}
	[[nodiscard]]static size_t get_no_mod_optimisation_size(data_view_type a,data_view_type b)noexcept{
		return no_mod_optimisation_of_div(a,b);
	}
	static size_t no_mod_optimisation_of_div(data_view_type&a,data_view_type&b)noexcept{
		size_t aret=0;
		//去除末尾0
		{
			const auto zeros=shrink_of_end_zeros(b);
			apply_shrink_of_end_zeros(a,zeros);
			aret+=zeros;
		}
		//被除数不及两倍除数长度减2时，可以忽略一部分最低位且不影响结果
		if ((b.size()-1)*2 > a.size()) {
			const auto ans_len = a.size() - b.size() + 2;
			const auto shr = b.size() - ans_len;
			a=a.subview(shr);
			b=b.subview(shr);
			aret+=shr;
			aret+=no_mod_optimisation_of_div(a,b);
		}
		return aret;
	}
	static size_t no_mod_optimisation_of_div(data_type&a,data_view_type&b)noexcept{
		size_t aret=0;
		//去除末尾0
		{
			const auto zeros=shrink_of_end_zeros(b);
			apply_shrink_of_end_zeros(a,zeros);
			aret+=zeros;
		}
		//被除数不及两倍除数长度减2时，可以忽略一部分最低位且不影响结果
		if ((b.size()-1)*2 > a.size()) {
			const auto ans_len = a.size() - b.size() + 2;
			const auto shr = b.size() - ans_len;
			a.forward_resize(a.size()-shr);
			b=b.subview(shr);
			aret+=shr;
			aret+=no_mod_optimisation_of_div(a,b);
		}
		return aret;
	}
	//除法实现
	[[nodiscard]]static base_type div_with_base_no_optimisation(data_type&buf,base_type*a,data_view_type b)noexcept{
		data_view_type tryto{a,b.size()+1};
		const calc_type dividend=exlambda{
			const base_type*p=tryto.rbegin();
			auto tmp=calc_type(*p);tmp*=base_type_mod;tmp+=calc_type(p[-1]);
			return tmp;
		}();
		const calc_type divisor=calc_type(b.back());
		calc_type left=dividend/(divisor+1);
		calc_type right=calc_type(dividend/divisor);
		right=min(right,(calc_type)max(type_info<base_type>));
		if(right==0)return 0;//a/b<=right==0
		base_type last_work_able=0;
		//left<=a/b<=right
		tryto=get_shrinked_data_view_of_data(tryto.data(),tryto.size());
		while(left<=right) {
			const calc_type test=(left+right)/2;//二分法
			muti_with_base(buf.data(),b,base_type(test));
			const auto myview=get_shrinked_data_view_of_data(buf);
			const auto cmp=compare(tryto,myview);
			if(cmp>=0)
				last_work_able=base_type(test);
			if(cmp>0){//tryto>myview：测试值太小或合适
				left=test+1;
				if(!base_type(left))//溢出了，test是最大的可用值
					break;
			}
			elseif(cmp<0)//tryto<myview：测试值太大
				right=test-1;
			else//tryto==myview：测试值合适
				break;
		}
		if(last_work_able==0)return 0;
		muti_with_base(buf.data(),b,last_work_able);
		sub_with_base(a,get_shrinked_data_view_of_data(buf));
		return last_work_able;
	}
	[[nodiscard]]static base_type div_with_base(data_type&buf,base_type*a,data_view_type b)noexcept{
		return div_with_base_no_optimisation(buf,a,b);
	}
	[[nodiscard]]static base_type div_base(base_type*a,data_view_type b)noexcept{
		array_t<base_type> fortry(note::size(b.size()+1));
		return div_with_base(fortry,a,b);
	}
	static data_type div_with_base_no_optimisation_impl(data_type&a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()-b.size()),0);
		const auto end=a.rend();
		auto begin=a.rbegin()+b.size();
		auto tmpwritter=tmp.rbegin();
		array_t<base_type> fortry(note::size(b.size()+1));
		while(begin!=end){
			auto result=div_with_base(fortry,begin,b);
			*tmpwritter=result;
			tmpwritter++;
			begin++;
		};
		shrink_to_fit(tmp);
		return tmp;
	}
	static data_type div_with_base_no_optimisation(data_type&a,data_view_type b)noexcept{
		a.push_back(0);
		return div_with_base_no_optimisation_impl(a,b);
	}
	static data_type div_with_base(data_type&a,data_view_type b)noexcept{
		no_mod_optimisation_of_div(a,b);
		return div_with_base_no_optimisation(a,b);
	}
	[[nodiscard]]static data_type div_with_base(ubigint&a,const ubigint&b)noexcept{
		return div_with_base(a._data,b.get_data_view());
	}
	[[nodiscard]]static data_type div_base_no_optimisation(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		copy_assign[a.size()](tmp.data(), a.data());
		tmp.back()=0;
		tmp=div_with_base_no_optimisation_impl(tmp,b);
		return tmp;
	}
	[[nodiscard]]static data_type div_base(data_view_type a,data_view_type b)noexcept{
		no_mod_optimisation_of_div(a,b);
		return div_base_no_optimisation(a,b);
	}
	//%
	static void mod_with_base_impl(data_type&a,data_view_type b)noexcept{
		const auto end=a.rend();
		auto begin=a.rbegin()+b.size();
		array_t<base_type> fortry(note::size(b.size()+1));
		while(begin!=end){
			discard(div_with_base_no_optimisation(fortry,begin,b));
			begin++;
		};
	}
	static void mod_with_base(data_type&a,data_view_type b)noexcept{
		a.push_back(0);
		mod_with_base_impl(a,b);
		shrink_to_fit(a);
	}
	[[nodiscard]]static data_type mod_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		copy_assign[a.size()](tmp.data(), a.data());
		tmp.back()=0;
		mod_with_base_impl(tmp,b);
		shrink_to_fit(tmp);
		return tmp;
	}
	//divmod
	template<class=void>
	struct divmod_result_t_base{
		ubigint quot;
		ubigint mod;
	};
public:
	typedef divmod_result_t_base<void> divmod_result_t;
private:
	[[nodiscard]]static divmod_result_t divmod_with_base(data_type&a,data_view_type b)noexcept{
		const auto opt_size=get_no_mod_optimisation_size(a,b);
		if(!opt_size){
			data_type quot = div_with_base_no_optimisation(a, b);
			shrink_to_fit(a);
			return {ubigint{move(quot)},ubigint{move(a)}};
		}
		else{
			const auto a_view=get_data_view_of_data(a).subview(opt_size);
			const auto ori_b_view=b; b=b.subview(opt_size);
			data_type quot = div_base_no_optimisation(a_view, b);
			sub_with_base(a,fast_muti_base(quot,ori_b_view));//already shrink_to_fit ed
			return {ubigint{move(quot)},ubigint{move(a)}};
		}
	}
	[[nodiscard]]static divmod_result_t divmod_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp=a;
		return divmod_with_base(tmp,b);
	}
	//除法分治：阈值
	static constexpr auto fast_div_base_threshold=1<<5;
	//除法分治：规则化
	[[nodiscard]]static auto fast_div_regularisation(data_view_type a,data_view_type b)noexcept{
		constexpr auto max_calc_type=min(max(type_info<calc_type>),base_type_mod*base_type_mod-1);
		const auto first2=(b.back()*base_type_mod+*(b.rbegin()+1)+1);
		const ubigint muti=max_calc_type/first2;
		const ubigint a_after_reg{fast_muti_base(a,muti)};
		const ubigint b_after_reg{fast_muti_base(b,muti)};
		struct result_t{
			ubigint a_after_reg;
			ubigint b_after_reg;
			ubigint muti;
		};
		return result_t{move(a_after_reg),move(b_after_reg),move(muti)};
	}
	[[nodiscard]]static divmod_result_t fast_divmod_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<=fast_div_base_threshold)return divmod_base(a,b);
		const auto [a_after_reg,b_after_reg,muti]=fast_div_regularisation(a,b);
		auto result=fast_divmod_base_impl(a_after_reg,b_after_reg);
		result.mod/=muti;
		return result;
	}
	[[nodiscard]]static divmod_result_t fast_divmod_base(const ubigint&a,const ubigint&b)noexcept{
		return fast_divmod_base(a.get_data_view(),b.get_data_view());
	}
	[[nodiscard]]static data_type fast_div_base(data_view_type a,data_view_type b)noexcept{
		no_mod_optimisation_of_div(a,b);
		if(a.size()<=fast_div_base_threshold)return div_base_no_optimisation(a,b);
		const auto [a_after_reg,b_after_reg,muti]=fast_div_regularisation(a,b);
		return fast_divmod_base_impl(a_after_reg,b_after_reg).quot._data;
	}
	static void fast_mod_with_base(data_type&a,data_view_type b)noexcept{
		if(a.size()<=fast_div_base_threshold)return mod_with_base(a,b);
		const auto [a_after_reg,b_after_reg,muti]=fast_div_regularisation(get_data_view_of_data(a),b);
		auto result=fast_divmod_base_impl(a_after_reg,b_after_reg);
		result.mod/=muti;
		a=move(result.mod._data);
	}
	[[nodiscard]]static data_type fast_mod_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<=fast_div_base_threshold)return mod_base(a,b);
		const auto [a_after_reg,b_after_reg,muti]=fast_div_regularisation(a,b);
		auto result=fast_divmod_base_impl(a_after_reg,b_after_reg);
		result.mod/=muti;
		return move(result.mod._data);
	}
	//除法分治：递归
	[[nodiscard]]static divmod_result_t fast_divmod_base_impl(data_view_type a,data_view_type b)noexcept{
		if(compare(a,b)<0)return {ubigint{},ubigint{a}};
		if(a.size()<=fast_div_base_threshold)return divmod_base(a,b);
		size_t base = (b.size()+1) / 2;
		//符合3/2时，进行试商
		if(a.size() <= base*3) {
			base = (b.size()-1) / 2;
			auto a_high = a.subview(base);//不需要re_shrink：subview是舍弃低位，下同
			auto b_high = b.subview(base);
			//数值优化，这意味着余数不可用（下方的remain确实被舍弃了所以可以这样优化）
			no_mod_optimisation_of_div(a_high,b_high);
			auto result=fast_divmod_base_impl(a_high, b_high);
			auto&ans_base=result.quot._data;auto&remain=result.mod._data;
			remain=fast_muti_base(ans_base,b);
			while (compare(remain, a) > 0) {
				sub_with_base(remain, b);
				sub_one_from_base(ans_base);
			}
			remain=sub_base(a,remain);
			return result;
		}
		//不符合3/2时，进行递归
		//选择合适的base长度做分割
		if(a.size() > base*4)
			base = a.size() / 2;
		const auto a_high = a.subview(base);
		auto result=fast_divmod_base_impl(a_high, b);
		auto&ans_base=result.quot._data;auto&remain=result.mod._data;
		ans_base.insert(0,base,base_type{0});
		data_type m{note::size(base + remain.size())};
		copy_assign[base](note::from(a.data()),note::to(m.data()));
		copy_assign[remain.size()](add_const(remain.data()),m.data()+base);
		//这里不需要对m进行shrink，因为remain是已经shrink过的
		{
			auto another_result=fast_divmod_base_impl(m, b);
			add_to_base(ans_base, move(another_result.quot._data));
			remain=move(another_result.mod._data);
		}
		return result;
	}
	[[nodiscard]]static divmod_result_t fast_divmod_base_impl(const data_type&a,data_view_type b)noexcept{
		return fast_divmod_base_impl(get_data_view_of_data(a),b);
	}
	[[nodiscard]]static divmod_result_t fast_divmod_base_impl(const data_type&a,const data_type&b)noexcept{
		return fast_divmod_base_impl(get_data_view_of_data(a),get_data_view_of_data(b));
	}
	[[nodiscard]]static divmod_result_t fast_divmod_base_impl(const ubigint&a,const ubigint&b)noexcept{
		return fast_divmod_base_impl(a._data,b._data);
	}
public:
	//friend abs
	[[nodiscard]]friend ubigint abs(const ubigint& a)noexcept{
		return a;
	}
	[[nodiscard]]friend ubigint&& abs(ubigint&& a)noexcept{
		return move(a);
	}
	//friend is_negative
	[[nodiscard]]friend bool is_negative(const ubigint&)noexcept{
		return false;
	}
	//friend to_size_t
	[[nodiscard]]friend size_t to_size_t(const ubigint& a)noexcept{
		return a.convert_to<size_t>();
	}
	//operator+
	[[nodiscard]]ubigint operator+(const ubigint& other)const&noexcept{
		return ubigint{add_base(get_data_view(), other.get_data_view())};
	}
	//operator-
	[[nodiscard]]ubigint operator-(const ubigint& other)const&noexcept{
		const auto this_view = get_data_view();
		const auto other_view = other.get_data_view();
		if(compare(this_view, other_view)<0)
			return ubigint{};
		return ubigint{sub_base(this_view, other_view)};
	}
	//operator*
	[[nodiscard]]ubigint operator*(const ubigint& other)const&noexcept{
		const auto this_view = get_data_view();
		const auto other_view = other.get_data_view();
		return ubigint{fast_muti_base(this_view, other_view)};
	}
	//operator/
	[[nodiscard]]ubigint operator/(const ubigint& other)const&noexcept{
		const auto other_view = other.get_data_view();
		if(other_view.empty())return ubigint{};
		const auto this_view = get_data_view();
		if(this_view.size() < other_view.size())return ubigint{};
		return ubigint{fast_div_base(this_view, other_view)};
	}
	//operator%
	[[nodiscard]]ubigint operator%(const ubigint& other)const&noexcept{
		const auto other_view = other.get_data_view();
		if(other_view.empty())return ubigint{};
		const auto this_view = get_data_view();
		if(this_view.size() < other_view.size())return*this;
		return ubigint{fast_mod_base(this_view, other_view)};
	}
	//friend divmod
	[[nodiscard]]friend divmod_result_t divmod(const ubigint& a,const ubigint& b)noexcept{
		const auto b_view = b.get_data_view();
		if(b_view.empty())return {};
		const auto a_view = a.get_data_view();
		if(a_view.size() < b_view.size())return {ubigint{},a};
		return fast_divmod_base(a_view,b_view);
	}
	[[nodiscard]]friend divmod_result_t divmod(ubigint&& a,const ubigint& b)noexcept{
		const auto b_view = b.get_data_view();
		if(b_view.empty())return {};
		const auto a_view = a.get_data_view();
		if(a_view.size() < b_view.size())return {ubigint{},move(a)};
		if(a_view.size()<=fast_div_base_threshold)//若不是很大的数
			return divmod_with_base(a._data,b_view);//直接使用a的内存而避免不必要的分配
		else
			return fast_divmod_base(a_view,b_view);
	}
	//operator<<
	template<integer_type T>
	[[nodiscard]]ubigint operator<<(T n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret<<=n;
		return aret;
	}
	//operator>>
	template<integer_type T>
	[[nodiscard]]ubigint operator>>(T n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret>>=n;
		return aret;
	}
	//operator+=
	ubigint& operator+=(const ubigint& other)&noexcept{
		//using add_to_base to avoid new alloc
		add_to_base(_data,other._data);
		return*this;
	}
	template<unsigned_basic_integer_type T>
	ubigint& operator+=(T other)&noexcept{
		//using add_to_base to avoid new alloc
		add_to_base(_data,other);
		return*this;
	}
	template<unsigned_basic_integer_type T>
	[[nodiscard]]ubigint& operator+(T&&other)&&noexcept{
		//using add_to_base to avoid new alloc
		add_to_base(_data,other);
		return*this;
	}
	//operator-=
	ubigint& operator-=(const ubigint& other)&noexcept{
		const auto this_view = get_data_view();
		const auto other_view = other.get_data_view();
		if(compare(this_view, other_view)<0)
			_data.clear();
		else
			sub_with_base(_data,other_view);//already shrink_to_fit ed
		return*this;
	}
	//operator*=
	ubigint& operator*=(const ubigint& other)&noexcept{
		//as muti always need new alloc, so we use just use operator*.
		*this = *this * other;
		return*this;
	}
	//operator/=
	ubigint& operator/=(const ubigint& other)&noexcept{
		//as div always need new alloc, so we use just use operator/.
		*this = *this / other;
		return*this;
	}
	//operator%=
	ubigint& operator%=(const ubigint& other)&noexcept{
		//using mod_with_base to avoid new alloc
		const auto other_view = other.get_data_view();
		if(other_view.empty()){
			_data.clear();
			return*this;
		}
		const auto this_view = get_data_view();
		if(this_view.size() < other_view.size())return*this;
		fast_mod_with_base(_data,other_view);
		return*this;
	}
	//operator<<=
	template<integer_type T>
	inline ubigint& operator<<=(T n)&noexcept{
		if(!*this)return*this;
		if constexpr(unsigned_type<T>){
			const auto oldsize=_data.size();
			const auto newsize_diff=to_size_t(n/bitnum_of(base_type));
			const auto newsize=oldsize+newsize_diff;
			if(newsize_diff){
				_data.insert(0,newsize_diff,base_type{0});
				n%=bitnum_of(base_type);
			}
			if(n){
				auto i=newsize_diff;
				const auto end=newsize;
				const auto offset=to_size_t(n);
				const auto another_offset=bitnum_of(base_type)-offset;
				base_type carry=0;
				for(;i<end;++i){
					const auto tmp=_data[i];
					_data[i]=(tmp<<offset)|carry;
					carry=tmp>>another_offset;
				}
				if(carry)_data.push_back(carry);
			}
			return*this;
		}else{
			if(n<0)return*this>>=abs(n);
			else return*this<<=abs(n);
		}
	}
	//operator>>=
	template<integer_type T>
	inline ubigint& operator>>=(T n)&noexcept{
		if(!*this)return*this;
		if constexpr(unsigned_type<T>){
			const auto oldsize=_data.size();
			const auto newsize_diff=to_size_t(n/bitnum_of(base_type));
			const auto newsize=oldsize-newsize_diff;
			if(newsize_diff){
				if(newsize_diff>=oldsize)return*this=zero;
				_data.forward_resize(newsize);
				n%=bitnum_of(base_type);
			}
			if(n){
				auto i=newsize;
				const auto offset=to_size_t(n);
				const auto another_offset=bitnum_of(base_type)-offset;
				base_type carry=0;
				for(;i--;){
					const auto tmp=_data[i];
					_data[i]=(tmp>>offset)|carry;
					carry=tmp<<another_offset;
				}
				shrink_to_fit();
			}
			return*this;
		}else{
			if(n<0)return*this<<=abs(n);
			else return*this>>=abs(n);
		}
	}
	//operatorX for rvalue
	template<unsigned_integer_type T>
	[[nodiscard]]ubigint&& operator+(T&&n)&&noexcept{
		return move(*this+=forward<T>(n));
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigint&& operator-(T&&n)&&noexcept{
		return move(*this-=forward<T>(n));
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigint&& operator*(T&&n)&&noexcept{
		return move(*this*=forward<T>(n));
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigint&& operator/(T&&n)&&noexcept{
		return move(*this/=forward<T>(n));
	}
	template<unsigned_integer_type T>
	[[nodiscard]]ubigint&& operator%(T&&n)&&noexcept{
		return move(*this%=forward<T>(n));
	}
	template<integer_type T>
	[[nodiscard]]ubigint&& operator<<(T&&n)&&noexcept{
		return move(*this<<=n);
	}
	template<integer_type T>
	[[nodiscard]]ubigint&& operator>>(T&&n)&&noexcept{
		return move(*this>>=n);
	}
	//operator!
	[[nodiscard]]bool operator!()const noexcept{
		return get_data_view().empty();
	}
	[[nodiscard]]explicit operator bool()const noexcept{
		return !operator!();
	}
	//operator++
	ubigint& operator++()&noexcept{
		add_one_to_base();
		return*this;
	}
	[[nodiscard]]ubigint operator++(int)&noexcept{
		auto tmp = *this;
		++*this;
		return tmp;
	}
	//operator--
	ubigint& operator--()&noexcept{
		sub_one_from_base();
		return*this;
	}
	[[nodiscard]]ubigint operator--(int)&noexcept{
		auto tmp = *this;
		--*this;
		return tmp;
	}
	//friend countr_zero
	[[nodiscard]]friend size_t countr_zero(const ubigint& x)noexcept{
		size_t aret=0;
		auto i=x._data.begin();
		const auto end=x._data.end();
		while(i!=end){
			if(!*i)
				aret+=bitnum_of(base_type);
			else{
				aret+=countr_zero(*i);
				break;
			}
			++i;
		}
		return aret;
	}
	//bit_at
	[[nodiscard]]bit_type bit_at(size_t n)noexcept{
		const auto reslut=divmod(n,bitnum_of(base_type));
		const auto index=reslut.quot;
		if(index>=_data.size())return false;
		return _data[index]&(1u<<reslut.mod);
	}
	//friend get_bitnum
	[[nodiscard]]friend size_t get_bitnum(const ubigint& x)noexcept{
		auto&data=x._data;
		if(data.empty())return 0;
		return data.size()*bitnum_of(base_type)-countl_zero(data.back());
	}
	//memory_usage
	[[nodiscard]]force_inline size_t memory_usage()const noexcept{
		return _data.size_in_byte();
	}
	//friend is_odd
	[[nodiscard]]friend bool is_odd(const ubigint& x)noexcept{
		return x._data.empty()?false:x._data.front()&1u;
	}
	//friend is_even
	[[nodiscard]]friend bool is_even(const ubigint& x)noexcept{
		return !is_odd(x);
	}
};

template<typename T>
concept ubigint_cvref=type_info<remove_cvref<T>> == type_info<ubigint>;

template<integer_type T,ubigint_cvref ubigint_t> requires(type_info<remove_cvref<T>> != type_info<ubigint>)
[[nodiscard]]inline auto operator+(T&&a,ubigint_t&&b)noexcept{
	return forward<ubigint_t>(b)+forward<T>(a);
}
template<unsigned_integer_type T,ubigint_cvref ubigint_t> requires(type_info<remove_cvref<T>> != type_info<ubigint>)
[[nodiscard]]inline auto operator-(T&&a,ubigint_t&&b)noexcept{
	return remove_cvref<T>(ubigint{forward<T>(a)}-forward<ubigint_t>(b));
}
template<unsigned_integer_type T,ubigint_cvref ubigint_t> requires(type_info<remove_cvref<T>> != type_info<ubigint>)
[[nodiscard]]inline auto operator*(T&&a,ubigint_t&&b)noexcept{
	return forward<ubigint_t>(b)*forward<T>(a);
}
template<integer_type T,ubigint_cvref ubigint_t> requires(type_info<remove_cvref<T>> != type_info<ubigint>)
[[nodiscard]]inline auto operator/(T&&a,ubigint_t&&b)noexcept{
	const bool sign=is_negative(a);
	return copy_as_negative(remove_cvref<T>(ubigint{abs(forward<T>(a))}/forward<ubigint_t>(b)),sign);
}
template<integer_type T,ubigint_cvref ubigint_t> requires(type_info<remove_cvref<T>> != type_info<ubigint>)
[[nodiscard]]inline auto operator%(T&&a,ubigint_t&&b)noexcept{
	const bool sign=is_negative(a);
	return copy_as_negative(remove_cvref<T>(ubigint{abs(forward<T>(a))}%forward<ubigint_t>(b)),sign);
}

BREAK_NAMESPACE
//注入math::arithmetic_type_info_prover
namespace base::math{
	using namespace bignum_n;
	inline ubigint arithmetic_type_info_prover<ubigint>::min()noexcept{
		return ubigint{};
	}
}
INTER_NAMESPACE(bignum_n)

//file_end

