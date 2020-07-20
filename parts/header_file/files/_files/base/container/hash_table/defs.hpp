//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace hash_table_n{
	template<typename T,template<typename>class stack_t=auto_stack_t,size_t bucket_max_size=256>
	class hash_table_t:container_struct{
		typedef hash_table_t<T,stack_t,bucket_max_size>this_t;
		typedef stack_t<T>bucket_t;
	public:
		typedef array_t<bucket_t>base_t_w;
	private:
		base_t_w _m;
	public:
		void swap(base_t_w&a)noexcept{swap(_m,a);}
	private:
		[[nodiscard]]bucket_t&find_bucket(hash_t a)noexcept{
			return _m[a%_m.size()];
		}
		void bucket_count_grow()noexcept{
			this_t tmp;
			tmp._m.resize(size_t(this->_m.size()*magic_number::gold_of_resize));
			_m.for_each(lambda_with_catch(&tmp)(bucket_t&a)noexcept{
				while(!a.empty())
					a.move_top_to(tmp.find_bucket(a.get_top_hash()));
			});
			swap(tmp);
		}
		hash_table_t(const base_t_w&a):_m(a)noexcept{}
		this_t copy()noexcept(copy_construct.nothrow<base_t_w>){
			return{_m};
		}
	public:
		hash_table_t():_m(2){}
		~hash_table_t()noexcept(destruct.nothrow<base_t_w>)=default;

		operator base_t_w&()noexcept{return _m;}
		operator const base_t_w&()const noexcept{return _m;}

		hash_table_t(const this_t&a):_m(a._m)noexcept{}
		hash_table_t(this_t&&a):_m(a._m)noexcept{}

		this_t&operator=(base_t_w&&a)&noexcept{
			swap(_m,a);
			return*this;
		}
		this_t&operator=(const base_t_w&a)&noexcept{
			_m=a;
			return*this;
		}

		template<typename T_>
		static constexpr bool hash_nothrow=noexcept(hash(declvalue(T_)));
		template<typename T_>
		static constexpr bool find_nothrow=hash_nothrow<T_>&&noexcept(declvalue(bucket_t).find(declvalue(T_)));

		void add(const T&a)noexcept(hash_nothrow<const T&>&&bucket_t::add_nothrow){
			if(find_bucket(hash(a)).add(a) > bucket_max_size)
				bucket_count_grow();
		}
		void remove(const T&a)noexcept(bucket_t::remove_nothrow){
			find_bucket(hash(a)).remove(a);
		}
		template<typename T_>
		[[nodiscard]]maybe_fail_reference<T>find(T_&&a)noexcept(find_nothrow<T_>){
			return find_bucket(hash(a)).find(a);
		}
		[[nodiscard]]bool in_table(const T&a)noexcept(find_nothrow<T_>){
			return find(a).not_fail();
		}

		void clear()noexcept(destruct.nothrow<this_t>&&construct<this_t>.nothrow<>){
			destruct(this);
			construct<this_t>[this]();
		}

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)noexcept_as(expr){
			_m.for_each(lambda(bucket_t&a)noexcept_as(expr){
				a.for_each(func);
			});
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)const noexcept_as(expr){
			_m.for_each(lambda(const bucket_t&a)noexcept_as(expr){
				a.for_each(func);
			});
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(bucket_t&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each_bucket(func_t&&func)noexcept_as(expr){
			_m.for_each(func);
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const bucket_t&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each_bucket(func_t&&func)const noexcept_as(expr){
			_m.for_each(func);
		}
		#undef expr
	};
	template<typename T,template<typename>class stack_t,size_t _,size_t __>
	inline void swap(hash_table_t<T,stack_t,_>&a,hash_table_t<T,stack_t,__>&b)noexcept{a.swap(b);}
	template<typename T,template<typename>class stack_t,size_t _>
	inline void swap(hash_table_t<T,stack_t,_>&a,typename hash_table_t<T,stack_t,_>::base_t_w&b)noexcept{a.swap(b);}
	template<typename T,template<typename>class stack_t,size_t _>
	inline void swap(typename hash_table_t<T,stack_t,_>::base_t_w&b,hash_table_t<T,stack_t,_>&a)noexcept{a.swap(b);}
}
