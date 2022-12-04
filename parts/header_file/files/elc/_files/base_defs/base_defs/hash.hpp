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
	/*!
	Computes the hash value of a pointer.

	@param a The pointer to hash.

	@returns The hash value of the pointer.
	*/
	template<class T>
	[[nodiscard]]inline constexpr hash_value_t pointer_hash(T*a)noexcept{
		if constexpr(was_not_an_ill_form(the_pointer_hash(a)))
			return the_pointer_hash(a);
		else
			return{hash_base_t(a)};
	}

	inline struct hash_t{
	private:
		template<class T>
		[[nodiscard]]static inline constexpr bool nothrow_helper()noexcept{
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
	public:
		template<class T>
		static constexpr bool nothrow=nothrow_helper<T>();
	private:
		template<class T>
		[[nodiscard]]static inline constexpr bool able_helper()noexcept{
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
	public:
		template<class T>
		static constexpr bool able=able_helper<T>();

		#define hash operator()

		/*!
		Computes the hash value of `nothing`.

		@returns The hash value of `nothing`.
		*/
		[[nodiscard]]inline constexpr hash_value_t hash(nothing)const noexcept{
			return{hash_base_t(nothing)};
		}
		/*!
		Computes the hash value of a base_type_info_t object.

		@param a The base_type_info_t object to hash.

		@returns The hash value of the object.
		*/
		[[nodiscard]]inline hash_value_t hash(const base_type_info_t&a)const noexcept{
			return{a.get_hash()};
		}
		/*!
		Computes the hash value of a value.

		@param a The value to hash.

		@returns The hash value of the value.
		*/
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
		/*!
		Computes the hash of a value in the base type.

		@param a The value to hash.

		@returns The hash of the value in the base type.
		*/
		template<class T>
		[[nodiscard]]constexpr_as_auto inline hash_base_t get_hash_in_base_type(const T&a)const noexcept(nothrow<T>){
			return hash(a)._value;
		}
		/*!
		从某个起始点算起的hash

		@param before The hashed value before this hash.
		@param before_size The size of the hashed value before this hash.
		@param a The sequence of values.
		@param size The number of values in the sequence.

		@returns The hash value of the sequence.
		*/
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
		/*!
		计算此hash重复N次的数组的hash结果

		@param value The hash value to repeat.
		@param size The number of times to repeat the hash value.

		@returns The repeated hash value.
		*/
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
					return{void_hash};
				if(size >= bit_range_max){
					const bool is_npos = ::std::popcount(value._value)%BIT_POSSIBILITY;
					aret = is_npos?npos_hash:void_hash;
					size-=bit_range_max;
				}
				if(!size)
					return{aret};
			}
			rot_iterator<decltype(aret)>rotl_offset = size;
			while(size--){
				aret ^= rotl(value._value,rotl_offset);
				rotl_offset--;
			}
			return{aret};
		}
		/*!
		计算此hash重复N次的数组的hash结果

		@param value The hash value to repeat.
		@param size The number of times to repeat the hash value.

		@returns The repeated hash value.
		*/
		template<class T>
		[[nodiscard]]force_inline constexpr hash_value_t repeat_times(T&&value,size_t size)const noexcept{
			return repeat_times(hash(value),size);
		}
		/*!
		Computes the hash value of an array of elements.

		@param a The array of elements.
		@param size The size of the array.

		@returns The hash value of the array.
		*/
		template<class T>
		[[nodiscard]]inline constexpr hash_value_t hash(const T*a,size_t size)const noexcept(nothrow<const T>){
			return with_calculated_before(hash(nothing),0,a,size);
		}
		/*!
		合并两个数据段的hash结果，好似计算这两个数据段合并后的hash结果一般

		@param before The first hash value.
		@param before_size The size of the first hash value.
		@param after The second hash value.
		@param after_size The size of the second hash value.

		@returns The merged hash value.
		*/
		[[nodiscard]]force_inline hash_value_t merge_array_hash_results(
			hash_value_t before,size_t before_size,hash_value_t after,[[maybe_unused]]size_t after_size
		)const noexcept{
			return{before._value^(rotl(after._value,before_size))};
		}
		/*!
		计算一个 array_like_view_t<T> 对象的哈希值。
		@param a array_like_view_t<T> 对象。

		@returns array_like_view_t<T> 对象的哈希值。
		*/
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]inline constexpr hash_value_t hash(const array_like_view_t<T>a)const noexcept(nothrow<T>){
			return hash(a.begin(),a.size());
		}
		/*!
		计算一个 array_like_view_t<T> 对象的哈希值，并在哈希值之前进行计算。

		@param before 用来作为计算基础的哈希值。
		@param before_size 用来作为计算基础的哈希值的大小。
		@param a 要计算哈希值的 array_like_view_t<T> 对象。

		@returns array_like_view_t<T> 对象的哈希值。
		*/
		template<class T> requires is_not_signal_value_for_array_like<T>
		[[nodiscard]]inline constexpr hash_value_t with_calculated_before(hash_value_t before,size_t before_size,const array_like_view_t<T>a)const noexcept{
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

