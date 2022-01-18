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
	[[nodiscard]]inline constexpr size_t size_of_array_like(T&&)noexcept{return 1;}
	template<class T,size_t N>
	[[nodiscard]]inline constexpr size_t size_of_array_like(T(&)[N])noexcept{return N;}
	template<class T>
	[[nodiscard]]inline size_t size_of_array_like(::std::initializer_list<T>&a)noexcept{return a.size();}

	template<class T>
	[[nodiscard]]inline constexpr auto begin_of_array_like(T&&a)noexcept{return addressof(a);}
	template<class T,size_t N>
	[[nodiscard]]inline constexpr auto begin_of_array_like(T(&a)[N])noexcept{return addressof(a[0]);}
	template<class T>
	[[nodiscard]]inline const T* begin_of_array_like(::std::initializer_list<T>&a)noexcept{return a.begin();}

	template<class T>
	[[nodiscard]]inline auto end_of_array_like(T&&a)noexcept{return begin_of_array_like(a)+size_of_array_like(a);}

	template<class T>
	constexpr bool is_array_like=was_not_an_ill_form_with_parameter(
									(T v){
										begin_of_array_like(v);
										size_of_array_like(v);
									}
								);

	template<class T>
	constexpr bool is_signal_value_for_array_like=type_info<remove_cvref<decltype(*begin_of_array_like(declvalue(T)))>> == type_info<T>;
	template<class T>
	constexpr bool is_not_signal_value_for_array_like=!is_signal_value_for_array_like<T>;

	template<class T,class U>
	constexpr bool is_array_like_for=was_not_an_ill_form_with_parameter(
										(U v){
											begin_of_array_like<T>(v);
											size_of_array_like<T>(v);
										}
									);

	template<class T>
	struct array_like_view_t{
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef array_like_view_t<T>this_t;
	private:
		T*_begin;
		size_t _size;
	public:
		explicit constexpr array_like_view_t(T*a,size_t b)noexcept:_begin(a),_size(b){}
		template<class U> requires is_array_like_for<T,U>
		constexpr_as_auto array_like_view_t(U&&a)noexcept_as(begin_of_array_like<T>(a),size_of_array_like<T>(a)):array_like_view_t(begin_of_array_like<T>(a),size_of_array_like<T>(a)){}
		constexpr array_like_view_t(const this_t&)noexcept=default;

		[[nodiscard]]constexpr size_t size()noexcept{return _size;}

		[[nodiscard]]constexpr iterator begin()noexcept{return _begin;}
		[[nodiscard]]constexpr iterator end()noexcept{return begin()+size();}

		[[nodiscard]]constexpr const_iterator cbegin()const noexcept{return const_cast<this_t*>(this)->begin();}
		[[nodiscard]]constexpr const_iterator cend()const noexcept{return const_cast<this_t*>(this)->end();}

		[[nodiscard]]constexpr bool empty()const noexcept{return size();}

		[[nodiscard]]constexpr T&operator[](size_t pos)noexcept{return begin()[pos];}
		[[nodiscard]]constexpr const T&operator[](size_t pos)const noexcept{return const_cast<this_t&>(*this)[pos];}

		[[nodiscard]]constexpr auto operator<=>(this_t a)noexcept(compare.nothrow<T>){
			return compare(_begin,_size,a._begin,a._size);
		}
		[[nodiscard]]constexpr auto operator==(this_t a)noexcept(equal.nothrow<T>){
			return equal(_begin,_size,a._begin,a._size);
		}
	};
	template<typename T>
	struct array_end_by_zero_t:array_like_view_t<T>{
		typedef array_like_view_t<T>base_t;

		[[nodiscard]]constexpr static size_t get_length_of(T*ptr){
			if(*ptr)return get_length_of(ptr+1)+1;
			else return 0;
		}

		constexpr array_end_by_zero_t(T*ptr):base_t(ptr,get_length_of(ptr)){}
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
using array_like_n::array_end_by_zero_t;

//file_end

