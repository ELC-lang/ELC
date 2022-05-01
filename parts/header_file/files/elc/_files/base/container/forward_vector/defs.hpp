//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace forward_vector_n{
	template<typename T>
	class forward_vector_t{
		typedef forward_vector_t<T>this_t;
		T*_m;
		#if defined(DEBUG) || defined(_DEBUG)
			size_t _size_for_debug_view=0;
		#endif

		/*return{ptr};*/
		constexpr forward_vector_t(T*a):_m(a){}
		/*返回一个自身的副本*/
		[[nodiscard]]this_t copy()const noexcept(copy_get.nothrow<T>) requires(copy_get.able<T>){
			#if defined(DEBUG) || defined(_DEBUG)
				return{copy_get(_m),_size_for_debug_view};
			#else
				return{copy_get(_m)};
			#endif
		}
	public:
		void swap_with(this_t&a)noexcept{
			swap(_m,a._m);
			#if defined(DEBUG) || defined(_DEBUG)
				swap(_size_for_debug_view,a._size_for_debug_view);
			#endif
		}
	public:
		/*默认构造*/
		constexpr forward_vector_t()noexcept:_m(null_ptr){}
		/*
		构造size个T
		*/
		explicit forward_vector_t(note::size_t<size_t>size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value]();
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size.value;
			#endif
		}
		/*
		此重载适用于T[N]，std::init_list<T>以及range_t<const T*>
		*/
		//template<as_concept<get<T>.as_forward_vector.able> U>
		template<class U> requires(get<T>.as_forward_vector.able<U>)
		forward_vector_t(U&&a)noexcept(get<T>.as_forward_vector.nothrow<U>){
			_m=get<T>.as_forward_vector(forward<U>(a));
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size();
			#endif
		}

		//复制和移动函数
		forward_vector_t(const this_t&a)noexcept_as(declvalue(this_t).copy()):forward_vector_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(declvalue(this_t).copy()){
			return operator=(a.copy());
		}
		forward_vector_t(this_t&&a)noexcept:forward_vector_t(){swap_with(a);}
		this_t&operator=(this_t&&a)&noexcept{
			swap_with(a);
			return*this;
		}

		~forward_vector_t()noexcept(unget.nothrow<T>){
			unget(_m);
		}
		[[nodiscard]]size_t size()const noexcept{
			return get_size_of_get(_m);
		}
		[[nodiscard]]size_t size_in_byte()const noexcept{
			return size()*sizeof(T);
		}
		static constexpr bool resize_nothrow = get_resize.nothrow<T>;
		void resize(size_t size)noexcept(resize_nothrow){
			get_resize(_m,size);
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size;
			#endif
		}
		static constexpr bool forward_resize_nothrow = get_forward_resize.nothrow<T>;
		void forward_resize(size_t size)noexcept(forward_resize_nothrow){
			get_forward_resize(_m,size);
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size;
			#endif
		}
		[[nodiscard]]bool empty()const{
			return _m==null_ptr;
		}
		void clear()noexcept(re_construct.nothrow<this_t>){
			re_construct(this);
		}
		[[nodiscard]]T&operator[](size_t pos)noexcept{return _m[pos];}
		[[nodiscard]]const T&operator[](size_t pos)const noexcept{return _m[pos];}
		[[nodiscard]]explicit operator hash_t()const noexcept(hash.nothrow<T>){return hash(_m,size());}
		[[nodiscard]]explicit operator T*()noexcept{return _m;}
		[[nodiscard]]explicit operator const T*()const noexcept{return _m;}

		typedef iterator_t<T>iterator;
		typedef const_iterator_t<T>const_iterator;

		[[nodiscard]]constexpr iterator get_iterator_at(size_t a)noexcept{
			return _m+a;
		}
		[[nodiscard]]constexpr const_iterator get_iterator_at(size_t a)const noexcept{
			return _m+a;
		}
		[[nodiscard]]constexpr iterator begin()noexcept{
			return get_iterator_at(zero);
		}
		[[nodiscard]]iterator end()noexcept{
			return get_iterator_at(size());
		}
		[[nodiscard]]constexpr const_iterator begin()const noexcept{
			return get_iterator_at(zero);
		}
		[[nodiscard]]const_iterator end()const noexcept{
			return get_iterator_at(size());
		}
		[[nodiscard]]const_iterator cbegin()const noexcept{
			return begin();
		}
		[[nodiscard]]const_iterator cend()const noexcept{
			return end();
		}

		[[nodiscard]]constexpr auto operator<=>(forward_vector_like_view_t<const T> a)const noexcept(compare.nothrow<forward_vector_like_view_t<T>>){
			return compare(forward_vector_like_view_t<const T>(*this),a);
		}
		[[nodiscard]]constexpr auto operator==(forward_vector_like_view_t<const T> a)const noexcept(equal.nothrow<forward_vector_like_view_t<T>>){
			return equal(forward_vector_like_view_t<const T>(*this),a);
		}
		[[nodiscard]]constexpr auto operator<=>(const this_t&a)const noexcept(compare.nothrow<forward_vector_like_view_t<T>>){
			return operator<=>(forward_vector_like_view_t<const T>(a));
		}
		[[nodiscard]]constexpr auto operator==(const this_t&a)const noexcept(equal.nothrow<forward_vector_like_view_t<T>>){
			return operator==(forward_vector_like_view_t<const T>(a));
		}

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t> requires was_not_an_ill_form(expr)
		void for_each(func_t&&func)noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const T&))
		template<typename func_t> requires was_not_an_ill_form(expr)
		void for_each(func_t&&func)const noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr

		void push_back(const T&a){
			get<T>.apply_end(note::to<T*&>(_m),a);
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size();
			#endif
		}
		void push_back(T&&a){
			get<T>.apply_end(note::to<T*&>(_m),move(a));
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size();
			#endif
		}
		void remove(T a){
			get<T>.remove(a,note::form(_m));
			#if defined(DEBUG) || defined(_DEBUG)
				_size_for_debug_view=size();
			#endif
		}
		//template<as_concept<get<T>.apply_end.able> U>
		template<class U,enable_if(get<T>.apply_end.able)>
		friend this_t&operator+=(this_t&a,U&&b)noexcept(get<T>.apply_end.nothrow<U>){
			get<T>.apply_end(note::to(a._m),b);
			return a;
		}
		friend this_t&operator+=(this_t&a,zero_t b)noexcept(get<T>.apply_end.nothrow<T>){
			return a+=T(0);
		}
		template<typename U>
		friend this_t operator+(const this_t&a,U&&b)noexcept_as(a.copy()+=b) requires was_not_an_ill_form(a.copy()+=b){
			return a.copy()+=b;
		}
		template<typename U>
		friend this_t operator+(U&&a,const this_t&b)noexcept_as(this_t(a)+=b) requires was_not_an_ill_form(this_t(a)+=b){
			return this_t(a)+=b;
		}

		template<typename U>
		this_t&& operator+(U&& b)&&noexcept_as(*this+=b) requires was_not_an_ill_form(*this+=b){//对右值的operator+优化为operator+=
			return *this+=b;
		}


		template<typename U>
		[[nodiscard]]iterator find(U&&a)noexcept requires was_not_an_ill_form(in_range(declvalue(this_t),a)){
			return in_range(*this,a);
		}
		template<typename U>
		[[nodiscard]]const_iterator find(U&&a)const noexcept requires was_not_an_ill_form(in_range(declvalue(this_t),a)){
			return in_range(*this,a);
		}
	};
	template<typename T>
	inline void swap(forward_vector_t<T>&a,forward_vector_t<T>&b)noexcept{a.swap_with(b);}

	template<class T>
	[[nodiscard]]inline auto size_of_forward_vector_like(const forward_vector_t<remove_cv<T>>&a)noexcept{return a.size();}
	template<class T>
	[[nodiscard]]inline auto begin_of_forward_vector_like(forward_vector_t<remove_cv<T>>&a)noexcept{return(T*)a.begin();}
	template<class T>
	[[nodiscard]]inline auto begin_of_forward_vector_like(const forward_vector_t<remove_cv<T>>&a)noexcept{return(const T*)a.begin();}
}

//file_end

