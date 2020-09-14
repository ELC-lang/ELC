//logic.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct logical_bool{
	typedef unsigned char base_t;
private:
	base_t _value;
	//01:false 10:true 00:neither 11:either
	constexpr logical_bool(base_t a):_value(a){}
public:
	constexpr logical_bool(special_init_t,base_t a):_value(a){}
	constexpr logical_bool(bool a):_value(a?0b10:0b01){}
	constexpr logical_bool(const logical_bool&)=default;
	constexpr logical_bool&operator=(const logical_bool&)& =default;
	constexpr explicit operator bool()const{return _value&0b10;}
	constexpr logical_bool operator!()const{return base_t(~_value);}

	[[nodiscard]]explicit constexpr operator hash_t(){
		return hash(_value);
	}
	friend logical_bool operator&&(logical_bool a,logical_bool b)noexcept{
		return logical_bool{special_init,logical_bool::base_t(a._value&b._value)};
	}
	friend logical_bool operator||(logical_bool a,logical_bool b)noexcept{
		return logical_bool{special_init,logical_bool::base_t(a._value|b._value)};
	}
};
constexpr logical_bool neither{special_init,0b00},either{special_init,0b11};

//file_end

