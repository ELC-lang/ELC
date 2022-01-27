// elc-header-file-dev_literal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
/*
未完成的elc解释器dev文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <string>
#include <iostream>
#ifdef _WIN32
	#include <fcntl.h>
	#include <io.h>
#endif

#include "../../files/base_exception"
#include "../../files/core"
#include "../../files/string"


#include "../../files/_files/_share/_defs.hpp"

namespace elc::defs{
	namespace base_read_n{
		struct base_read_error:exception{
			const char_t* _why;
			base_read_error(const char_t* why)noexcept{ _why=why; }
			virtual const char_t* what()noexcept override{ return _why; }
		};
		namespace AST_n{
			struct base_AST:abstract_base<base_AST>,ref_able<base_AST>,build_by_get_only,never_in_array<base_AST>{
				base_AST(never_ref_num_zero_t):ref_able<base_AST>(never_ref_num_zero){}
				base_AST()=default;
				virtual ~base_AST()noexcept=default;
				virtual string get_code_struct()=0;
				virtual void build_up(const char_t*& arg)=0;
			};
			inline struct NULL_AST final:base_AST,instance_struct<NULL_AST>{
				NULL_AST()noexcept:base_AST(never_ref_num_zero){}
				virtual void build_up(const char_t*& arg)override final{ throw base_read_error(es"build up NULL_AST"); }
				virtual string get_code_struct()override final{ throw base_read_error(es"code struct NULL_AST"); }
			}NULL_ASTP;
			base_AST* get_null_ptr(const base_AST*){ return&NULL_ASTP; }
		}
		comn_ptr_t<AST_n::base_AST> base_read_AST(const char_t*& arg);
		namespace AST_n{
			struct symbol_AST final:base_AST,instance_struct<symbol_AST>{
				string _symbol_name;
				virtual void build_up(const char_t*& arg)override final{
					if(*arg==ec('`')){
						while(*arg){
							if(*arg!=ec('`'))
								_symbol_name.push_back(*arg);
							elseif(*(arg+1)==ec('`')){
								_symbol_name.push_back(*arg);
								if(*(arg+1))
									arg++;
							}
							else{
								arg++;
								return;
							}
							arg++;
						}
						throw base_read_error(locale::str::base_read::expr_incomplete);
					}
					else{
						while(*arg){
							if(isspace(*arg)){
								arg++;
								return;
							}
							else
								_symbol_name.push_back(*arg);
							arg++;
						}
					}
				}
				virtual string get_code_struct()override final{
					return es"symbol: "+_symbol_name;
				}
			};
			struct list_AST final:base_AST,instance_struct<list_AST>{
				array_t<comn_ptr_t<base_AST>>_members;
				virtual void build_up(const char_t*& arg)override final{
					arg++;//"("
					while(*arg){
						if(isspace(*arg)){
							arg++;
							continue;
						}
						elseif(*arg!=ec(')'))
							_members.push_back(base_read_AST(arg));
						else{
							arg++;
							return;
						}
					}
					throw base_read_error(locale::str::base_read::expr_incomplete);
				}
				virtual string get_code_struct()override final{
					string_t aret=es"list: ( ";
					for(const auto& a:_members){
						aret+=a->get_code_struct();
						aret+=ec(' ');
					}
					aret+=ec(')');
					return aret;
				}
			};
		}
		struct AST_tree_t{
			comn_ptr_t<AST_n::base_AST> _m;
			string get_code_struct(){
				return _m->get_code_struct();
			}
		};
		comn_ptr_t<AST_n::base_AST> base_read_AST(const char_t*& arg){
			using namespace AST_n;
			using namespace elc::defs::locale::char_n;//isspace isalpha etc
			comn_ptr_t<base_AST> aret;
			while(*arg){
				if(isspace(*arg)){
					arg++;
					continue;
				}
				elseif(aret){
					return aret;
				}
				elseif(*arg==ec('(')){
					aret=get<list_AST>();
					aret->build_up(arg);
				}
				elseif(*arg==ec('`')||isprint(*arg)||*arg==ec('_')){
					aret=get<symbol_AST>();
					aret->build_up(arg);
				}
			}
			return aret;
		}
		AST_tree_t base_read(const char_t* arg){
			auto aret=base_read_AST(arg);
			if(*arg)
				throw base_read_error(locale::str::base_read::ending_of_expr_beyond_expectations);
			return {aret};
		}
	};
	base_read_n::AST_tree_t operator ""_elc_AST(const char_t*arg,size_t)noexcept(not_in_debug){
		return base_read_n::base_read(arg);

	}
	auto operator ""_elc_object(const char_t*,size_t)noexcept(not_in_debug){
		//return elc_code_t({L""});
	}
	auto operator ""_elc_code(const char_t*,size_t)noexcept(not_in_debug){
		//return elc_code_t({L""});
	}
}
namespace elc{
	using defs::operator ""_elc_AST;
	using defs::base_read_n::base_read_error;
}

#include "../../files/_files/_share/_undefs.hpp"

#define GET_CODE_STRUCT(...) ((__VA_ARGS__).get_code_struct())

int main()
{
	using namespace elc;
	try{
		std::cout << elc::APIs::str_code_convert::to_char_str(GET_CODE_STRUCT(U"( test)!4 )"_elc_AST));
	}
	catch(base_read_error& a){
		std::cerr << elc::APIs::str_code_convert::to_char_str(a.what());
	}
}
