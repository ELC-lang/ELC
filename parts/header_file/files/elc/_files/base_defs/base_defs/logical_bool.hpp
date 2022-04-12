//logic.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct logical_bool{
private:
	[[no_unique_address]]bool _is_true;
	[[no_unique_address]]bool _is_false;
	[[no_unique_address]]bool _is_unknown;
public:
	constexpr logical_bool(special_init_t,bool is_true,bool is_false,bool is_unknown=0):_is_true(is_true),_is_false(is_false),_is_unknown(is_unknown){}
	constexpr logical_bool(bool a):_is_true(a),_is_false(!a),_is_unknown(0){}
	constexpr logical_bool(const logical_bool&)=default;
	constexpr logical_bool&operator=(const logical_bool&)& =default;
	constexpr explicit operator bool()const{return _is_true && !_is_unknown;}
	constexpr logical_bool operator!()const{
		logical_bool aret=*this;
		swap(aret._is_true,aret._is_false);
		return aret;
	}
	friend logical_bool operator&&(logical_bool a,logical_bool b)noexcept{
		return logical_bool{special_init,a._is_true && b._is_true,a._is_false || b._is_false,a._is_unknown || b._is_unknown};
	}
	friend logical_bool operator||(logical_bool a,logical_bool b)noexcept{
		return logical_bool{special_init,a._is_true || b._is_true,a._is_false && b._is_false,a._is_unknown || b._is_unknown};
	}
	friend logical_bool operator==(logical_bool a,logical_bool b)noexcept{
		logical_bool aret(a._is_true == b._is_true && a._is_false == b._is_false);
		aret._is_unknown = a._is_unknown || b._is_unknown;
		return aret;
	}
};
constexpr logical_bool neither{special_init,0,0},either{special_init,1,1},unknown{special_init,0,0,1};

//file_end

