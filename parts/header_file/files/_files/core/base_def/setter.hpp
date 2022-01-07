//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
lazy_instantiation struct LIS_name(setter){
	typedef LIS_ID_t(setter)this_t;
	struct base_data_t:type_info_t<base_data_t>::template_name
	with_common_attribute<abstract_base,ref_able>{
		/*//COMMIT:
		是否应当支持setter data的引用计数？
		否的原因：
			这将导致应当对setter的data加入gc机制（否则容易出现闭环setter垃圾！）
			而这将严重增加elc的运行时时空负担
			有人可能得到递归引用的setter
		是的原因：
			防御式编程
			更佳多样的setter实现
			引用计数以降低setter的复制开销
		对否定想法的驳回：
			严重增加elc的运行时时空负担？：setter的复制开销可能更大
			有人可能得到递归引用的setter？这将导致应当对setter的data加入gc机制？：
				没见哪个语言不允许写while(true)
				自定义setter的应该都是聪明人，如果真的会出现循环引用那他们自己解决就好
		总结：
			加引用计数，但不加gc
			elc的设计理念包括“不应禁止用户做什么”，所以应当提供高自由度的setter定制可能
			但出于性能考虑，不加gc
			如果真的有需要gc解决的setter data那么让那聪明的设计者自己给自己的data类加gc解决这种问题就好
		*/
		virtual ~base_data_t()noexcept=default;

		virtual void be_set(ptr)=0;
		[[nodiscard]]virtual ptr get_value()=0;
		[[nodiscard]]virtual base_data_t*copy()const noexcept=0;
		[[nodiscard]]virtual const base_type_info_t& get_type_info()const noexcept=0;//为什么要加这个？我不知道，万一将来有人用上了呢？
	};

	struct constexpr_data_t:type_info_t<constexpr_data_t>::template_name
	with_common_attribute<alloc_by_pool,never_in_array,instance_struct>,base_data_t,build_by_get_only{
		ptr _m;
		constexpr_data_t(ptr a):_m(a){}
		constexpr_data_t(const constexpr_data_t&)noexcept=default;
		virtual ~constexpr_data_t()noexcept override=default;

		virtual void be_set(ptr)noexcept override{}
		[[nodiscard]]virtual ptr get_value()override{return _m;}
		[[nodiscard]]virtual base_data_t*copy()const noexcept override{return get<constexpr_data_t>(_m);}
		[[nodiscard]]virtual const base_type_info_t& get_type_info()const noexcept override{return type_info<constexpr_data_t>;}
	};
private:
	mutable base_data_t*_m;
public:
	explicit LIS_name(setter)(ptr a):_m(get<constexpr_data_t>(a)){}
	explicit LIS_name(setter)(node_like* a):LIS_name(setter)(ptr(a)){}
	constexpr LIS_name(setter)(base_data_t*a)noexcept:_m(a){}

	LIS_name(setter)(const this_t&a)noexcept=default;
	LIS_name(setter)(this_t&&a)noexcept=default;
	~LIS_name(setter)()noexcept=default;

	this_t&operator=(ptr a)&{
		_m->be_set(a);
		return*this;
	}
	operator ptr(){
		return _m->get_value();
	}
	operator const_ptr()const{
		return _m->get_value();
	}
	explicit operator bool()const{return bool(_m->get_value());}
};
lazy_instantiation_name(setter);

//file_end

