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
	distinctive seed_type rand_seed=72;
	template<class T>
	struct rand_t{
		static constexpr bool able=::std::is_trivially_constructible_v<T> && was_not_an_ill_form(unsigned_specific_size_t<sizeof(T)>{});
		static constexpr bool nothrow=able;
	private:
		//m
		static constexpr size_t modulus=size_t(pow(BIT_POSSIBILITY,2*min(bitnum_of(T),bitnum_of(seed_type)/2)));
		//a
		static constexpr size_t multiplier=lambda(){
			using namespace magic_number;
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			const size_t m=modulus;
			size_t a_off_1=m/4;
			/*
				`a − 1` is divisible by all prime factors of m
				`a − 1` should not be any more divisible by prime factors of m
			*/
			//if BIT_POSSIBILITY is prime, then let's set a_off_1=get_prime_num_big_than(BIT_POSSIBILITY)*BIT_POSSIBILITY !
			if constexpr(is_prime_num(BIT_POSSIBILITY)){
				a_off_1=get_prime_num_big_than(BIT_POSSIBILITY)*BIT_POSSIBILITY;
			}
			else{
				//use this in future!
				//get all prime factors of BIT_POSSIBILITY
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

		typedef unsigned_specific_size_t<min(sizeof(T),sizeof(seed_type)/2)> result_type;
	public:
		static result_type base_call()noexcept{
			seed_type old_seed=rand_seed;
			rand_seed=multiplier*old_seed+increment;
			constexpr size_t result_type_bitnum=bitnum_of(result_type);
			return rotl(result_type(old_seed >> result_type_bitnum), result_type(rand_seed << result_type_bitnum));
		}
		//rand
		T operator()()const noexcept{
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
		rand_seed=seed;
	}
}
using rand_n::rand;

//file_end

