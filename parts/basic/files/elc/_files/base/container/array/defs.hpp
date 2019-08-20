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
		explicit array_t(size_t size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size]();
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

		void swap(this_t&a)noexcept{
			using ::std::swap;
			swap(_m,a._m);
		}
	private:
		template<enable_if(copy_get.able<T>)>
		this_t copy()const noexcept_as(copy_get.nothrow<T>){
			return{copy_get(_m)};
		}
	public:
		array_t(this_t&&a):array_t()noexcept{
			swap(a);
		}
		this_t&operator=(this_t&&a)&noexcept{
			swap(a);
			return*this;
		}
		#define expr declvalue(this_t).copy()
		array_t(const this_t&a)noexcept_as(expr):array_t(a.copy()){}
		this_t&operator=(const this_t&a)&noexcept_as(expr){
			return operator=(a.copy());
		}
		#undef expr
	};
	template<typename T>
	inline void swap(array_t<T>&a,array_t<T>&b)noexcept{a.swap(b);}
}
