//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace copy_on_write_array_n{
	template<typename T>
	class copy_on_write_array_t:container_struct{
		static_assert(copy_construct.able<T>,"this type can\'t copy construct.");
		typedef array_t<T>base_t_w;
		typedef copy_on_write_array_t<T>this_t;

		struct data_t:array_t<T>,ref_able<data_t>,build_by_get_only,never_in_array<data_t>,force_use_default_null_ptr{
			typedef array_t<T>base_t;
			using base_t::base_t;

			data_t(const data_t&a)noexcept(construct<base_t>.nothrow<const base_t&>):base_t(a){}
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
		explicit copy_on_write_array_t(note::size_t<size_t>size)noexcept(get<data_t>.nothrow<size_t>):_m(get<data_t>(size.value)){}
		copy_on_write_array_t(const ::std::initializer_list<T>&init_list)noexcept(get<data_t>.nothrow<const ::std::initializer_list<T>&>):_m(get<data_t>(init_list)){}
		~copy_on_write_array_t()noexcept_as(declvalue(ptr_t).~ptr_t())=default;

		copy_on_write_array_t(const this_t&)noexcept=default;
		copy_on_write_array_t(this_t&&)noexcept=default;
		this_t&operator=(const this_t&)noexcept(ptr_t::reset_nothrow)=default;
		this_t&operator=(this_t&&)noexcept(ptr_t::reset_nothrow)=default;

		void swap_with(this_t&a)noexcept{_m.swap_with(a._m);}
		void swap_with(base_t_w&a)noexcept(check_nothrow){
			copy_check();
			a.swap_with(*_m);
		}
		this_t&operator=(const base_t_w&a)&noexcept(check_nothrow){
			copy_check();
			static_cast<base_t_w&>(*_m)=a;
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

	private:
		struct base_iterator_t{
			this_t*_ptr;
			size_t _size;

			[[nodiscard]]T*get_handle()noexcept(check_nothrow){
				return (*_ptr)[_size];
			}
			[[nodiscard]]base_iterator_t next_getter(){
				return {_ptr,_size+1};
			}
			[[nodiscard]]base_iterator_t before_getter(){
				return {_ptr,_size-1};
			}
		};
		struct base_const_iterator_t{
			const this_t*_ptr;
			size_t _size;

			constexpr base_const_iterator_t(const this_t*a,size_t b)noexcept:_ptr(a),_size(b){}
			constexpr base_const_iterator_t(const base_const_iterator_t&)noexcept=default;
			constexpr base_const_iterator_t(base_iterator_t&a)noexcept:_ptr(a._ptr),_size(a._size){}
			[[nodiscard]]const T*get_handle()noexcept{
				return (*_ptr)[_size];
			}
			[[nodiscard]]base_const_iterator_t next_getter(){
				return {_ptr,_size+1};
			}
			[[nodiscard]]base_const_iterator_t before_getter(){
				return {_ptr,_size-1};
			}
		};
	public:
		typedef iterator_t<T,base_iterator_t>iterator;
		typedef iterator_t<const T,base_const_iterator_t>const_iterator;

		[[nodiscard]]constexpr iterator get_iterator_at(size_t a)noexcept{
			return {this,a};
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

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)noexcept(check_nothrow&&noexcept(expr)){
			copy_check();
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
	};
	template<typename T>
	inline void swap(copy_on_write_array_t<T>&a,copy_on_write_array_t<T>&b)noexcept{a.swap_with(b);}
	template<typename T>
	inline void swap(array_t<T>&a,copy_on_write_array_t<T>&b)noexcept{b.swap_with(a);}
	template<typename T>
	inline void swap(copy_on_write_array_t<T>&a,array_t<T>&b)noexcept{a.swap_with(b);}
}

//file_end

