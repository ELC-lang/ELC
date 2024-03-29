//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
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
namespace array_n{
	template<typename T>
	class array_t{
		typedef array_t<T>this_t;
		T*_m;

		/*return{ptr};*/
		constexpr array_t(T*a)noexcept:_m(a){}
		/*返回一个自身的副本*/
		[[nodiscard]]this_t copy()const noexcept(copy_get.nothrow<T>) requires(copy_get.able<T>){
			return{copy_get(_m)};
		}
	public:
		void swap_with(this_t&a)noexcept{swap(_m,a._m);}
	public:
		/*默认构造*/
		constexpr array_t()noexcept:_m(null_ptr){}
		/*
		构造size个T
		*/
		explicit array_t(note::size_t size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value]();
		}
		explicit array_t(note::size_t size,const T&elem)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value](elem);
		}
		// ::std::initializer_list<T>的构造
		array_t(::std::initializer_list<T>&&list)noexcept(get<T>.as_array.nothrow<::std::initializer_list<T>>){
			_m=get<T>.as_array(array_like_view_t<const T>{list});
		}
		// T[N]的构造
		template<size_t N>
		array_t(T(&a)[N])noexcept(get<T>.as_array.nothrow<T(&)[N]>){
			_m=get<T>.as_array(a);
		}
		/*
		此重载适用于range_t<const T*>
		*/
		//template<as_concept<get<T>.as_array.able> U>
		template<class U> requires(get<T>.as_array.able<U> && type_info<remove_cvref<U>>!=type_info<this_t>)
		array_t(U&&a)noexcept(get<T>.as_array.nothrow<U>){
			_m=get<T>.as_array(forward<U>(a));
		}
		/*初始化器进行构建*/
		template<class Func> requires(invoke<Func>.with_return_type<T>.able<>)
		array_t(note::size_t size, Func&&func)noexcept(get<T>.nothrow<Func>){
			_m=get<T>[size.value]();
			for(auto&i:*this)
				i=func();
		}
		template<class Func> requires(invoke<Func>.with_return_type<T>.able<size_t>)
		array_t(note::size_t size, Func&&func)noexcept(get<T>.nothrow<Func>){
			_m=get<T>[size.value]();
			for(size_t i=0;i<size.value;++i)
				(*this)[i]=func(i);
		}

		//复制和移动函数
		array_t(const this_t&a)noexcept_as(declvalue(this_t).copy()):array_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(declvalue(this_t).copy()){
			return operator=(a.copy());
		}
		array_t(this_t&&a)noexcept:array_t(){swap_with(a);}
		this_t&operator=(this_t&&a)&noexcept{
			swap_with(a);
			return*this;
		}

		~array_t()noexcept(unget.nothrow<T>){
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
		}
		static constexpr bool forward_resize_nothrow = get_forward_resize.nothrow<T>;
		void forward_resize(size_t size)noexcept(forward_resize_nothrow){
			get_forward_resize(_m,size);
		}
		static constexpr bool insert_nothrow = get_resize.insert.nothrow<T>;
		void insert(size_t index,size_t count)noexcept(insert_nothrow){
			get_resize.insert(_m,index,count);
		}
		void insert(size_t index,size_t count,const T&value)noexcept(insert_nothrow){
			get_resize.insert(_m,index,count,value);
		}
		void insert(size_t index,size_t count,const T*data)noexcept(insert_nothrow){
			get_resize.insert(_m,index,count,data);
		}
		void insert_with_resize(size_t index,size_t count,const T*data,size_t new_size)noexcept(insert_nothrow){
			get_resize.insert_with_resize(_m,index,count,data,new_size);
		}
		void part_used_insert_with_resize(size_t index,size_t count,const T*data,size_t new_size,size_t used_size)noexcept(insert_nothrow){
			get_resize.insert_with_resize.with_used_size[used_size](_m,index,count,data,new_size);
		}
		static constexpr bool insert_with_forward_resize_nothrow = get_forward_resize.insert_with_resize.nothrow<T>;
		void insert_with_forward_resize(size_t index,size_t count,const T*data,size_t new_size)noexcept(insert_with_forward_resize_nothrow){
			get_forward_resize.insert_with_resize(_m,index,count,data,new_size);
		}
		void part_used_insert_with_forward_resize(size_t index,size_t count,const T*data,size_t new_size,size_t used_size)noexcept(insert_with_forward_resize_nothrow){
			get_forward_resize.insert_with_resize.with_used_size[used_size](_m,index,count,data,new_size);
		}
		[[nodiscard]]bool empty()const noexcept{
			return _m==null_ptr;
		}
		void clear()noexcept(re_construct.nothrow<this_t>){
			re_construct(this);
		}
		[[nodiscard]]T*data()noexcept{return _m;}
		[[nodiscard]]const T*data()const noexcept{return _m;}
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
		typedef reverse_iterator_t<T>reverse_iterator;
		typedef reverse_const_iterator_t<T>reverse_const_iterator;
		[[nodiscard]]reverse_iterator rbegin()noexcept{
			return _m+size()-1;
		}
		[[nodiscard]]constexpr reverse_iterator rend()noexcept{
			return _m-1;
		}
		[[nodiscard]]reverse_const_iterator rbegin()const noexcept{
			return _m+size()-1;
		}
		[[nodiscard]]auto&front()noexcept{
			return*begin();
		}
		[[nodiscard]]auto&front()const noexcept{
			return*begin();
		}
		[[nodiscard]]auto&back()noexcept{
			return*rbegin();
		}
		[[nodiscard]]auto&back()const noexcept{
			return*rbegin();
		}
		[[nodiscard]]constexpr reverse_const_iterator rend()const noexcept{
			return _m-1;
		}
		[[nodiscard]]reverse_const_iterator rcbegin()const noexcept{
			return rbegin();
		}
		[[nodiscard]]constexpr reverse_const_iterator rcend()const noexcept{
			return rend();
		}

		[[nodiscard]]constexpr auto operator<=>(array_like_view_t<const T> a)const noexcept(compare.nothrow<array_like_view_t<T>>){
			return compare(array_like_view_t<const T>(*this),a);
		}
		[[nodiscard]]constexpr auto operator==(array_like_view_t<const T> a)const noexcept(equal.nothrow<array_like_view_t<T>>){
			return equal(array_like_view_t<const T>(*this),a);
		}
		[[nodiscard]]constexpr auto operator<=>(const this_t&a)const noexcept(compare.nothrow<array_like_view_t<T>>){
			return operator<=>(array_like_view_t<const T>(a));
		}
		[[nodiscard]]constexpr auto operator==(const this_t&a)const noexcept(equal.nothrow<array_like_view_t<T>>){
			return operator==(array_like_view_t<const T>(a));
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

		void push_back(const T&a)noexcept(get<T>.apply_end.nothrow<const T&>){
			get<T>.apply_end(note::to<T*&>(_m),a);
		}
		void push_back(T&&a)noexcept(get<T>.apply_end.nothrow<T>){
			get<T>.apply_end(note::to<T*&>(_m),move(a));
		}
		void remove(T a)noexcept(get<T>.remove.nothrow<T>){
			get<T>.remove(a,note::from(_m));
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
		[[nodiscard]]iterator find(U&&a)noexcept{
			return in_range(a,array_like_view_t<T>{*this});
		}
		template<typename U>
		[[nodiscard]]const_iterator find(U&&a)const noexcept{
			return in_range(a,array_like_view_t<const T>{*this});
		}
	};
	template<typename T>
	inline void swap(array_t<T>&a,array_t<T>&b)noexcept{a.swap_with(b);}

	template<class T>
	[[nodiscard]]inline auto size_of_array_like(const array_t<remove_cv<T>>&a)noexcept{return a.size();}
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(array_t<remove_cv<T>>&a)noexcept{return(T*)a.begin();}
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(const array_t<remove_cv<T>>&a)noexcept{return(const T*)a.begin();}
}

//file_end

