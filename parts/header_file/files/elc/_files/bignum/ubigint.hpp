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
	#if defined(_DEBUG) && !defined(ELC_SPEED_TEST)
		typedef unsigned char base_type;
	#else
		typedef unsigned_specific_size_fast_t<sizeof(uintmax_t)/2> base_type;
	#endif
	typedef array_t<base_type> data_type;
	static constexpr auto base_type_mod=number_of_possible_values_per<base_type>;
	data_type _data;

	explicit ubigint(const data_type&data)noexcept:_data(data){}
	explicit ubigint(data_type&&data)noexcept:_data(move(data)){}
public:
	ubigint()noexcept = default;
	ubigint(const ubigint&)noexcept = default;
	ubigint(ubigint&&)noexcept = default;
	ubigint(const zero_t&)noexcept:ubigint(){}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	ubigint(T value)noexcept{
		constexpr auto size = sizeof(T)/sizeof(base_type)+((sizeof(T)%sizeof(base_type))?1:0);
		_data.resize(size);
		auto i=_data.begin();
		while(value){
			*i=base_type(value%base_type_mod);
			value=T(value/base_type_mod);
			i++;
		}
		const auto used_size=i-_data.begin();
		if(used_size!=size)
			_data.resize(used_size);
	}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	[[nodiscard]]bool is_safe_convert_to()const noexcept{
		constexpr auto min_value=min(type_info<T>);
		constexpr auto max_value=max(type_info<T>);
		if constexpr(min_value)
			if(*this<min_value)
				return false;
		if(*this>max_value)
			return false;
		return true;
	}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	[[nodiscard]]T convert_to()const noexcept{
		T value=0;
		auto i=_data.rbegin();
		while(i!=_data.rend()){
			value=T(value*base_type_mod);
			value+=*i++;
		}
		return value;
	}

	ubigint& operator=(const ubigint&)&noexcept = default;
	ubigint& operator=(ubigint&&)&noexcept = default;
	ubigint& operator=(const zero_t&)noexcept{
		_data.clear();
		return*this;
	}

	~ubigint() = default;
