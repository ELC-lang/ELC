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
		/*! 至少有给定大小的快速无符号整数类型 */
		template<size_t size>
		using unsigned_specific_size_fast_t=decltype(lambda(){
			#define TYPE_MAPPER(type) if constexpr(size <= sizeof(type))return (type)0;else
			#include "./arithmetic_mapper/unsigned_fast_mapper.hpp"
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
		//判断wchar_t是否和char_t相同
		inline constexpr bool wchar_t_same_as_char_t=sizeof(wchar_t)==sizeof(char_t);
		//判断wchar_t是否和char16_t相同
		inline constexpr bool wchar_t_same_as_char16_t=sizeof(wchar_t)==sizeof(char16_t);
		/*
		参考信息：
		float:
			Sign bit: 1 bit
			Exponent width: 8 bits
			Significand precision: 24 bits (23 explicitly stored)
		double:
			Sign bit: 1 bit
			Exponent: 11 bits
			Significand precision: 53 bits (52 explicitly stored)
		long double(IEEE-754 binary128):
			Sign bit: 1 bit
			Exponent width: 15 bits
			Significand precision: 113 bits (112 explicitly stored)
		注意一下long double在msvc上是double，得预处理判断一下
		*/
		//自浮点数获取精确数部分，舍去指数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_precision(T v)noexcept{
			if constexpr(::std::is_same_v<T,float>){
				auto tmp=*(uint32_t*)&v;
				tmp&=0x007FFFFF;
				return tmp;
			}
			else if constexpr(::std::is_same_v<T,double>){
				auto tmp=*(uint64_t*)&v;
				tmp&=0x000FFFFFFFFFFFFF;
				return tmp;
			}
			else if constexpr(::std::is_same_v<T,long double>){
				#if defined(_MSC_VER)//msvc上long double就是double
					return get_precision((double)v);
				#else
					auto tmp=*(uint128_t*)&v;
					tmp&=0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF;
					return tmp;
				#endif
			}
		}
		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_precision_base(T=T{})noexcept{
			if constexpr(::std::is_same_v<T,float>){
				return 0x800000u;
			}
			else if constexpr(::std::is_same_v<T,double>){
				return 0x10000000000000u;
			}
			else if constexpr(::std::is_same_v<T,long double>){
				#if defined(_MSC_VER)//msvc上long double就是double
					return get_precision_base(double{});
				#else
					return 0x10000000000000000000000000000u;
				#endif
			}
		}
		//（基础的）自浮点数获取指数部分，舍去基数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto base_get_exponent(T v)noexcept{
			if constexpr(::std::is_same_v<T,float>){
				auto tmp=*(uint32_t*)&v;
				tmp&=0x7FFFFFFF;
				tmp>>=23;
				return uint8_t(tmp);
			}
			else if constexpr(::std::is_same_v<T,double>){
				auto tmp=*(uint64_t*)&v;
				tmp&=0x7FFFFFFFFFFFFFFF;
				tmp>>=52;
				return uint16_t(tmp);
			}
			else if constexpr(::std::is_same_v<T,long double>){
				#if defined(_MSC_VER)//msvc上long double就是double
					return get_exponent((double)v);
				#else
					auto tmp=*(uint128_t*)&v;
					tmp&=0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
					tmp>>=112;
					return uint16_t(tmp);
				#endif
			}
		}
		//自浮点数获取指数部分，舍去基数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_exponent(T v)noexcept{
			const auto tmp=base_get_exponent(v);
			if constexpr(::std::is_same_v<T,float>){
				if(tmp==0)return -126;
				else return int8_t(tmp)-127;
			}
			else if constexpr(::std::is_same_v<T,double>){
				if(tmp==0)return -1022;
				else return int16_t(tmp)-1023;
			}
			else if constexpr(::std::is_same_v<T,long double>){
				#if defined(_MSC_VER)//msvc上long double就是double
					return get_exponent((double)v);
				#else
					if(tmp==0)return -16382;
					else return int16_t(tmp)-16383;
				#endif
			}
		}
		//自浮点数获取基数
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_base_num(T v)noexcept{
			const auto tmp=base_get_exponent(v);
			typedef decltype(get_precision_base(v)) precision_base_t;
			if(tmp==0)return precision_base_t{};
			else return get_precision_base(v);
		}
	}
	using basic_environment::BIT_POSSIBILITY;
	using basic_environment::unsigned_specific_size_t;
	using basic_environment::unsigned_specific_size_fast_t;
	using basic_environment::to_arithmetic;
	using basic_environment::wchar_t_same_as_char_t;
	using basic_environment::wchar_t_same_as_char16_t;
	using basic_environment::get_exponent;
	using basic_environment::get_base_num;
	using basic_environment::get_precision;
	using basic_environment::get_precision_base;

	#include "../_undefs.hpp"
}

//file_end

