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
	struct base_data_t:type_info_t<tester>::template
	with_common_attribute<abstract_base>{
		virtual ~base_data_t()noexcept=default;
		virtual void be_set(ptr)=0;
		virtual ptr get_value()=0;
		virtual void free_this()const noexcept=0;
		virtual base_data_t*copy()const noexcept=0;
	};
	struct constexpr_data_t:type_info_t<tester>::template
	with_common_attribute<instance_struct,alloc_by_pool>,base_data_t,build_by_get_only,never_in_array{
		ptr _m;
		constexpr_data_t(ptr a):_m(a){}
		virtual ~constexpr_data_t()noexcept override=default;
		virtual void be_set(ptr)noexcept override{}
		virtual ptr get_value()override{return _m;}
		virtual void free_this()const noexcept override{unget(this,not destruct);}
		virtual base_data_t*copy()const noexcept override{return get<constexpr_data_t>(_m);}
	};
private:
	mutable base_data_t*_m;
public:
	explicit LIS_name(setter)(ptr a):_m(get<constexpr_data_t>(a)){}
	constexpr LIS_name(setter)(base_data_t*a)noexcept:_m(a){}
	LIS_name(setter)(const this_t&a)noexcept:_m(a._m->copy()){}
	LIS_name(setter)(this_t&&a)noexcept:_m(a._m){a._m=null_ptr;}
	~LIS_name(setter)()noexcept{
		if(_m!=null_ptr){
			destruct(_m);
			_m->free_this();
		}
	}
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

