//logic.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/

/*! 混沌布尔值。
	允许输出介于真假之间的内容的布尔值。
	混沌布尔值会有五个可能的值，分别为true,false,neither,either。unknown视为不合法。
	用在if与while中时，true，either被判定为真，其与均为假。
	用法：
	logical_bool(bool)		输出logical_bool,将布尔值转换为混沌布尔值。
	bool(logical_bool)		输出bool，将混沌布尔值转化为布尔值。
	允许与或非运算：输出结果仍然为混沌布尔值,参与运算的值如果有unknown则均输出unknown.
	允许判定相等，输出结果为混沌布尔值,参与运算的值如果有unknown则均输出unknown，否则仅有true和false两个合法值。
*/
/*
	在 虚伪 的 信 念 之 前 - 他 们 将 三 元 论 视 为 真理
	所 有 的 东西 都 是 三 部 分 。
	从 那里 ， 他 们 终于 获得 了 答案 。
*/
struct logical_bool{
private:
	[[no_unique_address]]bool _is_true;
	[[no_unique_address]]bool _is_false;
	[[no_unique_address]]bool _is_unknown;
public:
	constexpr logical_bool(special_init_t,bool is_true,bool is_false,bool is_unknown=0)noexcept:_is_true(is_true),_is_false(is_false),_is_unknown(is_unknown){}
	constexpr logical_bool(bool a)noexcept:_is_true(a),_is_false(!a),_is_unknown(0){}
	constexpr logical_bool(const logical_bool&)noexcept=default;
	constexpr logical_bool&operator=(const logical_bool&)&noexcept=default;
	constexpr explicit operator bool()const noexcept{return _is_true && !_is_unknown;}
	constexpr logical_bool operator!()const noexcept{
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

