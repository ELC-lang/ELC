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
		//这里的定义不能使用basedefs中的type_info，所以得重新造一个小轮子
		struct type_uniquer_t{
			constexpr bool operator==(const type_uniquer_t&other)const noexcept{return this==&other;}
		};
		template<class T>
		inline constexpr type_uniquer_t type_uniquer{};
		//任意类型转算数类型
		inline constexpr struct to_arithmetic_t{
			template<class T,class type>
			//由于::std::is_convertible_v的歧义性质，使用requires表达式辅助推断type是否为实际支持的类型
			//由于requires表达式的强制性质，使用::std::is_convertible_v辅助推断隐式转换是否可行
			static inline constexpr bool is_convertible=std::is_convertible_v<T,type> && requires{declvalue(T).operator type();};
			template<class T>
			static inline constexpr bool r_able_helper()noexcept{
				#define TYPE_MAPPER(type) if constexpr(is_convertible<T,type>)return true;else
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				return false;
			}
			template<class T>
			static force_inline constexpr auto to_arithmetic_base(T&&v)noexcept{
				#define TYPE_MAPPER(type) if constexpr(r_able_helper<T>()&&is_convertible<T,type>)return (type)v;else
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				{}
			}
			template<class T>
			static inline constexpr size_t get_arithmetic_muti_convertible_count()noexcept{
				typedef decltype(to_arithmetic_base(declvalue(T))) my_type;
				push_and_disable_msvc_warning(26496);//未初始化警告diss
				size_t muti_convertible_count=0;
				pop_msvc_warning();
				#define TYPE_MAPPER(type) \
				if constexpr(type_uniquer<my_type>!=type_uniquer<type> && is_convertible<T,type>)\
					muti_convertible_count++;
				#include "./arithmetic_mapper/all_mapper.hpp"
				#undef TYPE_MAPPER
				return muti_convertible_count;
			}
			template<class T>
			static inline constexpr bool convertible_unique_helper()noexcept{
				return get_arithmetic_muti_convertible_count<T>()==0;
			}
			template<class T>
			static inline constexpr bool able_helper()noexcept{
				if constexpr(r_able_helper<T>())
					return convertible_unique_helper<T>();
				else
					return false;
			}
			template<class T>
			static inline constexpr bool able = able_helper<T>();
			//nothrow
			template<class T>
			static inline constexpr bool nothrow_helper()noexcept{
				if constexpr(able<T>){
					typedef decltype(to_arithmetic_base(declvalue(T))) my_type;
					return ::std::is_nothrow_convertible_v<T,my_type>;
				}
				else
					return false;
			}
			template<class T>
			static inline constexpr bool nothrow = nothrow_helper<T>();
			//算数类型转任意类型
			template<class T> requires(able<T>)
			force_inline constexpr auto operator()(T&&v)const noexcept(nothrow<T>){
				return to_arithmetic_base(::std::forward<T>(v));
			}
		}to_arithmetic{};
	}
	using basic_environment::BIT_POSSIBILITY;
	using basic_environment::unsigned_specific_size_t;
	using basic_environment::to_arithmetic;

	#include "../_undefs.hpp"
}

//file_end

