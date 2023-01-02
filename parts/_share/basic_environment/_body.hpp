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
		namespace float_infos{
			//精确数部分的掩码
			template<class T>
			constexpr auto precision_mask=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return(uint32_t)0x007FFFFFu;
				elseif constexpr(::std::is_same_v<T,double>)
					return(uint64_t)0x000FFFFFFFFFFFFFu;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return(uint64_t)0x000FFFFFFFFFFFFFu;
					#else
						return(uint128_t)0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFu;
					#endif
				}
			}();
			//精确数的无符号整数类型
			template<class T>
			using precision_type=decltype(precision_mask<T>);

			//浮点数的无符号整数数据类型
			template<class T>
			using data_type=decltype(lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return uint32_t{};
				elseif constexpr(::std::is_same_v<T,double>)
					return uint64_t{};
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return uint64_t{};
					#else
						return uint128_t{};
					#endif
				}
			}());

			//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
			template<class T>
			constexpr auto precision_base=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return(uint32_t)0x800000u;
				elseif constexpr(::std::is_same_v<T,double>)
					return(uint64_t)0x10000000000000u;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return(uint64_t)0x10000000000000u;
					#else
						return(uint128_t)0x10000000000000000000000000000u;
					#endif
				}
			}();

			//自浮点数获取精确数的基的位数（如float是23）
			template<class T>
			constexpr auto precision_base_bit=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return 23u;
				elseif constexpr(::std::is_same_v<T,double>)
					return 52u;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return 52u;
					#else
						return 112u;
					#endif
				}
			}();

			//浮点数的指数部分的diff（如float是127，double是1023，long double是16383）
			template<class T>
			constexpr auto exponent_diff=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return 127;
				elseif constexpr(::std::is_same_v<T,double>)
					return 1023;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return 1023;
					#else
						return 16383;
					#endif
				}
			}();
			//浮点数的指数部分的min（如float是-126，double是-1022，long double是-16382）
			template<class T>
			constexpr auto exponent_min=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return -126;
				elseif constexpr(::std::is_same_v<T,double>)
					return -1022;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return -1022;
					#else
						return -16382;
					#endif
				}
			}();
			//浮点数的指数部分的max（如float是127，double是1023，long double是16383）
			template<class T>
			constexpr auto exponent_max=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return 127;
				elseif constexpr(::std::is_same_v<T,double>)
					return 1023;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return 1023;
					#else
						return 16383;
					#endif
				}
			}();

			//浮点数的指数部分的无符号整数类型
			template<class T>
			using exponent_unsigned_type=decltype(lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return uint8_t{};
				elseif constexpr(::std::is_same_v<T,double>)
					return uint16_t{};
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return uint16_t{};
					#else
						return uint16_t{};
					#endif
				}
			}());
			//浮点数的指数部分的经过偏移后的有符号整数类型
			template<class T>
			using exponent_type=decltype(lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return int8_t{};
				elseif constexpr(::std::is_same_v<T,double>)
					return int16_t{};
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return int16_t{};
					#else
						return int16_t{};
					#endif
				}
			}());
			//浮点数的指数部分的掩码
			template<class T>
			constexpr auto exponent_mask=lambda(){
				if constexpr(::std::is_same_v<T,float>)
					return 0x7FFFFFFFu;
				elseif constexpr(::std::is_same_v<T,double>)
					return 0x7FFFFFFFFFFFFFFFu;
				elseif constexpr(::std::is_same_v<T,long double>){
					#if defined(_MSC_VER)//msvc上long double就是double
						return 0x7FFFFFFFFFFFFFFFu;
					#else
						return 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFu;
					#endif
				}
			}();
		}
		//自浮点数获取精确数部分，舍去指数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_precision(T v)noexcept{
			using namespace float_infos;
			auto tmp=*(data_type<T>*)&v;
			tmp&=precision_mask<T>;
			return tmp;
		}
		//自浮点数获取精确数的基（如float是2^23，double是2^52，long double是2^112）
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_precision_base(T=T{})noexcept{
			return float_infos::precision_base<T>;
		}
		template<class T> requires(::std::is_floating_point_v<T>)
		using float_precision_base_t = decltype(get_precision_base<T>());
		//（基础的）自浮点数获取指数部分，舍去基数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto base_get_exponent(T v)noexcept{
			using namespace float_infos;
			auto tmp=*(data_type<T>*)&v;
			tmp&=exponent_mask<T>;
			tmp>>=precision_base_bit<T>;
			return exponent_unsigned_type<T>(tmp);
		}
		//自浮点数获取指数部分，舍去基数和符号位
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_exponent(T v)noexcept{
			const auto tmp=base_get_exponent(v);
			using namespace float_infos;
			if(tmp==0)return exponent_min<T>;
			else return exponent_type<T>(tmp)-exponent_diff<T>;
		}
		//自浮点数获取基数
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr auto get_base_num(T v)noexcept{
			//特殊情况处理（exp=0时，base=0）
			const auto tmp=base_get_exponent(v);
			return tmp?get_precision_base(v):float_precision_base_t<T>{};
		}
		//自基数和指数构造浮点数
		//num=base_num*2^exponent
		template<class T> requires(::std::is_floating_point_v<T>)
		force_inline constexpr T make_float(float_precision_base_t<T> base_num,ptrdiff_t exponent)noexcept{
			using namespace float_infos;
			//首先将基数转换为precision_base（2^precision_base_bit）为分母的分数的分子
			//并在此过程中加减指数
			//需要注意的是，这里的基数是包含1的，所以转换目标是base_num>>precision_base_bit为1
			{
				const auto tmp=::std::countl_zero(base_num);
				constexpr auto need_shift=bitnum_of(base_num)-precision_base_bit<T>-1;
				const ptrdiff_t shift=tmp-need_shift;
				if(shift>0){
					base_num<<=shift;
					exponent-=shift;
				}
				elseif(shift<0){
					base_num>>=-shift;
					exponent-=shift;
				}
			}
			//适当放缩后，需要偏移指数部分，原因如下：
			//原有逻辑是num=base_num*2^exponent，这是大分数的表达逻辑
			//但是浮点数的逻辑是num=(base_num/precision_base)*2^exponent
			//将precision_base合并到exponent中，即可得到浮点数的逻辑
			exponent+=precision_base_bit<T>;
			if(exponent < exponent_min<T>){//指数过小，需要舍去
				const auto diff=exponent_min<T> - exponent;
				base_num>>=diff;
				exponent+=diff;
				if(!base_num)return 0;//基数过小，无法表示
			}
			if(exponent>exponent_max<T>)return 0;//指数过大，无法表示
			//DEN情况判断
			const bool is_den=!(base_num>>precision_base_bit<T>);//若基数最高位为0，则exponent一定为exponent_min，不用判断
			if(is_den){
				const data_type<T> data=base_num;
				return *(const T*)&data;
			}
			else{//非DEN情况下，根据浮点数表示规则去掉基数多余的1
				const auto exp=exponent_unsigned_type<T>(exponent+exponent_diff<T>);
				base_num-=get_precision_base<T>();
				data_type<T> data=base_num;
				data|=data_type<T>(exp)<<precision_base_bit<T>;
				return *(T*)&data;
			}
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
	using basic_environment::float_precision_base_t;
	using basic_environment::make_float;

	#include "../_undefs.hpp"
}

//file_end

