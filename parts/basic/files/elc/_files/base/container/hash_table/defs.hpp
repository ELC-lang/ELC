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
	class hash_table_t{
		typedef hash_table_t<T,stack_t,bucket_max_size>this_t;
		typedef stack_t<T>bucket_t;
		typedef array_t<bucket_t>base_t_w;

		base_t_w _m;
	public:
		template<size_t _>
		void swap(hash_table_t<T,stack_t,_>&a)noexcept{swap(_m,a._m);}
	private:
		[[nodiscard]]bucket_t&find_bucket(hash_t a)noexcept{
			return _m[a%_m.size()];
		}
		void bucket_count_grow()noexcept{
			this_t tmp;
			tmp._m.resize(size_t(this->_m.size()*magic_number::gold_for_resize));
			_m.for_each([&tmp](bucket_t&a)noexcept{
				while(a.move_top_to(tmp.find_bucket(a.get_top_hash())));
			});
			swap(tmp);
		}
		hash_table_t(const base_t_w&a):_m(a)noexcept{}
		this_t copy()noexcept_as(copy_construct.nothrow<base_t_w>){
			return{_m};
		}
	public:
		hash_table_t():_m(2){}
		~hash_table_t()noexcept(destruct.nothrow<base_t_w>)=default;
		hash_table_t(const this_t&a):_m(a._m)noexcept{}
		hash_table_t(this_t&&a):_m(a._m)noexcept{}

		this_t&operator=(this_t&&a)noexcept{
			swap(_m,a._m);
			return*this;
		}
		this_t&operator=(const this_t&a)noexcept{
			return operator=(a.copy());
		}

		template<typename T_>
		static constexpr bool hash_nothrow=noexcept(hash(declvalue(T_)));
		template<typename T_>
		static constexpr bool find_nothrow=hash_nothrow<T_>&&noexcept(declvalue(bucket_t).find(declvalue(T_)));

		void add(const T&a)noexcept(hash_nothrow<const T&>&&bucket_t::add_nothrow){
			if(find_bucket(hash(a)).add(a) > bucket_max_size)
				bucket_count_grow();
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
		//UF
		//for_each
	};
	template<typename T,template<typename>class stack_t,size_t _,size_t __>
	inline void swap(hash_table_t<T,stack_t,_>&a,hash_table_t<T,stack_t,__>&b)noexcept{a.swap(b);}
}
