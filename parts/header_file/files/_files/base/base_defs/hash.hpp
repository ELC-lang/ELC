//hash.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace hash_n{
	struct hash_t{
		size_t _value;
	};
	template<class T>
	auto operator%(const hash_t a,T&&b){
		return a._value%b;
	}
	struct unstable_hash_t:hash_t{};
	template<class T>
	auto is_unstable_hash_helper(int) -> decltype(
		void((struct unstable_hash_t)(declvalue(T const&))),
		::std::true_type{});
	template<class>
	auto is_unstable_hash_helper(...) -> ::std::false_type;

	template<class T>
	inline constexpr bool is_unstable_hash = decltype(is_unstable_hash_helper<T>(0))::value;
	template<class T>
	inline constexpr bool is_fundamental_hash = ::std::is_fundamental_v<T> && sizeof(T)<=sizeof(size_t);
	template<class T>
	constexpr_as_auto inline hash_t hash(const T&a)noexcept(is_fundamental_hash<T> or type_info<T>.can_nothrow_convert_to<hash_t>){
		if constexpr(is_fundamental_hash<T>)
			return{size_t(a)};
		elseif constexpr(is_unstable_hash<T>)
			return unstable_hash_t(a);
		else
			return hash_t(a);
	}
}
using hash_n::hash_t;
using hash_n::unstable_hash_t;
using hash_n::is_unstable_hash;
using hash_n::is_fundamental_hash;
using hash_n::hash;

//file_end

