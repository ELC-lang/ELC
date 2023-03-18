//array_like.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
namespace array_like_n{
	enable_adl(size_of_array_like);
	template<class T>
	[[nodiscard]]inline constexpr size_t size_of_array_like(T&&)noexcept{return 1;}
	template<class T,size_t N>
	[[nodiscard]]inline constexpr size_t size_of_array_like(T(&)[N])noexcept{return N;}
	template<class T>
	[[nodiscard]]inline size_t size_of_array_like(::std::initializer_list<T>&a)noexcept{return a.size();}

	enable_adl(begin_of_array_like);
	template<class T>
	[[nodiscard]]inline constexpr auto begin_of_array_like(T&&a)noexcept{return addressof(a);}
	template<class T,size_t N>
	[[nodiscard]]inline constexpr auto begin_of_array_like(T(&a)[N])noexcept{return addressof(a[0]);}
	template<class T>
	[[nodiscard]]inline const T* begin_of_array_like(::std::initializer_list<T>&a)noexcept{return a.begin();}

	enable_adl(end_of_array_like);
	template<class T>
	[[nodiscard]]inline auto end_of_array_like(T&&a)noexcept{return begin_of_array_like(a)+size_of_array_like(a);}

	template<class T>
	concept is_array_like=was_not_an_ill_form_with_parameter(
									(T v){
										begin_of_array_like(v);
										size_of_array_like(v);
									}
								);

	template<class T>
	concept is_signal_value_for_array_like=type_info<remove_cvref<decltype(*begin_of_array_like(declvalue(T)))>> == type_info<T>;
	template<class T>
	concept is_not_signal_value_for_array_like=!is_signal_value_for_array_like<T>;

	template<class T,class U>
	constexpr bool strict_is_array_like_for=was_not_an_ill_form_with_parameter(
										(U v){
											begin_of_array_like<T>(v);
											size_of_array_like<T>(v);
										}
									);
	template<class T,class U>
	constexpr bool is_array_like_for=strict_is_array_like_for<T,U>||strict_is_array_like_for<const T,U>;

	template<class T>
	struct array_like_view_t{
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef array_like_view_t<T>this_t;
		struct reverse_iterator{
			T* _m;
			constexpr reverse_iterator(T*p)noexcept:_m(p){}
			constexpr reverse_iterator(const reverse_iterator&)noexcept=default;
			constexpr reverse_iterator(reverse_iterator&&)noexcept=default;
			constexpr reverse_iterator operator++()noexcept{_m--;return *this;}
			constexpr reverse_iterator operator--()noexcept{_m++;return *this;}
			constexpr reverse_iterator operator++(int)noexcept{auto tmp=*this;this->operator++();return tmp;}
			constexpr reverse_iterator operator--(int)noexcept{auto tmp=*this;this->operator--();return tmp;}
			[[nodiscard]]constexpr T& operator*()const noexcept{return*_m;}
			[[nodiscard]]constexpr T& operator[](ptrdiff_t a)const noexcept{return*(_m-a);}
			[[nodiscard]]constexpr T* operator->()const noexcept{return _m;}
			[[nodiscard]]constexpr reverse_iterator operator+(ptrdiff_t diff)const noexcept{return _m-diff;}
			[[nodiscard]]constexpr reverse_iterator operator-(ptrdiff_t diff)const noexcept{return _m+diff;}
			[[nodiscard]]constexpr operator T*()const noexcept{return _m;}
			[[nodiscard]]constexpr bool operator==(T*p)const noexcept{return _m==p;}
			[[nodiscard]]constexpr bool operator==(const reverse_iterator&a)const noexcept{return _m==a._m;}
			[[nodiscard]]constexpr auto operator<=>(T*p)noexcept{return _m<=>p;}
			[[nodiscard]]constexpr bool operator<=>(const reverse_iterator&a)const noexcept{return a._m<=>_m;}
		};
	private:
		T*_begin=nullptr;
		size_t _size=0;
	public:
		constexpr explicit array_like_view_t(T*a,size_t b)noexcept:_begin(a),_size(b){}
		template<class U> requires strict_is_array_like_for<T,U>
		explicit constexpr_as_auto array_like_view_t(U&&a)noexcept_as(begin_of_array_like<T>(a),size_of_array_like<T>(a)):array_like_view_t(begin_of_array_like<T>(a),size_of_array_like<T>(a)){}
		constexpr array_like_view_t(const this_t&)noexcept=default;

