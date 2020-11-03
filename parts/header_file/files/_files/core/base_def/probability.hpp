//defs.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
lazy_instantiation struct LIS_name(probability){
	typedef LIS_ID_t(probability)this_t;
	struct base_probability_t{
		virtual ~base_probability_t()noexcept=default;
		virtual void delete_this()const noexcept=0;
		virtual base_probability_t*copy()const noexcept=0;
	};
	struct no_probability_t:type_info_t<tester>::template
	with_common_attribute<alloc_by_pool>,base_probability_t,build_by_get_only,never_in_array{
		ptr _m;
		no_probability_t(ptr a):_m(a){}
		virtual ~no_probability_t()noexcept override=default;
		virtual void delete_this()const noexcept override{unget(this);}
		virtual base_probability_t*copy()const noexcept override{return copy_get(this);}
	};
private:
	mutable base_probability_t*_m;
public:
	LIS_name(probability)(ptr a):_m(get<no_probability_t>(a)){}
	constexpr LIS_name(probability)(base_probability_t*a)noexcept:_m(a){}
	LIS_name(probability)(const this_t&a)noexcept:_m(a._m->copy()){}
	LIS_name(probability)(this_t&&a)noexcept:_m(a._m){a._m=null_ptr;}
	~LIS_name(probability)()noexcept{
		if(_m!=null_ptr){
			destruct(_m);
			_m->delete_this();
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
lazy_instantiation_name(probability);

//file_end

