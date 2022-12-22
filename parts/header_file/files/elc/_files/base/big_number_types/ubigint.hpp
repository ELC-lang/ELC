//ubigint.hpp
//at namespace elc::defs::big_number_types
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class ubigint{
	typedef unsigned_specific_size_fast_t<BIT_POSSIBILITY> base_type;
	typedef array_t<base_type> data_type;
	static constexpr auto base_type_mod=number_of_possible_values_per<base_type>;
	data_type _data;

	explicit ubigint(const data_type&data)noexcept:_data(data){}
	explicit ubigint(data_type&&data)noexcept:_data(move(data)){}
public:
	ubigint()noexcept = default;
	ubigint(const ubigint&)noexcept = default;
	ubigint(ubigint&&)noexcept = default;
	template<typename T> requires(::std::is_integral_v<T>&&::std::is_unsigned_v<T>)
	ubigint(T value)noexcept{
		const auto size = sizeof(T)/sizeof(base_type)+(sizeof(T)%sizeof(base_type)?1:0);
		_data.resize(size);
		auto i=_data.begin();
		while(value){
			*i=base_type(value%base_type_mod);
			value=T(value/base_type_mod);
			i++;
		}
		const auto end=_data.end();
		while(i!=end){
			*i=0;
			i++;
		}
	}

	ubigint& operator=(const ubigint&)&noexcept = default;
	ubigint& operator=(ubigint&&)&noexcept = default;

	~ubigint() = default;
private:
	typedef array_like_view_t<const base_type> data_view_type;
	static data_view_type get_data_view_of_data(const base_type*data,size_t size)noexcept{
		size_t i = size;
		while(i--)
			if(data[i]!=0)
				break;
		return data_view_type{data,i+1};
	}
	static data_view_type get_data_view_of_data(const data_type&a)noexcept{
		return get_data_view_of_data(a.data(), a.size());
	}
	data_view_type get_data_view()const noexcept{
		return get_data_view_of_data(_data);
	}
public:
	//operator==
	bool operator==(const ubigint& other)const noexcept{
		return equal(get_data_view(),other.get_data_view());
	}
	//operator<=>
	//由于低位在前，不能使用elc的默认compare
	static auto compare(data_view_type a, data_view_type b)noexcept{
		if(const auto tmp=a.size()<=>b.size(); tmp!=0)
			return tmp;

		auto i=a.size();
		while(i--)
			if(const auto tmp=a[i]<=>b[i]; tmp!=0)
				return tmp;
		return strong_ordering::equivalent;
	}
	auto operator<=>(const ubigint& other)const noexcept{
		return compare(get_data_view(),other.get_data_view());
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
	//-
	[[nodiscard]]static data_type sub_base(data_view_type a,data_view_type b)noexcept{
		//调用方保证a>=b
		auto base_size = a.size();
		const auto size = base_size;

		array_t<base_type> tmp(note::size(size));
		copy_assign[base_size](tmp.data(),a.data());
		copy_assign[size-base_size](note::to(tmp.data()+base_size),base_type{0});
		sub_with_base(tmp.data(),b);
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
		return tmp;
	}
	static void muti_with_base(base_type*buf,data_view_type a,base_type b)noexcept{
		auto i=0;
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
			add_to_base(buf+muti_sacle,tmp);
			muti_sacle++;
		}
	}
	[[nodiscard]]static data_type muti_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+b.size()),0);
		muti_with_base(tmp.data(),a,b);
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
		base_type aret=base_type(dividend/divisor);
		tryto=get_data_view_of_data(tryto.data(),tryto.size());
		while(aret!=0){
			//下面的muti_with_base至少会写入b.size()个元素，所以只需要置零buf中最后一个元素就行
			buf.back()=0;
			muti_with_base(buf.data(),b,aret);
			const auto myview=get_data_view_of_data(buf);
			if(compare(tryto,myview)>=0){
				sub_with_base(a,myview);
				return aret;
			}
			else
				aret--;
		}
		return 0;
	}
	[[nodiscard]]static base_type div_base(base_type*a,data_view_type b)noexcept{
		array_t<base_type> fortry(note::size(b.size()+1));
		return div_with_base(fortry,a,b);
	}
	[[nodiscard]]static data_type div_base(data_view_type a,data_view_type b)noexcept{
		array_t<base_type> tmp(note::size(a.size()+1));
		copy_assign[a.size()](tmp.data(), a.data());
		tmp.back()=0;
		return div_with_base(tmp,b);
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
		return*this;
	}
	//operator-=
	ubigint& operator-=(const ubigint& other)&noexcept{
		const auto this_view = get_data_view();
		const auto other_view = other.get_data_view();
		if(compare(this_view, other_view)<0)
			_data.clear();
		else
			sub_with_base(_data.data(),other_view);
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
		mod_with_base(_data,other_view);
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
};

//file_end

