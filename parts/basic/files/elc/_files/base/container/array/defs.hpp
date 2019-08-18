//defs.hpp
//at namespace elc::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace array_n{
	template<typename T>
	class array:non_copyable,non_moveable{
		T*_m;
	public:
		constexpr array():_m(null_ptr){}
		explicit array(size_t size)noexcept(get<T>.nothrow<>){
			_m=get<T>[size]();
		}
		~array()noexcept(unget.nothrow<T>){
			unget(_m);
		}
		size_t size()const noexcept{
			return get_size_of_get(_m);
		}
		void resize(size_t size)noexcept(get_resize.nothrow<T>){
			get_resize(_m,size);
		}
		T&operator[](size_t size)noexcept{return _m[size];}
		const T&operator[](size_t size)const noexcept{return _m[size];}
	};
}
