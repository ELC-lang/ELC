//rand.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace rand_n{
	typedef uint_t seed_type;
	distinctive seed_type rand_seed=1;
	template<class T>
	struct rand_impl_t{
		//m
		static constexpr size_t modulus=size_t(pow(BIT_POSSIBILITY,bitnum_of(T)));
		//a
		static constexpr size_t multiplier=lambda(){
			using namespace magic_number;
			//https://en.wikipedia.org/wiki/Linear_congruential_generator#Period_length
			size_t m=modulus;
			size_t a_off_1=m/4;
			/*
				`a − 1` is divisible by all prime factors of m
			*/
			//if BIT_POSSIBILITY is prime, then let's set a_off_1=7202081504*BIT_POSSIBILITY !
			if constexpr(is_prime_num(BIT_POSSIBILITY)){
				a_off_1=7202081504*BIT_POSSIBILITY;
			}
			else{
				//use this in future!
				//get all prime factors of m, we can use get_prime_num_big_than to get prime numbers
				size_t prime_test=2;
				while(m!=1){
					if(m%prime_test==0){
						m/=prime_test;
						a_off_1*=prime_test;
						//reset prime_test
						prime_test=2;
					}
					else
						prime_test=get_prime_num_big_than(prime_test);
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
			return modulus-1;
		}();

		//rand
		T operator()()const noexcept{
			rand_seed=multiplier*rand_seed+increment;
			return T(rand_seed%modulus);
		}
	};

	template<class T>
	struct rand_t{
		static constexpr bool able=::std::is_trivially_constructible_v<T>;
		static constexpr bool nothrow=true;

		//rand
		T operator()()const noexcept{
			data_block<T>aret;
			byte*init_p=aret;
			auto size=sizeof(T);
			if constexpr(sizeof(T)>=sizeof(seed_type)){
				while(size>=sizeof(seed_type)){
					data_cast<seed_type>(init_p)=rand_impl_t<seed_type>{}();
					init_p+=sizeof(seed_type);
					size-=sizeof(seed_type);
				}
				if constexpr(sizeof(T)%sizeof(seed_type)){
					data_cast<seed_type>(init_p+size-sizeof(seed_type))=rand_impl_t<seed_type>{}();
				}
			}
			elseif constexpr(sizeof(T)<sizeof(seed_type))
				while(size>0){
					*init_p=rand_impl_t<byte>{}();
					init_p+=1;
					size-=1;
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

