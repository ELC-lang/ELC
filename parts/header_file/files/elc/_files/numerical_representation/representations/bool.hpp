//bool.hpp
//at namespace elc::defs::numerical_representation_n
/*
未完成的elc解释器numerical_representation文件
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
/*!
基类，记录bool的表示方式以便客制化to_string
*/
struct base_bool_representation_t{
	virtual ~base_bool_representation_t()noexcept=default;
	virtual string_view get_true()const noexcept=0;
	virtual string_view get_false()const noexcept=0;
	virtual string_view get_other_prefix()const noexcept=0;
	virtual string_view get_other_suffix()const noexcept=0;
};
template<class T>
concept bool_representation=is_base_of<base_bool_representation_t,T>;
/*!
默认的bool表示方式
*/
struct common_bool_representation_t:base_bool_representation_t{
	string_view get_true()const noexcept override{
		return es"true"_constexpr_str;
	}
	string_view get_false()const noexcept override{
		return es"false"_constexpr_str;
	}
	string_view get_other_prefix()const noexcept override{
		return es"other("_constexpr_str;
	}
	string_view get_other_suffix()const noexcept override{
		return es")"_constexpr_str;
	}
};
constexpr common_bool_representation_t common_bool_representation{};

//file_end

