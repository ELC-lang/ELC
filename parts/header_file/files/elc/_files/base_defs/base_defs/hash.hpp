//hash.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
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
	struct unstable_hash_t:hash_t{
		unstable_hash_t(const hash_t&a):hash_t(a){}
	};

	template<class T>
	inline constexpr bool is_unstable_hash = type_info<T const>.can_convert_to<unstable_hash_t>;
	template<class T>
	inline constexpr bool is_fundamental_hash = ::std::is_fundamental_v<T>;

	template<class T>
	[[nodiscard]]inline constexpr hash_t pointer_hash(T*a)noexcept{
		if constexpr(was_not_an_ill_form(the_pointer_hash(a)))
			return the_pointer_hash(a);
		else
			return{size_t(a)};
	}

	[[nodiscard]]inline constexpr hash_t hash(nothing)noexcept{
		return{size_t(nothing)};
	}
	[[nodiscard]]inline hash_t hash(base_type_info_t&a)noexcept{
		return{a.get_hash()};
	}
	template<class T>
	[[nodiscard]]constexpr_as_auto inline auto hash(const T&a)noexcept(is_fundamental_hash<T> or type_info<T>.can_nothrow_convert_to<hash_t>){
		if constexpr(is_pointer<T>)
			return pointer_hash(a);
		elseif constexpr(is_fundamental_hash<T>)
			return hash_t{size_t(a)};
		elseif constexpr(is_unstable_hash<T>)
			return unstable_hash_t(a);
		elseif constexpr(was_not_an_ill_form(declvalue(const T&).hash()))
			return a.hash();
		elseif constexpr(was_not_an_ill_form(hash_t(declvalue(const T&))))
			return hash_t(a);
		else template_error("Please overload the function hash in the namespace where this type is defined.");
	}
	template<class T>
	[[nodiscard]]inline hash_t hash(const T*a,size_t size)noexcept_as(hash(*a)){
		size_t aret=0;
		while(size--)
			aret=hash(a[size])._value+aret*13;
		return{aret};
	}
	template<class T> requires is_not_signal_value_for_array_like<T>
	[[nodiscard]]inline hash_t hash(array_like_view_t<T>a)noexcept_as(hash(declvalue(T))){
		return hash(a.begin(),a.size());
	}
}
using hash_n::hash_t;
using hash_n::unstable_hash_t;
using hash_n::is_unstable_hash;
using hash_n::is_fundamental_hash;
using hash_n::hash;

//file_end

