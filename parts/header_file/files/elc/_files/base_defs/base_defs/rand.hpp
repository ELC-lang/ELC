//rand.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace rand_n{
	typedef uint_t seed_type;//考虑到通用性以便于跨平台通用的seed，不可以使用uintmax_t
	namespace linear_congruential_arguments_n{
		//type
		template<class T>
		using rand_value_type=unsigned_specific_size_t<min(2*sizeof(T),sizeof(seed_type))>;

		//m
		template<class T>
		[[nodiscard]]force_inline constexpr auto get_modulus_of(){
			//由于rand_value_type一定是无符号整数，所以max(type_info<rand_value_type>)+1等价于pow(BIT_POSSIBILITY,bitnum_of(rand_value_type))
			//同时可以简化编译期计算
			//若rand_value_type的max超出了当前环境支持的最宽uint最大值，它会在编译期合乎标准的溢出到0
			return max(type_info<rand_value_type<T>>)+1;
		}
		//a
		[[nodiscard]]inline constexpr size_t get_multiplier_by_modulus(auto m){
			using namespace magic_number;
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			size_t a_off_1=1;
			/*
				`a - 1` is divisible by all prime factors of m
			*/
			{
				//get all prime factors of BIT_POSSIBILITY as modulus always is a power of BIT_POSSIBILITY
				size_t prime_test	= BIT_POSSIBILITY;
				size_t prime_tester = 2;
				while(prime_test!=1){
					if(prime_test%prime_tester==0){
						prime_test /= prime_tester;
						if(a_off_1 % prime_tester)
							a_off_1 *= prime_tester;
						//reset prime_test
						prime_tester=2;
					}
					else
						prime_tester=get_prime_num_big_than(prime_tester);
				}
			}
			/*
				`a - 1` is divisible by 4 if m is divisible by 4.
			*/
			while(m%4==0 && a_off_1%4!=0)
				a_off_1*=2;
			/*
				`a - 1` should not be any more divisible by prime factors of m
				`a - 1` should not small than sqrt(m)
			*/
			{
				const auto sqrt_m = sqrt(m);
				do
					a_off_1 *= get_prime_num_big_than(a_off_1);
				while(a_off_1 < sqrt_m);
			}
			//return a
			return a_off_1+1;
		}
		template<class T>
		[[nodiscard]]force_inline constexpr size_t get_multiplier_of(){
			return get_multiplier_by_modulus(get_modulus_of<T>());
		}
		//c
		[[nodiscard]]force_inline constexpr size_t get_increment_by_modulus([[maybe_unused]]auto m){
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			/*
				`c` and `m` are coprime.
			*/
			return 1;
		}
		template<class T>
		[[nodiscard]]force_inline constexpr size_t get_increment_of(){
			return get_increment_by_modulus(get_modulus_of<T>());
		}
	}

	distinctive inline struct rand_seed_t{
		typedef unsigned_specific_size_t<sizeof(seed_type)/2> value_gen_cache_base_t;
		typedef data_block<value_gen_cache_base_t> value_gen_cache_t;

		seed_type			_seed,_seed_origin;
		value_gen_cache_t	_result_base_data, _xor_rot_offset_data;

		[[nodiscard]]inline static constexpr seed_type sowing_seed(seed_type seed)noexcept{
			for(size_t i=bitnum_of(seed_type);i--;){
				seed=13*seed+7;
			}
			return seed;
		}
		[[nodiscard]]inline constexpr seed_type get_seed()const noexcept{
			return _seed_origin;
		}
		[[nodiscard]]inline constexpr seed_type get_seed_now()const noexcept{
			return _seed;
		}
		inline constexpr void set_seed(seed_type seed)noexcept{
			set_seed_with_out_sowing(sowing_seed(seed));
		}
		inline constexpr void set_seed_with_out_sowing(seed_type seed)noexcept{
			_seed					= seed;
			_seed_origin			= seed;
			_result_base_data		= value_gen_cache_base_t(_seed >> bitnum_of(value_gen_cache_base_t));
			_xor_rot_offset_data	= value_gen_cache_base_t(_seed);
		}
		void set_seed_by_time()noexcept{
			set_seed(seed_type(::std::time(nullptr)));
		}
		constexpr rand_seed_t(seed_type seed=magic_number::god)noexcept{
			set_seed(seed);
		}
		template<typename T> requires(sizeof(seed_type)/2 >= sizeof(T))
		[[nodiscard]]force_inline constexpr auto base_gen_randbit(size_t modulus,size_t multiplier,size_t increment)noexcept{
			typedef unsigned_specific_size_t<sizeof(T)>		result_type;
			typedef unsigned_specific_size_t<2*sizeof(T)>	rand_value_type;
			_seed						 = multiplier * _seed + increment;
			constexpr size_t half_bitnum = bitnum_of(result_type);
			rand_value_type	 new_result;
			if(modulus)
				new_result = rand_value_type(_seed % modulus);
			else
				//若modulus溢出到0
				//此时不用取模
				new_result = rand_value_type(_seed);
			const auto		  xor_base		 = result_type(new_result >> half_bitnum);
			auto&			  xor_rot_offset = data_cast<result_type>(_xor_rot_offset_data);
			auto&			  result_base	 = data_cast<result_type>(_result_base_data);
			const auto		  rot_offset	 = result_type(new_result);
			const auto		  xor_value		 = rotr(xor_base, xor_rot_offset);
			const result_type result		 = rotl(result_base, rot_offset) ^ xor_value;
			//缓存以便下次计算
			xor_rot_offset = rot_offset;
			result_base	   = xor_base ^ result;
			return result;
		}
		template<typename T> requires(sizeof(seed_type)/2 >= sizeof(T))
		[[nodiscard]]force_inline constexpr T gen_randbit()noexcept{
			using namespace linear_congruential_arguments_n;
			typedef unsigned_specific_size_t<sizeof(T)> result_type;
			data_block<T,result_type>aret=base_gen_randbit<T>(get_modulus_of<T>(),get_multiplier_of<T>(),get_increment_of<T>());
			return data_cast<T>(aret);
		}
		template<typename T> requires(sizeof(seed_type)/2 < sizeof(T))
		[[nodiscard]]force_inline constexpr T gen_randbit()noexcept{
			typedef unsigned_specific_size_t<sizeof(seed_type)/2> sand_type;
			data_block<T,sand_type>aret;
			byte*p=aret;//aret.begin()
			
			for(size_t i=sizeof(T)/sizeof(sand_type);i--;){
				data_cast<sand_type>(p)=gen_randbit<sand_type>();
				p+=sizeof(sand_type);
			}
			if constexpr(sizeof(T)%sizeof(sand_type)){
				const auto rest=sizeof(T)%sizeof(sand_type);
				for(size_t i=rest;i--;){
					*p++=gen_randbit<byte>();
				}
			}
			return data_cast<T>(aret);
		}
	}rand_seed{};

	template<class T>
	struct rand_t{
		static constexpr bool able=::std::is_trivially_constructible_v<T>;
		static constexpr bool nothrow=able;
		//rand
		[[nodiscard]]force_inline T operator()()const noexcept{
			return rand_seed.gen_randbit<T>();
		}
		//浮点特供：[0,1)和[0,1]
		/// [0,1)
		[[nodiscard]]force_inline static constexpr T between_0_and_1_exclusive()noexcept requires(::std::is_floating_point_v<T>){
			T base=T(rand_seed.gen_randbit<unsigned_specific_size_t<sizeof(T)>>());
			constexpr size_t div_times=bitnum_of(base);
			constexpr auto div_num=pow(BIT_POSSIBILITY,div_times);
			return T(base/div_num);
		}
		/// [0,1]
		[[nodiscard]]force_inline static constexpr T between_0_and_1_inclusive()noexcept requires(::std::is_floating_point_v<T>){
			auto rnd=rand_seed.gen_randbit<unsigned_specific_size_t<sizeof(T)>>();
			//考虑到浮点数总会使用一些位来表示指数，取rnd的最低位来表示rand到1.0或以上的概率也不会影响到结果
			const bool is_one=rnd&1;rnd>>=1;
			T base=T(rnd);
			if(is_one)return rnd==0?1:between_0_and_1_inclusive();//若rnd!=0，为了概率均匀需要reroll
			constexpr size_t div_times=bitnum_of(base)-1;//取了一位来表示是否为1
			constexpr auto div_num=pow(BIT_POSSIBILITY,div_times);
			return T(base/div_num);
		}
		//between
		struct between_integral_t{
			T _min,_diff;
			size_t _bitnum;
			constexpr between_integral_t(T min,T max)noexcept:_min(min),_diff(max-min){
				_bitnum=1;
				for(;_diff>>_bitnum;_bitnum++);
			}
			[[nodiscard]]force_inline T operator()()const noexcept{
				return inclusive();
			}
			[[nodiscard]]force_inline T exclusive()const noexcept{
				T ret;
				do{
					ret=rand_seed.gen_randbit<T>();
					ret&=(1<<_bitnum)-1;
				}while(ret>=_diff);
				return ret+_min;
			}
			[[nodiscard]]force_inline T inclusive()const noexcept{
				T ret;
				do{
					ret=rand_seed.gen_randbit<T>();
					ret&=(1<<_bitnum)-1;
				}while(ret>_diff);
				return ret+_min;
			}
		};
		struct between_floating_t{
			T _min,_diff;
			constexpr between_floating_t(T min,T max)noexcept:_min(min),_diff(max-min){}
			[[nodiscard]]force_inline T operator()()const noexcept{
				return inclusive();
			}
			[[nodiscard]]force_inline T exclusive()const noexcept{
				return _min+between_0_and_1_exclusive()*_diff;
			}
			[[nodiscard]]force_inline T inclusive()const noexcept{
				return _min+between_0_and_1_inclusive()*_diff;
			}
		};
		[[nodiscard]]force_inline constexpr auto between(T amin,T amax)const noexcept requires ::std::is_arithmetic_v<T>{
			if constexpr(::std::is_integral_v<T>)
				return between_integral_t(min(amin,amax),max(amin,amax));
			else
				return between_floating_t(min(amin,amax),max(amin,amax));
		}
	};
	template<class T>
	constexpr rand_t<T>rand{};
}
using rand_n::rand;
using rand_n::rand_seed;

//file_end

