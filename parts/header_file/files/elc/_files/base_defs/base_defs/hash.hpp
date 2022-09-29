//hash.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace hash_n{
	typedef size_t hash_base_t;
	struct hash_value_t{
		hash_base_t _value;
		constexpr hash_base_t operator%(auto&&a)const noexcept{
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
			return{hash_base_t(a)};
	}

	inline struct hash_t{
		template<class T>
		[[nodiscard]]static constexpr inline bool nothrow_helper()noexcept{
			if constexpr(is_pointer<T>)
				return noexcept(pointer_hash(declvalue(const T&)));
			elseif constexpr(is_fundamental_hash<T>)
				return noexcept(hash_value_t{hash_base_t(declvalue(const T&))});
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

		#define hash operator()

		[[nodiscard]]inline constexpr hash_value_t hash(nothing)const noexcept{
			return{hash_base_t(nothing)};
		}
		[[nodiscard]]inline hash_value_t hash(const base_type_info_t&a)const noexcept{
			return{a.get_hash()};
		}
		template<class T> requires able<T>
		[[nodiscard]]constexpr_as_auto inline auto hash(const T&a)const noexcept(nothrow<T>){
			#undef hash
			if constexpr(is_pointer<T>)
				return pointer_hash(a);
			elseif constexpr(is_fundamental_hash<T>)
				return hash_value_t{hash_base_t(a)};
			elseif constexpr(is_unstable_hash<T>)
				return unstable_hash_value_t(a);
			elseif constexpr(was_not_an_ill_form(declvalue(const T&).hash()))
				return a.hash();
			elseif constexpr(was_not_an_ill_form(hash_value_t(declvalue(const T&))))
				return hash_value_t(a);
			else{
				template_error("Please overload the function hash in the namespace where this type is defined.");
				return hash_value_t{};
			}
			#define hash operator()
		}
		template<class T>
		[[nodiscard]]constexpr_as_auto inline hash_base_t get_hash_in_base_type(const T&a)const noexcept(nothrow<T>){
			return hash(a)._value;
		}

		/*从某个起始点算起的hash*/
		template<class T>
		[[nodiscard]]force_inline constexpr hash_value_t with_calculated_before(hash_value_t before,size_t before_size,const T*a,size_t size)const noexcept{
			hash_base_t aret=before._value;
			rot_iterator<decltype(aret)>rotl_offset = before_size+size;
			while(size--){
				aret ^= rotl(get_hash_in_base_type(a[size]),rotl_offset);
				rotl_offset--;
			}
			return{aret};
		}
		/*计算此hash重复N次的数组的hash结果*/
		[[nodiscard]]force_inline constexpr hash_value_t repeat_times(hash_value_t value,size_t size)const noexcept{
			hash_base_t aret=0;
			{
				//优化.
				//关於此,咱确信咱发现一种美妙的证法来保证这个优化不影响结果,但可惜凋可怜的脑容量不足以让他看懂.
				constexpr size_t bit_range_max=bitnum_of(hash_base_t);
				constexpr hash_base_t void_hash{nothing};
				suppress_msvc_warning(26475)//强制转换警告diss.
				constexpr hash_base_t npos_hash{hash_base_t(-1)};
				constexpr size_t bitnumof_void=bit_range_max*BIT_POSSIBILITY;

				size=size%bitnumof_void;
				if(value._value==0 || size==0)
					return {void_hash};
				if(size >= bit_range_max){
					const bool is_npos = ::std::popcount(value._value)%BIT_POSSIBILITY;
					aret = is_npos?npos_hash:void_hash;
					size-=bit_range_max;
				}
				if(!size)
					return {aret};
			}
			rot_iterator<decltype(aret)>rotl_offset = size;
			while(size--){
				aret ^= rotl(value._value,rotl_offset);
				rotl_offset--;
			}
			return{aret};
		}
		/*计算此hash重复N次的数组的hash结果*/
		template<class T>
		[[nodiscard]]force_inline constexpr hash_value_t repeat_times(T&&value,size_t size)const noexcept{
			return repeat_times(hash(value),size);
		}
		template<class T>
		[[nodiscard]]constexpr inline hash_value_t hash(const T*a,size_t size)const noexcept(nothrow<const T>){
			return with_calculated_before(hash(nothing),0,a,size);
		}
		/*合并两个数据段的hash结果，好似计算这两个数据段合并后的hash结果一般*/
		[[nodiscard]]force_inline hash_value_t merge_array_hash_results(
			hash_value_t before,size_t before_size,hash_value_t after,[[maybe_unused]]size_t after_size
		)const noexcept{
			return{before._value^(rotl(after._value,before_size))};
		}
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]constexpr inline hash_value_t hash(const array_like_view_t<T>a)const noexcept(nothrow<T>){
			return hash(a.begin(),a.size());
		}
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]constexpr inline hash_value_t with_calculated_before(hash_value_t before,size_t before_size,const array_like_view_t<T>a)const noexcept{
			return with_calculated_before(before,before_size,a.begin(),a.size());
		}

		#undef hash

	}hash{};
}
typedef hash_n::hash_value_t hash_t;
typedef hash_n::unstable_hash_value_t unstable_hash_t;
using hash_n::is_unstable_hash;
using hash_n::is_fundamental_hash;
using hash_n::hash;

//file_end

