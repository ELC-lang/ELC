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
	[[nodiscard]]inline constexpr seed_type sowing_seed(seed_type seed)noexcept{
		for(size_t i=bitnum_of(seed_type);i--;){
			seed=13*seed+7;
		}
		return seed;
	}
	distinctive inline seed_type rand_seed=sowing_seed(magic_number::god);
	template<class T>
	struct rand_t{
		static constexpr bool able=::std::is_trivially_constructible_v<T> && (2*sizeof(T)<sizeof(seed_type) || !(sizeof(T)%sizeof(seed_type)));
		static constexpr bool nothrow=able;
	private:
		//由于进行低半部分的结果舍去所以2*sizeof(T)
		typedef unsigned_specific_size_t<min(2*sizeof(T),sizeof(seed_type))> rand_value_type;
		//m
		//由于rand_value_type一定是无符号整数，所以max(type_info<rand_value_type>)+1等价于pow(BIT_POSSIBILITY,bitnum_of(rand_value_type))
		//同时可以简化编译期计算
		//若rand_value_type的max超出了当前环境支持的最宽uint最大值，它会在编译期合乎标准的溢出到0
		static constexpr auto modulus=max(type_info<rand_value_type>)+1;

		//a
		static constexpr size_t multiplier=lambda(){
			using namespace magic_number;
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			const size_t m=modulus;
			size_t a_off_1=1;
			/*
				`a − 1` is divisible by all prime factors of m
			*/
			{
				//get all prime factors of BIT_POSSIBILITY as modulus always is a power of BIT_POSSIBILITY
				size_t prime_test=BIT_POSSIBILITY;
				size_t prime_tester=2;
				while(prime_test!=1){
					if(prime_test%prime_tester==0){
						prime_test/=prime_tester;
						if(a_off_1%prime_tester)
							a_off_1*=prime_tester;
						//reset prime_test
						prime_tester=2;
					}
					else
						prime_tester=get_prime_num_big_than(prime_tester);
				}
			}
			/*
				`a − 1` should not be any more divisible by prime factors of m
			*/
			a_off_1*=get_prime_num_big_than(a_off_1);
			/*
				`a − 1` is divisible by 4 if m is divisible by 4.
			*/
			while(m%4==0 && a_off_1%4!=0)
				a_off_1*=2;
			//return a
			return a_off_1+1;
		}();
		//c
		static constexpr size_t increment=lambda(){
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			/*
				`c` and `m` are coprime.
			*/
			return 1;
		}();

		typedef unsigned_specific_size_t<sizeof(rand_value_type)/2> result_type;
	public:
		[[nodiscard]]static force_inline result_type base_call()noexcept{
			const seed_type old_seed=rand_seed;
			rand_seed=multiplier*old_seed+increment;
			constexpr size_t half_bitnum=bitnum_of(result_type);
			rand_value_type old_result,new_result;
			if constexpr(modulus){
				old_result=rand_value_type(old_seed%modulus);
				new_result=rand_value_type(rand_seed%modulus);
			}
			else{
				//若rand_value_type的max超出了当前环境支持的最宽uint最大值，它会在编译期合乎标准的溢出到0（见modulus定义）
				//此时不用取模
				old_result = rand_value_type(old_seed);
				new_result = rand_value_type(rand_seed);
			}
			const auto result_base=result_type(old_result >> half_bitnum);
			const auto rot_offset=result_type(new_result);
			return rotl(result_base,rot_offset);
		}
		//rand
		[[nodiscard]]T operator()()const noexcept{
			data_block<T>aret;
			if constexpr(sizeof(T)==sizeof(result_type))
				data_cast<result_type>(aret)=base_call();
			else{
				byte*p=aret;
				for(size_t i=0;i<sizeof(T);i+=sizeof(result_type)){
					data_cast<result_type>(p+i)=base_call();
				}
			}
			return data_cast<T>(aret);
		}
	};
	template<class T>
	constexpr rand_t<T>rand{};

	//set_seed
	void set_seed(seed_type seed)noexcept{
		rand_seed=sowing_seed(seed);
	}
}
using rand_n::rand;

//file_end

