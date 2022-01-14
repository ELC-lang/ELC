// elc-header-file-dev_literal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>
#include <iostream>
#include <cstdlib>//aligned_alloc:malloc
namespace std{//cpp20还未实现，占位。
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]] inline void* aligned_alloc([[maybe_unused]] std::size_t alignment,std::size_t size){ return malloc(size); }
	#endif
}

#include "../../files/base"

namespace elc::defs{
	typedef ::std::wstring string_t;//临时使用，后期换回elc::string_t
	namespace base_read{
		namespace AST{
			struct base_AST{
				virtual ~base_AST()noexcept=default;
				virtual string_t get_code_struct()=0;
			};
			struct symbol_AST:base_AST{
				string_t _symbol_name;
				virtual string_t get_code_struct()override{
					return L"symbol: "+_symbol_name;
				}
			};
			struct list_AST:base_AST{
				array_t<base_AST*>_members;
				virtual ~list_AST()noexcept{
					for(const auto& a:_members){
						unget(a);
					}
				}
				virtual string_t get_code_struct()override{
					string_t aret=L"list: (";
					for(const auto& a:_members){
						aret+=a->get_code_struct();
					}
					aret+=L')';
					return aret;
				}
			};
		}
		struct AST_container_t{
			AST::base_AST* _m;
		};
	};
	auto operator ""_elc_object(const char_t*,size_t)noexcept{
		//return elc_code_t({L""});
	}
	auto operator ""_elc_code(const char_t*,size_t)noexcept{
		//return elc_code_t({L""});
	}
}

#define GET_CODE_STRUCT(...) ((__VA_ARGS__)._code_struct)

int main()
{
	using namespace elc;
	//std::wcout << GET_CODE_STRUCT(L"test"_elc_object);
}
