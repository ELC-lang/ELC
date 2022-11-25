//basic_environment.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace elc::defs{
	#include "_defs.hpp"

	inline namespace basic_environment{
		/// 每个bit（不是字节）的可能性
		/// 为什么c艹委员会不定义这个？
		constexpr size_t BIT_POSSIBILITY=2;
		/*! 给定大小的无符号整数类型 */
		template<size_t size>
		using unsigned_specific_size_t=decltype(lambda(){
			if constexpr(size==1)
				return uint8_t{};
			elseif constexpr(size==2)
				return uint16_t{};
			elseif constexpr(size==4)
				return uint32_t{};
			elseif constexpr(size==8)
				return uint64_t{};
		}());
	}

	#include "_undefs.hpp"
}

//file_end