		void swap_with(this_t&b)noexcept_as(swap(_begin,b._begin),swap(_size,b._size)){
			swap(_begin,b._begin);
			swap(_size,b._size);
		}

		[[nodiscard]]constexpr const_iterator data()const noexcept{return _begin;}
		[[nodiscard]]constexpr size_t size()const noexcept{return _size;}

		[[nodiscard]]constexpr iterator begin()noexcept requires(type_info<iterator>!=type_info<const_iterator>){return _begin;}
		[[nodiscard]]constexpr iterator end()noexcept requires(type_info<iterator>!=type_info<const_iterator>){return begin()+size();}
		[[nodiscard]]constexpr const_iterator begin()const noexcept{return _begin;}
		[[nodiscard]]constexpr const_iterator end()const noexcept{return begin()+size();}

		[[nodiscard]]constexpr const_iterator cbegin()const noexcept{return remove_const(this)->begin();}
		[[nodiscard]]constexpr const_iterator cend()const noexcept{return remove_const(this)->end();}

		[[nodiscard]]constexpr reverse_iterator rbegin()noexcept{return _begin+size()-1;}
		[[nodiscard]]constexpr reverse_iterator rend()noexcept{return _begin-1;}

		[[nodiscard]]constexpr T&front()noexcept{return _begin[0];}
		[[nodiscard]]constexpr const T&front()const noexcept{return _begin[0];}
		[[nodiscard]]constexpr T&back()noexcept{return _begin[size()-1];}
		[[nodiscard]]constexpr const T&back()const noexcept{return _begin[size()-1];}

		[[nodiscard]]constexpr bool empty()const noexcept{return!size();}

		[[nodiscard]]constexpr T&operator[](size_t pos)noexcept{return begin()[pos];}
		[[nodiscard]]constexpr const T&operator[](size_t pos)const noexcept{return remove_const(*this)[pos];}

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
		using base_t::base_t;

		[[nodiscard]]constexpr static size_t get_length_of(T*ptr)noexcept{
			if(*ptr)return get_length_of(ptr+1)+1;
			else return 0;
		}

		constexpr array_end_by_zero_t(T*ptr)noexcept:base_t(ptr,get_length_of(ptr)){}
	};

	template<class T>
	[[nodiscard]]inline constexpr T* begin_of_array_like(array_like_view_t<T>&a)noexcept requires(type_info<T>!=type_info<const T>){return a.begin();}
	template<class T>
	[[nodiscard]]inline constexpr auto begin_of_array_like(const array_like_view_t<T>&a)noexcept{return a.begin();}
	template<class T>
	[[nodiscard]]inline constexpr T* begin_of_array_like(array_like_view_t<remove_cv<T>>&a)noexcept{return a.begin();}
	template<class T>
	[[nodiscard]]inline constexpr size_t size_of_array_like(const array_like_view_t<T>&a)noexcept{return a.size();}
}
using array_like_n::size_of_array_like;
using array_like_n::begin_of_array_like;
using array_like_n::end_of_array_like;
using array_like_n::is_array_like;
using array_like_n::is_signal_value_for_array_like;
using array_like_n::is_not_signal_value_for_array_like;
using array_like_n::is_array_like_for;
using array_like_n::strict_is_array_like_for;
using array_like_n::array_like_view_t;
using array_like_n::array_end_by_zero_t;

//file_end

