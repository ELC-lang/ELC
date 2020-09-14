//*
#define STEVE_H_TEST
#include<evest.h>
#define STEST_ON
#include<steve.h>
/*/
#define al_adapt_steve_h
#define STEST_ON
#include <ala>
//*/

#include <type_traits>
#include <cstdlib>
namespace std{//cpp20还未实现，占位。
	template<size_t N,class T>
	[[nodiscard]]constexpr T*assume_aligned(T*ptr){return ptr;}
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]]inline void*aligned_alloc([[maybe_unused]]std::size_t alignment,std::size_t size){return malloc(size);}
	#endif

	#if defined(__GNUC__)&&!defined(__clang__)
	template<class,class,class=void>struct is_nothrow_convertible:std::false_type{};
	template<class From,class To>struct is_nothrow_convertible<From,To,enable_if_t<is_convertible_v<From,To>>>
	:bool_constant<noexcept(static_cast<To>(declval<From>()))>{};

	template<class From,class To>
	inline constexpr bool is_nothrow_convertible_v=is_nothrow_convertible<From,To>::value;

	template<class T>
	struct remove_cvref{
		typedef remove_cv_t<remove_reference_t<T>>type;
	};
	template<class T>
	using remove_cvref_t=typename remove_cvref<T>::type;
	#endif
}

#define ELC_TEST_ON
#include"../files/base"
#include"../files/core"
#include"../files/test"

int ste::stst(){
	elc::test();
	elc::test_log_out();
	elc::test_end();
	ret 0;
}
