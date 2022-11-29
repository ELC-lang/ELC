//_body.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs{
	#include "../_defs.hpp"

	namespace basic_environment{
		/// 每个bit（不是字节）的可能性
		/// 为什么c艹委员会不定义这个？
		constexpr size_t BIT_POSSIBILITY=2;
		/*! 给定大小的无符号整数类型 */
		template<size_t size>
		using unsigned_specific_size_t=decltype(lambda(){
			#define TYPE_MAPPER(type) if constexpr(size == sizeof(type))return (type)0;else
			#include "./arithmetic_mapper/unsigned_mapper.hpp"
			#undef TYPE_MAPPER
			{}
		}());
		//任意类型转算数类型
		template<class T>
		force_inline constexpr bool is_arithmetic_convertible_helper()noexcept{
			#define TYPE_MAPPER(type) if constexpr(::std::is_convertible_v<T,type>)return true;else
			#include "./arithmetic_mapper/all_mapper.hpp"
			#undef TYPE_MAPPER
			return false;
		}
		template<class T>
		constexpr bool is_arithmetic_convertible= is_arithmetic_convertible_helper<T>();
		template<class T> requires(is_arithmetic_convertible<T>)
		force_inline constexpr auto to_arithmetic(T&&v)noexcept{
			#define TYPE_MAPPER(type) if constexpr(::std::is_convertible_v<T,type>)return(type)v;else
			#include "./arithmetic_mapper/all_mapper.hpp"
			#undef TYPE_MAPPER
			{}
		}
	}
	using basic_environment::BIT_POSSIBILITY;
	using basic_environment::unsigned_specific_size_t;
	using basic_environment::is_arithmetic_convertible;
	using basic_environment::to_arithmetic;

	#include "../_undefs.hpp"
}

//file_end

