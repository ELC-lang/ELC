//rand.hpp
//at namespace elc::defs
/*
未完成的elc解释器random文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
namespace rand_n{
	typedef uint_t seed_type;//考虑到通用性以便于跨平台通用的seed，不可以使用uintmax_t

	//修复32位环境下的溢出问题
	typedef uint64_t size_t;
	typedef int64_t ptrdiff_t;

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
			return uintmax_t(max(type_info<rand_value_type<T>>))+1;
		}
		//a
		[[nodiscard]]inline constexpr size_t get_multiplier_by_modulus(auto m){
			if(m==0)m-=4;//考虑到m可能溢出到0，此时对m进行约等计算
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

	//decl
	template<class T>
	struct rand_t;

	distinctive inline struct rand_seed_t{
	private:
		seed_type			   _seed, _seed_origin;
		static constexpr auto  εντροπία_bitnum = CHAR_BIT;
		static constexpr auto  εντροπία_num = size_t(pow(BIT_POSSIBILITY, εντροπία_bitnum));
		static constexpr auto  εντροπία_size = εντροπία_num * sizeof(seed_type);
		byte _εντροπία_data[εντροπία_size];

		static inline constexpr void sowing_seed_one_step(seed_type&seed)noexcept{seed=13*seed+7;}
	public:
		[[nodiscard]]inline constexpr seed_type get_origin()const noexcept{return _seed_origin;}
		inline constexpr void set(seed_type seed)noexcept{
			_seed					= seed;
			_seed_origin			= seed;
			constexpr auto half_bitnum = bitnum_of(seed_type)/2;
			static_assert(εντροπία_num>=bitnum_of(seed_type));
			for(size_t i=εντροπία_num;i--;){
				auto& cache = data_ptr_cast<seed_type>(_εντροπία_data)[i];
				cache		= _seed^(_seed >> half_bitnum);
				sowing_seed_one_step(_seed);
			}
		}
		void set_by_time()noexcept{this->set(seed_type(::std::time(nullptr)));}
		constexpr rand_seed_t(seed_type seed=magic_number::god)noexcept{this->set(seed);}
		constexpr rand_seed_t(const rand_seed_t&other)noexcept{
			_seed					= other._seed;
			_seed_origin			= other._seed_origin;
			if in_consteval
				for(size_t i=εντροπία_size;i--;)
					_εντροπία_data[i]	= other._εντροπία_data[i];
			else
				::std::memcpy(_εντροπία_data, other._εντροπία_data, εντροπία_size);
		}
	private:
		//friend
		template<class T>
		friend struct rand_t;

		template<typename T> requires(sizeof(seed_type)/2 >= sizeof(T))
		[[nodiscard]]force_inline constexpr auto base_gen_randbit()noexcept{
			using namespace linear_congruential_arguments_n;
			typedef unsigned_specific_size_t<sizeof(T)>		result_type;
			typedef unsigned_specific_size_t<2*sizeof(T)>	rand_value_type;
			constexpr auto	 multiplier	 = get_multiplier_of<seed_type>();
			constexpr auto	 increment	 = get_increment_of<seed_type>();
			constexpr size_t half_bitnum = bitnum_of(result_type);
			//
			const auto old_seed = _seed;
			_seed = multiplier*_seed+increment;
			//
			constexpr auto cut_seed_bitnum = bitnum_of(seed_type)-bitnum_of(rand_value_type);
			const auto new_result = rand_value_type(_seed>>cut_seed_bitnum);
			const auto old_result_base = rand_value_type(old_seed>>cut_seed_bitnum);
			//
			const auto		  result_base	= result_type(new_result >> half_bitnum);
			const auto		  rot_offset	= result_type(old_result_base >> half_bitnum);
			constexpr auto	  εντροπία_max	= εντροπία_size/sizeof(result_type);
			auto&			  xor_value		= data_ptr_cast<result_type>(_εντροπία_data)[rot_offset % εντροπία_max];
			const result_type result		= rotl(result_base, rot_offset) ^ xor_value;
			//
			xor_value = result;
			return result;
		}
		template<typename T> requires unsigned_basic_integer_type<T>
		[[nodiscard]]force_inline constexpr T gen_randbit()noexcept{
			if constexpr(sizeof(seed_type)/2 >= sizeof(T))
				return base_gen_randbit<T>();
			else{//为了加速，这不影响通用性并且大多数时候能节省2ns左右（将内存访问转为位操作）
				typedef unsigned_specific_size_t<sizeof(T)/2> sand_type;
				T aret=T(gen_randbit<sand_type>())<<bitnum_of(sand_type);
				aret|=T(gen_randbit<sand_type>());
				return aret;
			}
		}
		template<size_t sand_size,size_t size>
		force_inline constexpr void gen_randbit_with_sand_size_to_pointer(byte*to)noexcept{
			typedef unsigned_specific_size_t<sand_size> sand_type;
			constexpr size_t sand_num = size/sand_size;
			if constexpr(sand_num)
				for(size_t i=sand_num;i--;to+=sand_size)
					data_cast<sand_type>(to)=gen_randbit<sand_type>();
			if constexpr(size%sand_size)
				gen_randbit_with_sand_size_to_pointer<sand_size/BIT_POSSIBILITY,size%sand_size>(to);
		}
		template<typename T> requires(!unsigned_basic_integer_type<T>)
		[[nodiscard]]force_inline constexpr T gen_randbit()noexcept{
			if constexpr(sizeof(seed_type)/2 >= sizeof(T)){
				typedef unsigned_specific_size_t<sizeof(T)> result_type;
				return union_cast<T>(gen_randbit<result_type>());
			}
			else{
				data_block<T,align_as_t<seed_type>>aret;
				gen_randbit_with_sand_size_to_pointer<sizeof(seed_type),sizeof(T)>(aret);
				return data_cast<T>(aret);
			}
		}
		template<typename T> requires unsigned_integer_type<T>
		[[nodiscard]]force_inline constexpr T gen_randbit_with_bitnum(size_t bitnum)noexcept{
			if constexpr(is_big_type<T>){
				typedef T::base_type base_type;
				constexpr size_t base_bitnum = bitnum_of(base_type);
				T aret{};
				while(bitnum>base_bitnum){
					bitnum-=base_bitnum;
					apply_basetype_to_head(aret,gen_randbit<base_type>());
				}
				apply_basetype_to_head(aret,gen_randbit_with_bitnum<base_type>(bitnum));
				return aret;
			}
			else
				return gen_randbit<T>()&((T{1}<<bitnum)-1);
		}
	}rand_seed{};

	struct base_rand_t{
	protected:
		typedef base_rand_t this_t;
		rand_seed_t& _seed;
	public:
		constexpr base_rand_t(rand_seed_t&seed)noexcept:_seed(seed){}
		constexpr base_rand_t(const this_t&other)noexcept=default;
		constexpr base_rand_t(this_t&&other)noexcept=default;
		//delete operator=
		this_t& operator=(const this_t&other)=delete;
		this_t& operator=(this_t&&other)=delete;
	};
	template<class T>
	struct rand_t:base_rand_t{
		static constexpr bool common_able=::std::is_trivially_constructible_v<T>;
		static constexpr bool range_able=is_arithmetic_type<T> && !is_big_float_type<T>;//大浮点数不支持范围（因为其无限精度）
		static constexpr bool able=common_able || range_able;
		static constexpr bool nothrow=able;
	private:
		typedef rand_t<T> this_t;
		typedef base_rand_t base_t;
	public:
		using base_t::base_t;
		[[nodiscard]]static force_inline constexpr this_t with_seed(rand_seed_t&seed)noexcept{return this_t(seed);}
		//rand
		[[nodiscard]]force_inline T operator()()const noexcept(nothrow) requires common_able{return _seed.gen_randbit<T>();}
		//浮点特供：[0,1)和[0,1]
		/// [0,1)
		[[nodiscard]]force_inline constexpr T between_0_and_1_exclusive()const noexcept(nothrow) requires basic_float_type<T>{
			T base=T(_seed.gen_randbit<unsigned_specific_size_t<sizeof(T)>>());
			constexpr size_t div_times=bitnum_of(base);
			constexpr auto div_num=pow(BIT_POSSIBILITY,div_times);
			return T(base/div_num);
		}
		/// [0,1]
		[[nodiscard]]force_inline constexpr T between_0_and_1_inclusive()const noexcept(nothrow) requires basic_float_type<T>{
			auto rnd=_seed.gen_randbit<unsigned_specific_size_t<sizeof(T)>>();
			//考虑到浮点数总会使用一些位来表示指数，取rnd的最低位来表示rand到1.0或以上的概率也不会影响到结果
			const bool is_one=rnd&1;rnd>>=1;
			if(is_one)return rnd==0?1:between_0_and_1_inclusive();//若rnd!=0，为了概率均匀需要reroll
			T base=T(rnd);
			constexpr size_t div_times=bitnum_of(base)-1;//取了一位来表示是否为1
			constexpr auto div_num=pow(BIT_POSSIBILITY,div_times);
			return T(base/div_num);
		}
		//not nan.
		[[nodiscard]]force_inline constexpr T not_NaN()const noexcept(nothrow) requires common_able{//即使是大整数，你也不应该用这个（因为其范围无限）
			T num;
			do num=_seed.gen_randbit<T>();while(isNaN(num));
			return num;
		}
		//between
	private:
		struct between_integral_t{
		private:
			rand_seed_t& _seed;
			typedef to_unsigned_t<T> unsigned_T;
			T _min;
			unsigned_T _diff;
			size_t _bitnum;
		public:
			constexpr between_integral_t(rand_seed_t&seed,T amin,T amax)noexcept:_seed(seed),_min(amin),_diff(amax-amin){
				_bitnum=get_bitnum(_diff);
			}
			[[nodiscard]]force_inline T operator()()const noexcept{return inclusive();}
			[[nodiscard]]force_inline operator T()const noexcept{return operator()();}
			[[nodiscard]]force_inline T exclusive()const noexcept{
				unsigned_T diff;
				do diff=_seed.gen_randbit_with_bitnum<unsigned_T>(_bitnum);while(diff>=_diff);
				return _min+diff;
			}
			[[nodiscard]]force_inline T inclusive()const noexcept{
				unsigned_T diff;
				do diff=_seed.gen_randbit_with_bitnum<unsigned_T>(_bitnum);while(diff>_diff);
				return _min+diff;
			}
		};
		struct between_floating_t{
		private:
			rand_seed_t& _seed;
			T _min,_diff;
		public:
			constexpr between_floating_t(rand_seed_t&seed,T amin,T amax)noexcept:_seed(seed),_min(amin),_diff(amax-amin){}
			[[nodiscard]]force_inline T operator()()const noexcept{return inclusive();}
			[[nodiscard]]force_inline operator T()const noexcept{return operator()();}
			[[nodiscard]]force_inline T exclusive()const noexcept{
				return _min+rand_t<T>{_seed}.between_0_and_1_exclusive()*_diff;
			}
			[[nodiscard]]force_inline T inclusive()const noexcept{
				return _min+rand_t<T>{_seed}.between_0_and_1_inclusive()*_diff;
			}
		};
	public:
		[[nodiscard]]force_inline constexpr auto between(T amin,T amax)const noexcept requires range_able{
			#define args _seed,min(amax,amin),max(amax,amin)
			if constexpr(integer_type<T>)
				return between_integral_t(args);
			else
				return between_floating_t(args);
			#undef args
		}
		[[nodiscard]]force_inline constexpr auto uniform_in_range()const noexcept requires(range_able && has_min_max<T>){
			return between(min(type_info<T>),max(type_info<T>));
		}
	};
	//bool rand
	template<>struct rand_t<bool>:base_rand_t{
		static constexpr bool common_able=true;
		static constexpr bool range_able=false;//你打算干什么？
		static constexpr bool able=true;
		static constexpr bool nothrow=true;
	private:
		typedef bool T;
		typedef rand_t<T> this_t;
		typedef base_rand_t base_t;
	public:
		using base_t::base_t;
		[[nodiscard]]static force_inline constexpr this_t with_seed(rand_seed_t&seed)noexcept{return this_t(seed);}
		[[nodiscard]]force_inline T operator()()const noexcept{
			return _seed.gen_randbit<unsigned char>()&unsigned char{1};
		}
	};
	template<class T>
	constexpr rand_t<T>rand{rand_seed};
}
using rand_n::rand;
using rand_n::rand_seed;

//file_end

