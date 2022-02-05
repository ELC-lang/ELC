//value.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct value:non_copy_assign_able{
	typedef value this_t;
	struct base_data_t:type_info_t<base_data_t>::template_name
	with_common_attribute<abstract_base,ref_able,never_in_array,replace_able>,
	build_by_get_only{
		/*//COMMIT:
		是否应当支持value data的引用计数？
		否的原因：
			这将导致应当对value的data加入gc机制（否则容易出现闭环value垃圾！）
			而这将严重增加elc的运行时时空负担
			有人可能得到递归引用的value
		是的原因：
			防御式编程
			更佳多样的value实现
			引用计数以降低value的复制开销
		对否定想法的驳回：
			严重增加elc的运行时时空负担？：value的复制开销可能更大
			有人可能得到递归引用的value？这将导致应当对value的data加入gc机制？：
				没见哪个语言不允许写while(true)
				自定义value的应该都是聪明人，如果真的会出现循环引用那他们自己解决就好
		总结：
			加引用计数，但不加gc
			elc的设计理念包括“不应禁止用户做什么”，所以应当提供高自由度的value定制可能
			但出于性能考虑，不加gc
			如果真的有需要gc解决的value data那么让那聪明的设计者自己给自己的data类加gc解决这种问题就好
		*/
		base_data_t()noexcept=default;
		base_data_t(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }
		virtual ~base_data_t()noexcept=default;

		virtual void be_set(ptr)=0;
		[[nodiscard]]virtual ptr get_value()=0;
		[[nodiscard]]virtual base_data_t*copy()const noexcept=0;
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept=0;//为什么要加这个？我不知道，万一将来有人用上了呢？
	};

	distinctive inline static struct null_data_t final:instance_struct<null_data_t>
	,value::base_data_t{
		null_data_t():value::base_data_t(never_ref_num_zero){}
		virtual ~null_data_t()noexcept override final=default;

		virtual void be_set(ptr a)noexcept override final{}
		[[nodiscard]]virtual ptr get_value()override final{return null_ptr;}
		[[nodiscard]]virtual base_data_t*copy()const noexcept override final{return remove_const(this);}
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<null_data_t>;}
	}null_data{};

	struct constexpr_data_t final:instance_struct<constexpr_data_t>
	,base_data_t{
		ptr _m;
		constexpr_data_t(ptr a):_m(a){}
		constexpr_data_t(const constexpr_data_t&)noexcept=default;
		virtual ~constexpr_data_t()noexcept override final=default;

		virtual void be_set(ptr)noexcept override final{}
		[[nodiscard]]virtual ptr get_value()override final{return _m;}
		[[nodiscard]]virtual base_data_t*copy()const noexcept override final{return get<constexpr_data_t>(_m);}
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<constexpr_data_t>;}
	};

	struct variable_data_t final:instance_struct<variable_data_t>
	,base_data_t{
		ptr _m;
		variable_data_t(ptr a):_m(a){}
		variable_data_t(const variable_data_t&)noexcept=default;
		virtual ~variable_data_t()noexcept override final=default;

		virtual void be_set(ptr a)noexcept override final{_m=a;}
		[[nodiscard]]virtual ptr get_value()override final{return _m;}
		[[nodiscard]]virtual base_data_t*copy()const noexcept override final{return get<variable_data_t>(_m);}
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<variable_data_t>;}
	};
private:
	mutable comn_ptr_t<base_data_t> _m;
public:
	explicit value():_m(get<variable_data_t>(null_ptr)){}
	explicit value(special_init_t):_m(null_ptr){}
	explicit value(ptr a):_m(get<constexpr_data_t>(a)){}
	explicit value(node_like* a):value(ptr(a)){}
	value(base_data_t*a)noexcept:_m(a){}

	value(const this_t&a)noexcept=default;
	value(this_t&&a)noexcept=default;
	~value()noexcept=default;

	this_t&operator=(ptr a){
		_m->be_set(a);
		return *this;
	}
	template<typename T>
	this_t&operator=(T&&a){
		return operator=(as_ptr(a));
	}

	[[nodiscard]]operator ptr(){
		return _m->get_value();
	}
	[[nodiscard]]operator const_ptr()const{
		return _m->get_value();
	}
	[[nodiscard]]ptr operator&(){
		return operator ptr();
	}
	[[nodiscard]]const_ptr operator&()const{
		return operator const_ptr();
	}
	[[nodiscard]]explicit operator bool()const{return bool(_m->get_value());}

	void ref_to(this_t a){
		_m.do_replace(a._m);
	}
	void un_ref(){
		_m=_m->copy();
	}
	void re_ref_to(this_t a){
		_m=a._m;
	}
	this_t& operator>>(this_t a){
		a.ref_to(*this);
		return*this;
	}

	template<typename T>
	[[nodiscard]]auto operator[](T&&index){
		return (*operator&())[forward<T>(index)];
	}
	template<typename...Args> requires(invoke<node_like>.able<Args...>)
	inline auto operator()(Args&&... rest)noexcept(invoke<node_like>.nothrow<Args...>){return(*operator&())(forward<Args>(rest)...);}
	template<typename T>
	[[nodiscard]]auto operator==(T&&a)const{
		return const_ptr(*this) == const_ptr(as_value(a));
	}
	template<typename T>
	[[nodiscard]]auto operator!=(T&&a)const{
		return !operator==(forward<T>(a));
	}
	[[nodiscard]]operator ptr::for_delete_t*(){
		return _m->get_value();
	}
};

static value::base_data_t*the_get_null_ptr(const value::base_data_t*)noexcept{
	return&value::null_data;
}

BREAK_NAMESPACE

INTER_NAMESPACE(base)
template<>
distinctive inline core::value const_default_value_of<core::value>{special_init};
BREAK_NAMESPACE

INTER_NAMESPACE(core)

//file_end

