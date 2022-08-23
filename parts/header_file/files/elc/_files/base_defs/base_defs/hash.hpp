//hash.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace hash_n{
	struct hash_value_t{
		size_t _value;
		constexpr size_t operator%(auto&&a)const noexcept{
			return _value%a;
		}
		constexpr bool operator==(const hash_value_t&a)const noexcept{
			return _value==a._value;
		}
	};
	struct unstable_hash_value_t:hash_value_t{
		unstable_hash_value_t(const hash_value_t&a)noexcept:hash_value_t(a){}
	};

	template<class T>
	inline constexpr bool is_unstable_hash = type_info<T const>.can_convert_to<unstable_hash_value_t>;
	template<class T>
	inline constexpr bool is_fundamental_hash = ::std::is_fundamental_v<T>;

	enable_adl(the_pointer_hash);
	template<class T>
	[[nodiscard]]inline constexpr hash_value_t pointer_hash(T*a)noexcept{
		if constexpr(was_not_an_ill_form(the_pointer_hash(a)))
			return the_pointer_hash(a);
		else
			return{size_t(a)};
	}

	inline struct hash_t{
		template<class T>
		[[nodiscard]]static constexpr inline bool nothrow_helper()noexcept{
			if constexpr(is_pointer<T>)
				return noexcept(pointer_hash(declvalue(const T&)));
			elseif constexpr(is_fundamental_hash<T>)
				return noexcept(hash_value_t{size_t(declvalue(const T&))});
			elseif constexpr(is_unstable_hash<T>)
				return noexcept(unstable_hash_value_t(declvalue(const T&)));
			elseif constexpr(was_not_an_ill_form(declvalue(const T&).hash()))
				return noexcept(declvalue(const T&).hash());
			elseif constexpr(was_not_an_ill_form(hash_value_t(declvalue(const T&))))
				return noexcept(hash_value_t(declvalue(const T&)));
			elseif constexpr(type_info<remove_cv<T>> == type_info<base_type_info_t>)
				return true;
			else
				return false;
		}
		template<class T>
		static constexpr bool nothrow=nothrow_helper<T>();
		template<class T>
		[[nodiscard]]static constexpr inline bool able_helper()noexcept{
			if constexpr(is_pointer<T>)
				return true;
			elseif constexpr(is_fundamental_hash<T>)
				return true;
			elseif constexpr(is_unstable_hash<T>)
				return true;
			elseif constexpr(was_not_an_ill_form(declvalue(const T&).hash()))
				return true;
			elseif constexpr(was_not_an_ill_form(hash_value_t(declvalue(const T&))))
				return true;
			elseif constexpr(type_info<remove_cv<T>> == type_info<base_type_info_t>)
				return true;
			else
				return false;
		}
		template<class T>
		static constexpr bool able=able_helper<T>();

		[[nodiscard]]inline constexpr hash_value_t operator()(nothing)const noexcept{
			return{size_t(nothing)};
		}
		[[nodiscard]]inline hash_value_t operator()(const base_type_info_t&a)const noexcept{
			return{a.get_hash()};
		}
		template<class T>
		[[nodiscard]]constexpr_as_auto inline auto operator()(const T&a)const noexcept(nothrow<T>){
			if constexpr(is_pointer<T>)
				return pointer_hash(a);
			elseif constexpr(is_fundamental_hash<T>)
				return hash_value_t{size_t(a)};
			elseif constexpr(is_unstable_hash<T>)
				return unstable_hash_value_t(a);
			elseif constexpr(was_not_an_ill_form(declvalue(const T&).hash()))
				return a.hash();
			elseif constexpr(was_not_an_ill_form(hash_value_t(declvalue(const T&))))
				return hash_value_t(a);
			else template_error("Please overload the function hash in the namespace where this type is defined.");
		}
		template<class T>
		[[nodiscard]]constexpr_as_auto inline size_t get_hash_in_size_type(const T&a)const noexcept(nothrow<T>){
			return operator()(a)._value;
		}

		/*从某个起始点算起的hash*/
		template<class T>
		[[nodiscard]]constexpr force_inline hash_value_t with_calculated_before(hash_value_t before,size_t before_size,const T*a,size_t size)const noexcept{
			size_t aret=before._value;
			while(size--)
				aret ^= rotl(get_hash_in_size_type(a[size]),before_size+size);
			return{aret};
		}
		template<class T>
		[[nodiscard]]constexpr inline hash_value_t operator()(const T*a,size_t size)const noexcept(nothrow<const T>){
			return with_calculated_before(operator()(nothing),0,a,size);
		}
		/*合并两个数据段的hash结果，好似计算这两个数据段合并后的hash结果一般*/
		[[nodiscard]]force_inline hash_value_t merge_array_hash_results(
			hash_value_t before,size_t before_size,hash_value_t after,size_t after_size
		)const noexcept{
			return{before._value^(rotl(after._value,before_size))};
		}
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]constexpr inline hash_value_t operator()(array_like_view_t<T>a)const noexcept(nothrow<T>){
			return operator()(a.begin(),a.size());
		}
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]constexpr inline hash_value_t with_calculated_before(hash_value_t before,size_t before_size,array_like_view_t<T>a)const noexcept{
			return with_calculated_before(before,before_size,a.begin(),a.size());
		}
	}hash{};
}
typedef hash_n::hash_value_t hash_t;
typedef hash_n::unstable_hash_value_t unstable_hash_t;
using hash_n::is_unstable_hash;
using hash_n::is_fundamental_hash;
using hash_n::hash;

//file_end

