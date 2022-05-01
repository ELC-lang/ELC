//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace vector_n{
	template<typename T>
	class vector_t{
		typedef vector_t<T>this_t;
		T*_m;
		size_t _used_size;

		/*return{ptr,size};*/
		constexpr vector_t(T*a,size_t used_size):_m(a),_used_size(used_size){}
		/*返回一个自身的副本*/
		[[nodiscard]]this_t copy()const noexcept(copy_get.nothrow<T>) requires(copy_get.able<T>){
			return{copy_get(_m),_used_size};
		}
	public:
		void swap_with(this_t&a)noexcept{
			swap(_m,a._m);
			swap(_used_size,a._used_size);
		}
	public:
		/*默认构造*/
		constexpr vector_t()noexcept:_m(null_ptr),_used_size(0){}
		/*
		构造size个T
		*/
		explicit vector_t(note::size_t<size_t>size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value]();
			_used_size=size.value;
		}
		/*
		此重载适用于T[N]，std::init_list<T>以及range_t<const T*>
		*/
		//template<as_concept<get<T>.as_array.able> U>
		template<class U> requires(get<T>.as_array.able<U>)
		vector_t(U&&a)noexcept(get<T>.as_array.nothrow<U>){
			_m=get<T>.as_array(forward<U>(a));
			_used_size=get_size_of_get(_m);
		}

		//复制和移动函数
		vector_t(const this_t&a)noexcept_as(declvalue(this_t).copy()):vector_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(declvalue(this_t).copy()){
			return operator=(a.copy());
		}
		vector_t(this_t&&a)noexcept:vector_t(){swap_with(a);}
		this_t&operator=(this_t&&a)&noexcept{
			swap_with(a);
			return*this;
		}

		~vector_t()noexcept(unget.nothrow<T>){
			unget(_m);
		}
		[[nodiscard]]size_t capacity()const noexcept{
			return get_size_of_get(_m);
		}
		[[nodiscard]]size_t size()const noexcept{
			return _used_size;
		}
		[[nodiscard]]size_t size_in_byte()const noexcept{
			return size()*sizeof(T);
		}
		static constexpr bool resize_nothrow = get_resize.nothrow<T>;
		void resize(size_t size)noexcept(resize_nothrow){
			if(size<=_used_size){
				re_construct[_m+size][_used_size-size]();
				_used_size=size;
				return;
			}
			if(size<=capacity()){
				_used_size=size;
				return;
			}
			get_resize(_m,get_next_gold_size_to_resize_for_array(size));
			_used_size=size;
		}
		[[nodiscard]]bool empty()const{
			return _used_size==0;
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

		void push_back(const T&a){
			resize(_used_size+1);
			(*this)[_used_size-1]=a;
		}
		void push_back(T&&a){
			resize(_used_size+1);
			(*this)[_used_size-1]=move(a);
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
	inline void swap(vector_t<T>&a,vector_t<T>&b)noexcept{a.swap_with(b);}

	template<class T>
	[[nodiscard]]inline auto size_of_array_like(const vector_t<remove_cv<T>>&a)noexcept{return a.size();}
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(vector_t<remove_cv<T>>&a)noexcept{return(T*)a.begin();}
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(const vector_t<remove_cv<T>>&a)noexcept{return(const T*)a.begin();}
}

//file_end

