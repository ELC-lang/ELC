//expr.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
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
struct expr:cons,instance_struct<expr>{
	typedef expr this_t;
	override_instance_struct;
public:
	using cons::_car;
	using cons::_cdr;
	using cons::cons;

	[[nodiscard]]virtual value be_eval()override{
		return as_ptr(_car)->be_call(_cdr);
	}
	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<expr>;}
};

//file_end

