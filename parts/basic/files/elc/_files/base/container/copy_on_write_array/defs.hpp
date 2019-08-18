//defs.hpp
//at namespace elc::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace copy_on_write_array_n{
	template<typename T>
	class copy_on_write_array_t{
		static_assert(copy_construct.able<T>,"this type can\'t copy construct.");
		typedef array_t<T>base_t_w;
		typedef copy_on_write_array_t<T>this_t;

		struct data_t:array_t<T>,ref_able<data_t>,build_by_get_only,force_use_default_null_ptr{
			typedef array_t<T>base_t;
			using base_t::base_t;

			data_t(const data_t&a)noexcept_as(construct<base_t>.nothrow<const base_t&>):base_t(a){}
		};
		typedef comn_ptr_t<data_t>ptr_t;

		ptr_t _m;

		static constexpr bool check_nothrow=noexcept(declvalue(ptr_t&)=get<data_t>(*declvalue(ptr_t)));
		void copy_check()noexcept(check_nothrow){
			if(!_m.unique())
				_m=get<data_t>(*_m);
		}
	public:
		copy_on_write_array_t()noexcept:_m(get<data_t>()){}
		explicit copy_on_write_array_t(size_t size)noexcept(get<data_t>.nothrow<size_t>):_m(get<data_t>(size)){}
		~copy_on_write_array_t()noexcept_as(declvalue(ptr_t).~ptr_t())=default;

		copy_on_write_array_t(const this_t&)noexcept=default;
		copy_on_write_array_t(this_t&&)noexcept=default;
		this_t&operator=(const this_t&)noexcept(ptr_t::reset_nothrow)=default;
		this_t&operator=(this_t&&)noexcept(ptr_t::reset_nothrow)=default;

		void swap(this_t&a)noexcept{_m.swap(a._m);}
		void swap(base_t_w&a)noexcept(check_nothrow){
			copy_check();
			a.swap(*_m);
		}
		this_t&operator=(const base_t_w&a)&noexcept(check_nothrow){
			copy_check();
			*_m=a;
			return*this;
		}
		operator base_t_w&()noexcept(check_nothrow){
			copy_check();
			return*_m;
		}
		[[nodiscard]]explicit operator hash_t()noexcept{return hash(_m);}

		[[nodiscard]]T&operator[](size_t size)noexcept(check_nothrow){
			copy_check();
			return(*_m)[size];
		}
		[[nodiscard]]const T&operator[](size_t size)const noexcept{return(*_m)[size];}

		size_t size()const noexcept{
			return _m->size();
		}
		void resize(size_t size)noexcept(get_resize.nothrow<T>&&check_nothrow){
			copy_check();
			_m->resize(size);
		}
	};
	template<typename T>
	inline void swap(copy_on_write_array_t<T>&a,copy_on_write_array_t<T>&b)noexcept{a.swap(b);}
	template<typename T>
	inline void swap(array_t<T>&a,copy_on_write_array_t<T>&b)noexcept{b.swap(a);}
	template<typename T>
	inline void swap(copy_on_write_array_t<T>&a,array_t<T>&b)noexcept{a.swap(b);}
}
