//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace map_n{
	template<typename T,typename key_t,template<typename>class stack_t=auto_stack_t,size_t bucket_max_size=256>
	class map_t:container_struct{
		typedef map_t<T,key_t,stack_t,bucket_max_size>this_t;

		struct data_t{
			key_t _key;
			T _value;

			struct seek_value_t{
				const T*_m;
				constexpr seek_value_t(const T&a):_m(&a){};
			}

			bool operator==(const key_t&a)noexcept_as(declvalue(key_t&)==declvalue(const key_t&)){
				return _key==a;
			}
			bool operator==(const seek_value_t&a)noexcept_as(declvalue(T&)==declvalue(const T&)){
				return _value==*a._m;
			}
			constexpr_as(hash(declvalue(key_t&)))operator hash_t()noexcept_as(hash(declvalue(key_t&))){
				return hash(_key);
			}
			/* operator T&()noexcept{
				return _value;
			} */
			[[nodiscard]]bool empty(){
				return _value==const_default_value_of<T>;
			}
		};
		typedef hash_table_t<data_t,stack_t,bucket_max_size>base_t_w;

		mutable base_t_w _m;//mutable cause shrink.

		map_t(const base_t_w&a):_m(a)noexcept{}
		this_t copy()noexcept(copy_construct.nothrow<base_t_w>){
			return{_m};//不用疑惑，这是deep copy
		}
	public:
		map_t()noexcept=default;
		~map_t()noexcept(destruct.nothrow<base_t_w>)=default;
		map_t(const this_t&a):_m(a._m)noexcept{}
		map_t(this_t&&a):_m(a._m)noexcept{}

		this_t&operator=(this_t&&a)noexcept{
			swap(_m,a._m);
			return*this;
		}
		this_t&operator=(const this_t&a)noexcept{
			return operator=(a.copy());
		}

		template<size_t _>
		void swap(map_t<T,stack_t,_>&a)noexcept{swap(_m,a._m);}

		T&operator[](const key_t&a){
			auto tmp=_m.find(a);
			if(tmp.fail()){
				_m.add({a,T()});
				tmp=_m.find(a);
			}
			return tmp.get()._value;
		}
		const T&operator[](const key_t&a)const{
			auto tmp=_m.find(a);
			return tmp.fail()?const_default_value_of<T>:tmp.get()._value;
		}
		void clear()noexcept(re_construct.nothrow<this_t>){
			re_construct(this);
		}

		#define expr declvalue(func_t)(declvalue(T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)noexcept_as(expr){
			_m.for_each(lambda(data_t&a)noexcept_as(expr){
				func(a->_value);
			});
		}
		#undef expr

		#define expr declvalue(func_t)(declvalue(const T&))
		template<typename func_t,enable_if_not_ill_form(expr)>
		void for_each(func_t&&func)const noexcept_as(expr){
			_m.for_each(lambda(data_t&a)noexcept_as(expr){
				func(const_cast<const T&>(a->_value));
			});
		}
		#undef expr

		static constexpr bool shrink_nothow=stack_t::remove_nothrow;
		void shrink()noexcept(shrink_nothow){
			_m.for_each_bucket(lambda(stack_t&a)noexcept(shrink_nothow){
				while(a.remove(data_t::seek_value_t(const_default_value_of<T>)));
			});
		}
	};
	template<typename T,template<typename>class stack_t,size_t _,size_t __>
	inline void swap(map_t<T,stack_t,_>&a,map_t<T,stack_t,__>&b)noexcept{a.swap(b);}
}

//file_end

