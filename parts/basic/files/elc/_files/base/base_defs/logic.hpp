//logic.hpp
//at namespace elc::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class logical_bool{
	typedef unsigned char base_t;
	base_t _value;
	//00:false 01:true 10:neither 11:either
	constexpr logical_bool(base_t a):_value(a){}
public:
	constexpr logical_bool(special_init_t,base_t a):_value(a){}
	constexpr logical_bool(bool a):_value(a?0b01:0b00){}
	constexpr logical_bool(const logical_bool&)=default;
	constexpr logical_bool&operator=(const logical_bool&)& =default;
	constexpr explicit operator bool()const{return _value&0b01;}
	constexpr logical_bool operator!()const{return{_value&0b10?_value:base_t(_value^0b01)};}

	[[nodiscard]]explicit /*constexpr*/ operator hash_t(){
		return hash(_value);
	}
};
constexpr logical_bool neither{special_init,0b10},either{special_init,0b11};