private:
	typedef array_like_view_t<const base_type> data_view_type;
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
	template<typename T> requires ::std::is_integral_v<T>
	[[nodiscard]]bool operator==(T other)const noexcept{
		if constexpr(::std::is_signed_v<T>){
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
public:
	[[nodiscard]]auto operator<=>(const ubigint& other)const noexcept{
		return compare(get_data_view(),other.get_data_view());
	}
	template<typename T> requires ::std::is_integral_v<T>
	[[nodiscard]]auto operator<=>(T other)const noexcept{
		if constexpr(::std::is_signed_v<T>){
			if(is_negative(other))return strong_ordering::greater;
			return *this<=>to_unsigned_t<T>(other);
		}else{
			auto i=_data.begin();
			const auto end=_data.end();
			while(i!=end){
				if(other<*i)//this>other
					return strong_ordering::greater;
				other-=*i;
				other/=base_type_mod;
				i++;
			}
			return other==0?strong_ordering::equivalent:strong_ordering::less;
		}
	}
	//调试用，返回一个可以输出到流的内部结构查看器
	[[nodiscard]]auto get_data_viewr()const noexcept{
		return lambda_with_catch(this)(auto&stream)noexcept->auto&{
			for(auto& i: _data)
				stream << i << U' ';
			return stream;
		};
	}
private:
	//operator+-*/%s
	typedef unsigned_specific_size_t<BIT_POSSIBILITY*sizeof(base_type)> calc_type;
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
			return res>>bitnum_of(base_type);
		}
	}
	[[nodiscard]]static size_t get_safety_add_buf_size(data_view_type a,data_view_type b)noexcept{
		return a.size()+get_safety_add_buf_size_diff(a,b);
	}
	[[nodiscard]]static size_t get_safety_add_buf_size_with_not_copmared_buf(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())
			swap(a,b);
		return get_safety_add_buf_size(a,b);
	}
	[[nodiscard]]static data_type add_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())
			swap(a,b);
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
	static void add_to_base(base_type*buf,data_view_type b)noexcept{
		bool is_overflows = 0;
		const auto end_size = b.size();
		auto other_begin = b.begin();

		for(size_t i=0;i<end_size;++i){
			calc_type result = calc_type(*buf)+calc_type(*other_begin)+calc_type(is_overflows);
			is_overflows = result>>bitnum_of(base_type);
			*buf = base_type(result);

			++buf;
			++other_begin;
		}
		while(is_overflows){
			calc_type result = calc_type(*buf)+calc_type(is_overflows);
			is_overflows = result>>bitnum_of(base_type);
			*buf = base_type(result);

			++buf;
		}
	}
	static void add_to_base(base_type*buf,const data_type&b)noexcept{
		add_to_base(buf,get_data_view_of_data(b));
	}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	static void add_to_base(data_type&buf,T num)noexcept{
		//定义运算类型：若T比calc_type大，则使用T，否则使用calc_type
		using calc_t = conditional<(sizeof(T) > sizeof(calc_type)),T,calc_type>;
		//初始化
		calc_t tmp=num;
		base_type*intr=buf.begin();
		const auto end=buf.end();
		bool is_overflows = false;
		while(intr!=end && tmp){
			calc_t result = calc_t(*intr)+tmp+calc_t(is_overflows);
			is_overflows = result <= tmp;
			tmp = result>>bitnum_of(base_type);
			*intr=base_type(result);
			++intr;
		}
		if(tmp){
			{
				const auto intr_pos=intr-buf.begin();
				buf.resize(buf.size()+sizeof(tmp));
				intr=buf.begin()+intr_pos;
			}
			while(tmp){
				*intr=base_type(tmp)+calc_t(is_overflows);
				is_overflows = is_overflows && *intr==0;
				tmp>>=bitnum_of(base_type);
				++intr;
			}
			buf.resize(intr-buf.begin());
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
			buf.resize(size_need);
			copy_assign[size_diff](note::to(buf.data()+size_now),base_type{0});//填充0
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
		auto base_size = a.size();
		const auto size = base_size;

		array_t<base_type> tmp(note::size(size));
		copy_assign[base_size](tmp.data(),a.data());
		copy_assign[size-base_size](note::to(tmp.data()+base_size),base_type{0});
		sub_with_base(tmp.data(),b);
		shrink_to_fit(tmp);
		return tmp;
	}
	static void sub_with_base(base_type*buf,data_view_type b)noexcept{
		//调用方保证a>=b
		bool is_overflows = 0;
		const auto end_size = b.size();
		auto other_begin = b.begin();

		for(size_t i=0;i<end_size;++i){
			auto t_a=calc_type(*buf);
			auto t_b=calc_type(*other_begin)+calc_type(is_overflows);
			is_overflows=t_a<t_b;
			if(is_overflows)
				t_a+=base_type_mod;
			*buf = base_type(t_a-t_b);

			++other_begin;
			++buf;
		}
		while(is_overflows){
			auto t_a=calc_type(*buf);
			auto t_b=calc_type(is_overflows);
			is_overflows=!t_a;
			if(is_overflows)
				t_a+=base_type_mod;
			*buf = base_type(t_a-t_b);

			++buf;
		}
	}
	//++
	//add_one_to_base
	static void add_one_to_base(data_type&buf)noexcept{
		base_type*intr=buf.begin();
		const auto end=buf.end();
		while(intr!=end){
			++*intr;
			if(*intr)
				return;
			++intr;
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
		base_type*intr=buf.begin();
		const auto end=buf.end();
		while(intr!=end){
			if(*intr){
				--*intr;
				return;
			}
			*intr=max(type_info<base_type>);
		}
		//整个buf都是0，说明buf是0，不需要做任何操作
	}
	void sub_one_from_base()noexcept{
		sub_one_from_base(_data);
	}
	//*
	[[nodiscard]]static data_type muti_base(data_view_type a,base_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		//下面的muti_with_base至少会写入a.size()个元素，所以只需要置零tmp中最后一个元素就行
		tmp.back()=0;
		muti_with_base(tmp.data(),a,b);
		shrink_to_fit(tmp);
		return tmp;
	}
	static void muti_with_base(base_type*buf,data_view_type a,base_type b)noexcept{
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
	}
	static void muti_with_base(base_type*buf,data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		size_t muti_sacle=0;
		while(muti_sacle!=b.size()){
			//下面的muti_with_base至少会写入a.size()个元素，所以只需要置零tmp中最后一个元素就行
			tmp.back()=0;
			muti_with_base(tmp.data(),a,b[muti_sacle]);
			add_to_base(buf+muti_sacle,get_shrinked_data_view_of_data(tmp));
			muti_sacle++;
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
	///
	static base_type div_with_base(data_type&buf,base_type*a,data_view_type b)noexcept{
		data_view_type tryto{a,b.size()+1};
		const calc_type dividend=[&](){
			const base_type*p=tryto.rbegin();
			auto tmp=calc_type(*p);tmp*=base_type_mod;tmp+=calc_type(p[-1]);
			return tmp;
		}();
		const calc_type divisor=calc_type(b.back());
		calc_type left=dividend/(divisor+1);
		calc_type right=min(calc_type(dividend/divisor),(calc_type)max(type_info<base_type>));
		base_type last_work_able=0;
		//left<=a/b<=right
		tryto=get_shrinked_data_view_of_data(tryto.data(),tryto.size());
		while(left<=right) {
			//下面的muti_with_base至少会写入b.size()个元素，所以只需要置零buf中最后一个元素就行
			buf.back()=0;
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
		if(last_work_able==0)
			return 0;
		//下面的muti_with_base至少会写入b.size()个元素，所以只需要置零buf中最后一个元素就行
		buf.back()=0;
		muti_with_base(buf.data(),b,last_work_able);
		sub_with_base(a,get_shrinked_data_view_of_data(buf));
		return last_work_able;
	}
	[[nodiscard]]static base_type div_base(base_type*a,data_view_type b)noexcept{
		array_t<base_type> fortry(note::size(b.size()+1));
		return div_with_base(fortry,a,b);
	}
	[[nodiscard]]static data_type div_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		copy_assign[a.size()](tmp.data(), a.data());
		tmp.back()=0;
		tmp=div_with_base(tmp,b);
		return tmp;
	}
	static data_type div_with_base(data_type&a,data_view_type b)noexcept{
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
	//%
	static void mod_with_base(data_type&a,data_view_type b)noexcept{
		const auto end=a.rend();
		auto begin=a.rbegin()+b.size();
		array_t<base_type> fortry(note::size(b.size()+1));
		while(begin!=end){
			div_with_base(fortry,begin,b);
			begin++;
		};
	}
	[[nodiscard]]static data_type mod_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		copy_assign[a.size()](tmp.data(), a.data());
		tmp.back()=0;
		mod_with_base(tmp,b);
		shrink_to_fit(tmp);
		return tmp;
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
		return ubigint{div_base(this_view, other_view)};
	}
	//operator%
	[[nodiscard]]ubigint operator%(const ubigint& other)const&noexcept{
		const auto other_view = other.get_data_view();
		if(other_view.empty())return ubigint{};
		const auto this_view = get_data_view();
		if(this_view.size() < other_view.size())return*this;
		return ubigint{mod_base(this_view, other_view)};
	}
	//friend divmod
	[[nodiscard]]friend auto divmod(const ubigint& a,const ubigint& b)noexcept{
		struct divmod_result{
			ubigint quot;
			ubigint mod;
		};
		const auto b_view = b.get_data_view();
		if(b_view.empty())return divmod_result{};
		const auto a_view = a.get_data_view();
		if(a_view.size() < b_view.size())return divmod_result{ubigint{},a};
		{
			array_t<base_type> mod(note::size(a_view.size() + 1));
			copy_assign[a_view.size()](mod.data(), a_view.data());
			mod.back()=0;
			array_t<base_type> quot = div_with_base(mod, b_view);
			shrink_to_fit(mod);
			return divmod_result{ubigint{move(quot)},ubigint{move(mod)}};
		}
	}
	[[nodiscard]]friend auto divmod(ubigint&& a,const ubigint& b)noexcept{
		//直接使用a的内存而避免不必要的分配
		typedef decltype(divmod(a,b)) divmod_result;
		const auto b_view = b.get_data_view();
		if(b_view.empty())return divmod_result{};
		const auto a_view = a.get_data_view();
		if(a_view.size() < b_view.size())return divmod_result{ubigint{},move(a)};
		{
			a._data.push_back(0);
			array_t<base_type> quot = div_with_base(a._data, b_view);
			shrink_to_fit(a._data);
			return divmod_result{ubigint{move(quot)},move(a)};
		}
	}
	//operator<<
	template<typename T> requires ::std::integral<T>
	[[nodiscard]]ubigint operator<<(T n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret<<=n;
		return aret;
	}
	[[nodiscard]]ubigint operator<<(ubigint n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret<<=n;
		return aret;
	}
	//operator>>
	template<typename T> requires ::std::integral<T>
	[[nodiscard]]ubigint operator>>(T n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret>>=n;
		return aret;
	}
	[[nodiscard]]ubigint operator>>(ubigint n)const&noexcept{
		if(!*this)return ubigint{};
		auto aret=*this;
		aret>>=n;
		return aret;
	}
	//operator+=
	ubigint& operator+=(const ubigint& other)&noexcept{
		//using add_to_base to avoid new alloc
		const auto this_view = get_data_view();
		const auto other_view = other.get_data_view();
		const auto origin_size = this_view.size();
		const auto new_size = get_safety_add_buf_size_with_not_copmared_buf(this_view,other_view);
		const auto size_diff = new_size - origin_size;
		if(size_diff){
			_data.resize(new_size);
			copy_assign[size_diff](base_type{0},note::to(_data.data()+origin_size));
		}
		add_to_base(_data.data(),other_view);
		if(size_diff)
			shrink_to_fit();
		return*this;
	}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	ubigint& operator+=(T other)&noexcept{
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
		else{
			sub_with_base(_data.data(),other_view);
			shrink_to_fit();
		}
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
		_data.resize(_data.size()+1);
		_data.back()=0;
		mod_with_base(_data,other_view);
		shrink_to_fit();
		return*this;
	}
	//operator<<=
	template<typename T> requires ::std::integral<T>
	inline ubigint& operator<<=(T n)&noexcept{
		if(!*this)return*this;
		if constexpr(::std::is_unsigned_v<T>){
			const auto oldsize=_data.size();
			const auto newsize_diff=n/bitnum_of(base_type);
			const auto newsize=oldsize+newsize_diff;
			if(newsize_diff){
				_data.forward_resize(newsize);
				copy_assign[newsize_diff](note::to(_data.data()), base_type{0});
				n%=bitnum_of(base_type);
			}
			if(n){
				auto i=newsize_diff;
				const auto end=newsize;
				const auto another_offset=bitnum_of(base_type)-n;
				base_type carry=0;
				for(;i<end;++i){
					const auto tmp=_data[i];
					_data[i]=(tmp<<n)|carry;
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
	ubigint& operator<<=(ubigint n)&noexcept{
		if(!*this)return*this;
		const auto oldsize=_data.size();
		const auto newsize_diff=to_size_t(n/bitnum_of(base_type));
		const auto newsize=oldsize+newsize_diff;
		if(newsize_diff){
			_data.forward_resize(newsize);
			copy_assign[newsize_diff](note::to(_data.data()), base_type{0});
			n%=bitnum_of(base_type);
		}
		if(n){
			const auto offset=to_size_t(n);
			auto i=newsize_diff;
			const auto end=newsize;
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
	}
	//operator>>=
	template<typename T> requires ::std::integral<T>
	inline ubigint& operator>>=(T n)&noexcept{
		if(!*this)return*this;
		if constexpr(::std::is_unsigned_v<T>){
			const auto oldsize=_data.size();
			const auto newsize_diff=n/bitnum_of(base_type);
			const auto newsize=oldsize-newsize_diff;
			if(newsize_diff){
				if(newsize_diff>=oldsize)return*this=zero;
				_data.forward_resize(newsize);
				n%=bitnum_of(base_type);
			}
			if(n){
				auto i=newsize;
				const auto another_offset=bitnum_of(base_type)-n;
				base_type carry=0;
				for(;i--;){
					const auto tmp=_data[i];
					_data[i]=(tmp>>n)|carry;
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
	ubigint& operator>>=(ubigint n)&noexcept{
		if(!*this)return*this;
		const auto oldsize=_data.size();
		const auto newsize_diff=to_size_t(n/bitnum_of(base_type));
		const auto newsize=oldsize-newsize_diff;
		if(newsize_diff){
			if(newsize_diff>=oldsize)return*this=zero;
			_data.forward_resize(newsize);
			n%=bitnum_of(base_type);
		}
		if(n){
			const size_t offset=to_size_t(n);
			auto i=newsize;
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
	}
	//operatorX for rvalue
	[[nodiscard]]ubigint&& operator+(const ubigint& other)&&noexcept{
		return move(*this+=other);
	}
	[[nodiscard]]ubigint&& operator-(const ubigint& other)&&noexcept{
		return move(*this-=other);
	}
	[[nodiscard]]ubigint&& operator*(const ubigint& other)&&noexcept{
		return move(*this*=other);
	}
	[[nodiscard]]ubigint&& operator/(const ubigint& other)&&noexcept{
		return move(*this/=other);
	}
	[[nodiscard]]ubigint&& operator%(const ubigint& other)&&noexcept{
		return move(*this%=other);
	}
	template<typename T> requires ::std::integral<T>
	[[nodiscard]]ubigint&& operator<<(T n)&&noexcept{
		return move(*this<<=n);
	}
	[[nodiscard]]ubigint&& operator<<(const ubigint& other)&&noexcept{
		return move(*this<<=other);
	}
	template<typename T> requires ::std::integral<T>
	[[nodiscard]]ubigint&& operator>>(T n)&&noexcept{
		return move(*this>>=n);
	}
	[[nodiscard]]ubigint&& operator>>(const ubigint& other)&&noexcept{
		return move(*this>>=other);
	}
	[[nodiscard]]ubigint&& operator+(ubigint&& other)noexcept{
		return move(other+=*this);
	}
	/*
	[[nodiscard]]ubigint&& operator-(ubigint&& other)noexcept{
		return move((-other)+*this);
	}
	*/
	[[nodiscard]]ubigint&& operator*(ubigint&& other)noexcept{
		return move(other*=*this);
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
	//friend get_bitnum
	[[nodiscard]]friend size_t get_bitnum(const ubigint& x)noexcept{
		auto&data=x._data;
		if(data.empty())return 0;
		return data.size()*bitnum_of(base_type)-::std::countl_zero(data.back());
	}
	//friend is_odd
	[[nodiscard]]friend bool is_odd(const ubigint& x)noexcept{
		return x._data.empty()?false:x._data.front()&1u;
	}
	//friend is_even
	[[nodiscard]]friend bool is_even(const ubigint& x)noexcept{
		return !is_odd(x);
	}
	//friend pow
	[[nodiscard]]friend ubigint pow(ubigint x,ubigint y)noexcept{
		ubigint aret=1u;
		while(y){
			if(is_odd(y))aret*=x;
			x*=x;
			y>>=1u;
		}
		return aret;
	}
	template<typename T> requires(::std::integral<T> && ::std::is_unsigned_v<T>)
	[[nodiscard]]friend ubigint pow(ubigint x,T y)noexcept{
		ubigint aret=1u;
		while(y){
			if(y&1u)aret*=x;
			x*=x;
			y>>=1u;
		}
		return aret;
	}
};

//file_end

