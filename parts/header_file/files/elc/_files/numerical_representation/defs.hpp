//defs.hpp
/*
未完成的elc解释器numerical_representation文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class numerical_representation{
	size_t _radix=10;
	string _radix_table=es"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"_elc_string;
	char_t _point=ec('.');
	char_t _sign=ec('-');
	string _nan=es"NaN"_elc_string;
	string _inf=es"Inf"_elc_string;
public:
	void set_radix(size_t radix){
		_radix=radix;
	}
	void set_radix_table(string_view radix_table){
		_radix_table=radix_table;
		_radix=radix_table.size();
	}
	void set_point(char_t point){
		_point=point;
	}
	void set_sign(char_t sign){
		_sign=sign;
	}
	void set_nan(string_view nan){
		_nan=nan;
	}
	void set_inf(string_view inf){
		_inf=inf;
	}
	[[nodiscard]]constexpr size_t radix()const noexcept{
		return _radix;
	}
	[[nodiscard]]constexpr string radix_table()const noexcept{
		return _radix_table;
	}
	[[nodiscard]]constexpr char_t point()const noexcept{
		return _point;
	}
	[[nodiscard]]constexpr char_t sign()const noexcept{
		return _sign;
	}
	[[nodiscard]]constexpr string nan()const noexcept{
		return _nan;
	}
	[[nodiscard]]constexpr string inf()const noexcept{
		return _inf;
	}
}

//file_end

