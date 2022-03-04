//string_arec_as_value.hpp
//at namespace ::
//multi when ELC_STRING & ELC_CORE
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs{
	#include "../_share/_defs.hpp"
	namespace string_n{
		template<class char_T>
		value arec_as_value(string_t<char_T>&str,const value index){
			size_t i=size_t(use_as<int_t>(index));
			if(!i)
				i=size_t(use_as<uint_t>(index));
			struct arec_data_t final:instance_struct<arec_data_t>
			,value::base_data_t{
				typename string_t<char_T>::arec_t _m;
				arec_data_t(string_t<char_T>&str,size_t index)noexcept:_m(&str,index){};
				arec_data_t(const arec_data_t&ref)noexcept:_m(special_init,ref._m){}
				virtual ~arec_data_t()noexcept override final=default;

				virtual void be_set(ptr a)noexcept override final{_m=use_as<char_T>(a);}
				[[nodiscard]]virtual ptr get_value()noexcept override final{return core::make_binary_node_from<char_T>(_m);}
				[[nodiscard]]virtual base_data_t*copy()const noexcept override final{return get<arec_data_t>(*this);}
				[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<arec_data_t>;}
			};
			return get<arec_data_t>(str,i);
		}
	}
	#include "../_share/_undefs.hpp"
}

//file_end

