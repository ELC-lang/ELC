//ubigint.hpp
//at namespace elc::defs::bignum_n
/*
未完成的elc解释器bignum文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class ubigint{
	#if defined(_DEBUG)
		typedef unsigned char base_type;
	#else
		typedef unsigned_specific_size_fast_t<BIT_POSSIBILITY> base_type;
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
	[[nodiscard]]static data_type add_base(data_view_type a,data_view_type b)noexcept{
		if(a.size()<b.size())
			swap(a,b);
		auto base_size = a.size();
		const auto size = base_size+1;

		array_t<base_type> tmp(note::size(size));
		copy_assign[base_size](tmp.data(),a.data());
		copy_assign[size-base_size](note::to(tmp.data()+base_size),base_type{0});
		add_to_base(tmp.data(),b);
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
	static void add_to_base(base_type*buf,size_t end_size,T num)noexcept{
		//定义运算类型：若T比calc_type大，则使用T，否则使用calc_type
		using calc_t = conditional<(sizeof(T) > sizeof(calc_type)),T,calc_type>;
		//首先判断第一个数是否溢出
		{
			const calc_t result = calc_t(*buf)+num;
			const bool is_overflows = result<num;
			if(is_overflows){
				//若T不是uintmax_t，提升到uintmax_t并递归
				if constexpr(type_info<T>!=type_info<uintmax_t>)
					add_to_base(buf,end_size,uintmax_t(num));
				else{
					ubigint tmp=num;
					add_to_base(buf,tmp.get_data_view());
				}
				return;
			}
		}
		//现在确保了第一个数不会溢出：放心进行加法
		for(size_t i=0;i<end_size && num;++i){
			num+=*buf;
			*buf=base_type(num);
			num>>=bitnum_of(base_type);
			++buf;
		}
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
		array_t<base_type> tmp(note::size(a.size()+b.size()),0);
		muti_with_base(tmp.data(),a,b);
		shrink_to_fit(tmp);
		return tmp;
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
		auto this_view = get_data_view();
		auto other_view = other.get_data_view();
		if(this_view.size() < other_view.size())//大数在前循环数小
			swap(this_view, other_view);
		return ubigint{muti_base(this_view, other_view)};
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
		auto origin_size = this_view.size();
		auto new_size = max(origin_size,other_view.size())+1;
		auto size_diff = new_size - origin_size;
		_data.resize(new_size);
		copy_assign[size_diff](base_type{0},note::to(_data.data()+origin_size));
		add_to_base(_data.data(),other_view);
		shrink_to_fit();
		return*this;
	}
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	ubigint& operator+=(T other)&noexcept{
		//using add_to_base to avoid new alloc
		auto origin_size = _data.size();
		const auto size_diff = lambda_with_catch(&)()->size_t{
			const auto back_bits_not_using=countl_zero(_data.back());
			const auto bits_other_not_using=countl_zero(other);
			const auto bitnum_other=bitnum_of(T)-bits_other_not_using;
			const auto bitnum_now=bitnum_of(base_type)*origin_size-back_bits_not_using;
			if(bitnum_other>bitnum_now)
				return sizeof(T)/sizeof(base_type)+(bits_other_not_using?0:1);
			return back_bits_not_using?0:1;
		}();
		if(size_diff){
			const auto new_size = origin_size + size_diff;
			_data.resize(new_size);
			copy_assign[size_diff](base_type{0},note::to(_data.data()+origin_size));
		}
		add_to_base(_data.data(),origin_size,other);
		if(size_diff)
			shrink_to_fit();
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
		return *this+=1u;
	}
	[[nodiscard]]ubigint operator++(int)&noexcept{
		auto tmp = *this;
		++*this;
		return tmp;
	}
	//operator--
	ubigint& operator--()&noexcept{
		return *this-=1u;
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
};

//file_end

