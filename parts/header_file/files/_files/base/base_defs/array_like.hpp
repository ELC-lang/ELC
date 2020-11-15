//array_like.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_like_n{
	template<class T>
	inline constexpr size_t size_of_array_like(T&&)noexcept{return 1;}
	template<class T,size_t N>
	inline constexpr size_t size_of_array_like(T(&)[N])noexcept{return N;}
	template<class T>
	inline size_t size_of_array_like(::std::initializer_list<T>&a)noexcept{return a.size();}

	template<class T>
	inline constexpr auto begin_of_array_like(T&&a)noexcept{return addressof(a);}
	template<class T,size_t N>
	inline constexpr auto begin_of_array_like(T(&a)[N])noexcept{return addressof(a[0]);}
	template<class T>
	inline const T* begin_of_array_like(::std::initializer_list<T>&a)noexcept{return a.begin();}

	template<class T>
	inline auto end_of_array_like(T&&a)noexcept{return begin_of_array_like(a)+size_of_array_like(a);}

	template<class T>
	auto is_array_like_helper(int) -> decltype(
		void(begin_of_array_like(declvalue(T))),
		void(size_of_array_like(declvalue(T))),
		::std::true_type{});
	template<class>
	auto is_array_like_helper(...) -> ::std::false_type;

	template<class T>
	constexpr bool is_array_like=is_array_like_helper<T>();

	template<class T>
	constexpr bool is_signal_value_for_array_like=type_info<remove_cvref<decltype(*begin_of_array_like(declvalue(T)))>> == type_info<T>;
	template<class T>
	constexpr bool is_not_signal_value_for_array_like=!is_signal_value_for_array_like<T>;

	template<class T,class T_>
	auto is_array_like_for_helper(int) -> decltype(
		void(begin_of_array_like<T>(declvalue(T_))),
		void(size_of_array_like<T>(declvalue(T_))),
		::std::true_type{});
	template<class,class>
	auto is_array_like_for_helper(...) -> ::std::false_type;

	template<class T,class T_>
	constexpr bool is_array_like_for=is_array_like_for_helper<T,T_>();

	template<class T>
	struct array_like_view_t{
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef array_like_view_t<T>this_t;
	private:
		T*_begin;
		size_t _size;
	public:
		template<class T_,enable_if(is_array_like_for<T,T_>)>
		constexpr_as_auto array_like_view_t(T_&&a)noexcept_as(begin_of_array_like<T>(declvalue(T_)),size_of_array_like<T>(declvalue(T_))){
			_begin=begin_of_array_like<T>(a);
			_size=size_of_array_like<T>(a);
		}
		constexpr array_like_view_t(const this_t&)noexcept=default;

		[[nodiscard]]constexpr size_t size()noexcept{return _size;}

		[[nodiscard]]constexpr iterator begin()noexcept{return _begin;}
		[[nodiscard]]constexpr iterator end()noexcept{return begin()+size();}

		[[nodiscard]]constexpr const_iterator cbegin()const noexcept{return const_cast<this_t*>(this)->begin();}
		[[nodiscard]]constexpr const_iterator cend()const noexcept{return const_cast<this_t*>(this)->end();}

		[[nodiscard]]constexpr bool empty()const noexcept{return size();}

		[[nodiscard]]constexpr T&operator[](size_t pos)noexcept{return begin()[pos];}
		[[nodiscard]]constexpr const T&operator[](size_t pos)const noexcept{return const_cast<this_t&>(*this)[pos];}
	};
}
using array_like_n::size_of_array_like;
using array_like_n::begin_of_array_like;
using array_like_n::end_of_array_like;
using array_like_n::is_array_like;
using array_like_n::is_signal_value_for_array_like;
using array_like_n::is_not_signal_value_for_array_like;
using array_like_n::is_array_like_for;
using array_like_n::array_like_view_t;

//file_end

