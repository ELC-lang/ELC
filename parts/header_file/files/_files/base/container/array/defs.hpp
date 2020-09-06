//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_n{
	template<typename T>
	class array_t{
		typedef array_t<T>this_t;
		T*_m;
		constexpr array_t(T*a):_m(a){}
	public:
		constexpr array_t():_m(null_ptr){}
		explicit array_t(note::size_t<size_t>size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size.value]();
		}
		array_t(const ::std::initializer_list<T>&init_list)noexcept(copy_construct.nothrow<T>){
			_m=alloc<T>[init_list.size()]();//get==alloc+construct
			size_t index=0;
			for(const T&v:init_list)
				copy_construct(note::form(&v),note::to(_m+(index++)));
		}
		~array_t()noexcept(unget.nothrow<T>){
			unget(_m);
		}
		[[nodiscard]]size_t size()const noexcept{
			return get_size_of_get(_m);
		}
		void resize(size_t size)noexcept(get_resize.nothrow<T>){
			get_resize(_m,size);
		}
		[[nodiscard]]T&operator[](size_t size)noexcept{return _m[size];}
		[[nodiscard]]const T&operator[](size_t size)const noexcept{return _m[size];}
		[[nodiscard]]explicit operator hash_t()noexcept{return hash(_m);}

		typedef iterator_t<T>iterator;
		typedef iterator_t<const T>const_iterator;

		[[nodiscard]]constexpr iterator get_iterator_at(size_t a)noexcept{
			return _m+a;
		}
		[[nodiscard]]constexpr iterator begin()noexcept{
			return iteratorget_iterator_at(zero);
		}
		[[nodiscard]]iterator end()noexcept{
			return iteratorget_iterator_at(size());
		}
		[[nodiscard]]const_iterator cbegin()const noexcept{
			return const_cast<this_t*>(this)->begin();
		}
		[[nodiscard]]const_iterator cend()const noexcept{
			return const_cast<this_t*>(this)->end();
		}

		[[nodiscard]]bool empty()const{
			return _m==null_ptr;
		}

		void swap_with(this_t&a)noexcept{
			using ::elc::defs::swap;
			swap(_m,a._m);
		}
	private:
		template<enable_if(copy_get.able<T>)>
		[[nodiscard]]this_t copy()const noexcept(copy_get.nothrow<T>){
			return{copy_get(_m)};
		}
	public:
		array_t(this_t&&a)noexcept:array_t(){swap(a);}
		this_t&operator=(this_t&&a)&noexcept{
			swap_with(a);
			return*this;
		}
		#define expr declvalue(this_t).copy()
		array_t(const this_t&a)noexcept_as(expr):array_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(expr){
			return operator=(a.copy());
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)const noexcept_as(expr){
			auto asize=size();
			while(asize--)
				func((*this)[asize]);
		}
		#undef expr
		/*
		void add(T a){
			resize(size()+1);
			operator[](size()-1)=a;
		}

		void remove(T a){
			size_t size=this->size();
			if(size){
				size_t i=0;
				this_t&this_e=*this;
				while(this_e[i++]!=a && i!=size);
				destruct(&this_e[--i]);
				while(++i!=size){
					copy_construct(note::from(&this_e[i]),note::to(&this_e[i-1]));
					destruct(&this_e[i]);
				}
				construct<T>[&this_e[szie-1]]();
				resize(size-1);
			}
		}
		*/
	};
	template<typename T>
	inline void swap(array_t<T>&a,array_t<T>&b)noexcept{a.swap_with(b);}
}

//file_end

