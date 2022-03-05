//code_list.hpp
//at namespace elc::defs::core
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline value eval(ptr p){
	return p->be_eval();
}
inline value eval(auto&&node){
	return eval(as_ptr(node));
}
struct code_list:cons,instance_struct<code_list>{
	typedef code_list this_t;
	override_instance_struct;
public:
	using cons::cons;

	[[nodiscard]]virtual value be_eval()override{
		ptr self = this;
		value aret{special_init};
	from_goto:
		try{
			do{
				eval(car_of(self)) >> aret;
				self=cdr_of(self);
			}while(self);
		}
		catch(core_runtime_internal_helper_n::goto_t&info){
			self=info.to;
			goto from_goto;
		}
		catch(core_runtime_internal_helper_n::return_t&){
		}
		return aret;
	}
	[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override{return type_info<code_list>;}
};

//file_end

