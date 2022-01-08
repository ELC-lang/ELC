//base_function_t.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	#if !defined(_MSC_VER)
	template<class T>
	class base_func_data_t;
	template<class Ret_t,class...Args_t>
	struct base_func_data_t<Ret_t(Args_t...)>:type_info_t<base_func_data_t<Ret_t(Args_t...)>>::template_name with_common_attribute<abstract_base,ref_able,never_in_array>,build_by_get_only{
		typedef base_func_data_t<Ret_t(Args_t...)>this_t;

		virtual ~base_func_data_t()=default;
		virtual Ret_t call(Args_t...)=0;
		//for equal:
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept=0;
		[[nodiscard]]virtual const void*get_data_begin()const noexcept=0;
		[[nodiscard]]virtual bool equal_with(const void*)const=0;
		[[nodiscard]]bool operator==(const this_t&a)const{
			return this->get_type_info()==a.get_type_info()&&this->equal_with(a.get_data_begin());
		}
		template<typename T>
		[[nodiscard]]bool operator==(T&&a)const{
			return this->get_type_info()==type_info<T>&&this->equal_with(addressof(a));
		}
	};

	template<class T,class Func_t>
	class func_data_t;
	template<class T,class Ret_t,class...Args_t>
	struct func_data_t<T,Ret_t(Args_t...)>:
	type_info_t<func_data_t<T,Ret_t(Args_t...)>>::template_name with_common_attribute<instance_struct>,
	base_func_data_t<Ret_t(Args_t...)>,function_data_warpper_t<T,Ret_t(Args_t...)>{
		static_assert(!::std::is_function_v<T>);
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;
		typedef function_data_warpper_t<T,Ret_t(Args_t...)>data_t;

		using data_t::data_t;
		virtual ~func_data_t()override=default;
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<T>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return addressof(data_t::get_data());}
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept(equal.able<T>?equal.nothrow<T>:true) override{
			if constexpr(equal.able<T>)
				return data_t::_value==*reinterpret_cast<const T*>(a);
			else
				return false;
		}
		[[nodiscard]]virtual Ret_t call(Args_t...args)override{
			return data_t::operator()(forward<Args_t>(args)...);
		}
	};

	template<class T>
	class default_func_data_t;
	template<class Ret_t,class...Args_t>
	struct default_func_data_t<Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		virtual ~default_func_data_t()noexcept override{}
		virtual Ret_t call(Args_t...)override{return Ret_t();}
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<void>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return null_ptr;}//这玩意实际上用不到，艹
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept override{return true;}
	};
	template<class Ret_t,class...Args_t>
	inline constexpr default_func_data_t<Ret_t(Args_t...)>default_func_data{};

	template<class Ret_t,class...Args_t>
	[[nodiscard]]constexpr base_func_data_t<Ret_t(Args_t...)>*get_null_ptr(type_info_t<base_func_data_t<Ret_t(Args_t...)>>)noexcept{
		return&default_func_data<Ret_t,Args_t...>;
	}

	template<class T>
	class function_data_saver_t;
	template<class Ret_t,class...Args_t>
	class function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>this_t;
		typedef base_func_data_t<Ret_t(Args_t...)> base_t_w;
		typedef comn_ptr_t<base_t_w>ptr_t;

		ptr_t _m;
		void swap_with(this_t&a)noexcept{swap(_m,a._m);}
	public:
		function_data_saver_t()noexcept=default;
		function_data_saver_t(const this_t&a)noexcept:_m(a._m){}
		function_data_saver_t(this_t&&a)noexcept{swap_with(a);}
		[[nodiscard]]bool operator==(const this_t&a)const{
			return *_m==*(a._m);
		}
		template<typename T> requires(equal.able<base_t_w,T>)
		[[nodiscard]]bool operator==(T&&a)const noexcept(equal.nothrow<base_t_w,T>){
			return *_m==a;
		}
		void operator=(const this_t&a){_m=a._m;}
		Ret_t call(Args_t&&...rest)const{return _m->call(forward<Args_t>(rest)...);}
	};

	template<class T,bool promise_nothrow_at_destruct>
	class base_function_t;
	template<class Ret_t,class...Args_t,bool nothrow,bool promise_nothrow_at_destruct>
	struct base_function_t<Ret_t(Args_t...)noexcept(nothrow),promise_nothrow_at_destruct>:function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>base_t;
		typedef base_function_t<Ret_t(Args_t...)noexcept(nothrow),promise_nothrow_at_destruct>this_t;

		template<class,bool>
		friend class base_function_t;

		template<class U>
		using func_data_t=function_n::func_data_t<U,Ret_t(Args_t...)>;

		typedef Ret_t(*func_ptr_t)(Args_t...)noexcept(nothrow);

		using base_t::ptr_t;
		using base_t::_m;

		template<class T>
		static constexpr bool base_on_this_t_or_more_stringent_restrictions=(
			type_info<T>.base_on<this_t>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept(bool(nothrow+1)),	promise_nothrow_at_destruct			>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept(nothrow),			bool(promise_nothrow_at_destruct+1)	>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept(bool(nothrow+1)),	bool(promise_nothrow_at_destruct+1)	>>
		);
	public:
		void swap_with(this_t&a)noexcept{//不与base_t::swap_with重复：与更加严格（或宽松）的this_t进行swap是错误的
			base_t::swap_with(a);
		}

		base_function_t()noexcept=default;
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		base_function_t(const T&a)noexcept:base_t(a){}
		base_function_t(const this_t&a)noexcept:base_t(a){}
		base_function_t(this_t&&a)noexcept:base_function_t(){
			swap_with(a);
		}
		base_function_t(nullptr_t)noexcept:base_function_t(){}
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		template<class T> requires(invoke<T>.able<Args_t...> && not base_on_this_t_or_more_stringent_restrictions<T>)
		base_function_t(T&&a)noexcept(get<func_data_t<remove_cvref<T>>>.nothrow<T>){
			//BLOCK:constexpr checks
			if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
				template_error("unexpected assign.");
			if constexpr(nothrow)
				if constexpr(!invoke<T>.nothrow<Args_t...>)
					template_warning("the call of T was not noexcept,this may cause terminate.");
			//BLOCK_END
			_m=get<func_data_t<remove_cvref<T>>>(a);
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		template<class T>
		this_t&operator=(T&&a)&noexcept_as(declvalue(this_t).swap_with((this_t&)this_t(a))) requires was_not_an_ill_form(declvalue(this_t).swap_with((this_t&)this_t(a))){
			this_t tmp(forward<T>(a));
			swap_with(tmp);
			return*this;
		}

		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}

		Ret_t operator()(Args_t...args)const noexcept(nothrow){
			return base_t::call(forward<Args_t>(args)...);
		}
		/*
		private:
			//以下是突然想加的功能(没什么用<迷惑行为大赏>).
			static ptr_t _func_ptr_data;
			static Ret_t _func_ptr_value(Args_t...args)noexcept(nothrow){
				return _func_ptr_data->call(forward<Args_t>(args)...);
			}
		public:
			[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
				_func_ptr_data=base_t::_m;
				return _func_ptr_value;
			}
		*/
	};
	template<class T,bool promise_nothrow_at_destruct>
	void swap(base_function_t<T,promise_nothrow_at_destruct>&a,base_function_t<T,promise_nothrow_at_destruct>&b)noexcept{
		a.swap_with(b);
	}
	#else
	//MSVC，我滴垃圾堆
	template<class T>
	class base_func_data_t;
	template<class Ret_t,class...Args_t>
	struct base_func_data_t<Ret_t(Args_t...)>:type_info_t<base_func_data_t<Ret_t(Args_t...)>>::template_name with_common_attribute<abstract_base,ref_able,never_in_array>,build_by_get_only{
		typedef base_func_data_t<Ret_t(Args_t...)>this_t;

		virtual ~base_func_data_t()=default;
		virtual Ret_t call(Args_t...)=0;
		//for equal:
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept=0;
		[[nodiscard]]virtual const void*get_data_begin()const noexcept=0;
		[[nodiscard]]virtual bool equal_with(const void*)const=0;
		[[nodiscard]]bool operator==(const this_t&a)const{
			return this->get_type_info()==a.get_type_info()&&this->equal_with(a.get_data_begin());
		}
		template<typename T>
		[[nodiscard]]bool operator==(T&&a)const{
			return this->get_type_info()==type_info<T>&&this->equal_with(addressof(a));
		}
	};

	template<class T,class Func_t>
	class func_data_t;
	template<class T,class Ret_t,class...Args_t>
	struct func_data_t<T,Ret_t(Args_t...)>:
	type_info_t<func_data_t<T,Ret_t(Args_t...)>>::template_name with_common_attribute<instance_struct>,
	base_func_data_t<Ret_t(Args_t...)>,function_data_warpper_t<T,Ret_t(Args_t...)>{
		static_assert(!::std::is_function_v<T>);
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;
		typedef function_data_warpper_t<T,Ret_t(Args_t...)>data_t;

		using data_t::data_t;
		virtual ~func_data_t()override=default;
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<T>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return addressof(data_t::get_data());}
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept(equal.able<T>?equal.nothrow<T>:true) override{
			if constexpr(equal.able<T>)
				return data_t::_value==*reinterpret_cast<const T*>(a);
			else
				return false;
		}
		[[nodiscard]]virtual Ret_t call(Args_t...args)override{
			return data_t::operator()(forward<Args_t>(args)...);
		}
	};

	template<class T>
	class default_func_data_t;
	template<class Ret_t,class...Args_t>
	struct default_func_data_t<Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>,instance_struct<default_func_data_t<Ret_t(Args_t...)>>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		virtual ~default_func_data_t()noexcept override{}
		virtual Ret_t call(Args_t...)override{return Ret_t();}
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<void>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return null_ptr;}//这玩意实际上用不到，艹
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept override{return true;}
	};
	template<class Ret_t,class...Args_t>
	inline default_func_data_t<Ret_t(Args_t...)>default_func_data{};

	template<class Ret_t,class...Args_t>
	[[nodiscard]]constexpr base_func_data_t<Ret_t(Args_t...)>*get_null_ptr(type_info_t<base_func_data_t<Ret_t(Args_t...)>>)noexcept{
		return&default_func_data<Ret_t,Args_t...>;
	}

	template<class T>
	class function_data_saver_t;
	template<class Ret_t,class...Args_t>
	class function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>this_t;
		typedef base_func_data_t<Ret_t(Args_t...)> base_t_w;
		typedef comn_ptr_t<base_t_w>ptr_t;

		ptr_t _m;
		void swap_with(this_t&a)noexcept{swap(_m,a._m);}
	public:
		function_data_saver_t()noexcept=default;
		function_data_saver_t(const this_t&a)noexcept:_m(a._m){}
		function_data_saver_t(this_t&&a)noexcept{swap_with(a);}
		[[nodiscard]]bool operator==(const this_t&a)const{
			return *_m==*(a._m);
		}
		template<typename T> requires(equal.able<base_t_w,T>)
		[[nodiscard]]bool operator==(T&&a)const noexcept(equal.nothrow<base_t_w,T>){
			return *_m==a;
		}
		void operator=(const this_t&a){_m=a._m;}
		Ret_t call(Args_t&&...rest)const{return _m->call(forward<Args_t>(rest)...);}
	};

	template<class T,bool promise_nothrow_at_destruct>
	class base_function_t;
	template<class Ret_t,class...Args_t,bool promise_nothrow_at_destruct>
	struct base_function_t<Ret_t(Args_t...),promise_nothrow_at_destruct>:function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>base_t;
		typedef base_function_t<Ret_t(Args_t...),promise_nothrow_at_destruct>this_t;

		template<class,bool>
		friend class base_function_t;

		template<class U>
		using func_data_t=function_n::func_data_t<U,Ret_t(Args_t...)>;

		typedef Ret_t(*func_ptr_t)(Args_t...);

		using base_t::ptr_t;
		using base_t::_m;

		template<class T>
		static constexpr bool base_on_this_t_or_more_stringent_restrictions=(
			type_info<T>.base_on<this_t>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept,	promise_nothrow_at_destruct			>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...),			bool(promise_nothrow_at_destruct+1)	>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept,	bool(promise_nothrow_at_destruct+1)	>>
		);
	public:
		void swap_with(this_t&a)noexcept{//不与base_t::swap_with重复：与更加严格（或宽松）的this_t进行swap是错误的
			base_t::swap_with(a);
		}

		base_function_t()noexcept=default;
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		base_function_t(const T&a)noexcept:base_t(a){}
		base_function_t(const this_t&a)noexcept:base_t(a){}
		base_function_t(this_t&&a)noexcept:base_function_t(){
			swap_with(a);
		}
		base_function_t(nullptr_t)noexcept:base_function_t(){}
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		template<class T> requires(invoke<T>.able<Args_t...> && not base_on_this_t_or_more_stringent_restrictions<T>)
		base_function_t(T&&a)noexcept(get<func_data_t<remove_cvref<T>>>.nothrow<T>){
			//BLOCK:constexpr checks
			if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
				template_error("unexpected assign.");
			//BLOCK_END
			_m=get<func_data_t<remove_cvref<T>>>(a);
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		template<class T>
		this_t&operator=(T&&a)&noexcept_as(declvalue(this_t).swap_with((this_t&)this_t(a))) requires was_not_an_ill_form(declvalue(this_t).swap_with((this_t&)this_t(a))){
			this_t tmp(forward<T>(a));
			swap_with(tmp);
			return*this;
		}

		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}

		Ret_t operator()(Args_t...args)const{
			return base_t::call(forward<Args_t>(args)...);
		}
		/*
		private:
			//以下是突然想加的功能(没什么用<迷惑行为大赏>).
			static ptr_t _func_ptr_data;
			static Ret_t _func_ptr_value(Args_t...args){
				return _func_ptr_data->call(forward<Args_t>(args)...);
			}
		public:
			[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
				_func_ptr_data=base_t::_m;
				return _func_ptr_value;
			}
		*/
	};
	template<class Ret_t,class...Args_t,bool promise_nothrow_at_destruct>
	struct base_function_t<Ret_t(Args_t...)noexcept,promise_nothrow_at_destruct>:function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>base_t;
		typedef base_function_t<Ret_t(Args_t...)noexcept,promise_nothrow_at_destruct>this_t;

		template<class,bool>
		friend class base_function_t;

		template<class U>
		using func_data_t=function_n::func_data_t<U,Ret_t(Args_t...)>;

		typedef Ret_t(*func_ptr_t)(Args_t...)noexcept;

		using base_t::ptr_t;
		using base_t::_m;

		template<class T>
		static constexpr bool base_on_this_t_or_more_stringent_restrictions=(
			type_info<T>.base_on<this_t>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept,	promise_nothrow_at_destruct			>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...)noexcept,	bool(promise_nothrow_at_destruct+1)	>>
		);
	public:
		void swap_with(this_t&a)noexcept{//不与base_t::swap_with重复：与更加严格（或宽松）的this_t进行swap是错误的
			base_t::swap_with(a);
		}

		base_function_t()noexcept=default;
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		base_function_t(const T&a)noexcept:base_t(a){}
		base_function_t(const this_t&a)noexcept:base_t(a){}
		base_function_t(this_t&&a)noexcept:base_function_t(){
			swap_with(a);
		}
		base_function_t(nullptr_t)noexcept:base_function_t(){}
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		template<class T> requires(invoke<T>.able<Args_t...> && not base_on_this_t_or_more_stringent_restrictions<T>)
		base_function_t(T&&a)noexcept(get<func_data_t<remove_cvref<T>>>.nothrow<T>){
			//BLOCK:constexpr checks
			if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
				template_error("unexpected assign.");
			if constexpr(1)
				if constexpr(!invoke<T>.nothrow<Args_t...>)
					template_warning("the call of T was not noexcept,this may cause terminate.");
			//BLOCK_END
			_m=get<func_data_t<remove_cvref<T>>>(a);
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		template<class T>
		this_t&operator=(T&&a)&noexcept_as(declvalue(this_t).swap_with((this_t&)this_t(forward<T>(a)))) requires was_not_an_ill_form(declvalue(this_t).swap_with((this_t&)this_t(forward<T>(a)))){
			this_t tmp(forward<T>(a));
			swap_with(tmp);
			return*this;
		}

		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}

		Ret_t operator()(Args_t...args)const noexcept{
			return base_t::call(forward<Args_t>(args)...);
		}
		/*
		private:
			//以下是突然想加的功能(没什么用<迷惑行为大赏>).
			static ptr_t _func_ptr_data;
			static Ret_t _func_ptr_value(Args_t...args)noexcept{
				return _func_ptr_data->call(forward<Args_t>(args)...);
			}
		public:
			[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
				_func_ptr_data=base_t::_m;
				return _func_ptr_value;
			}
		*/
	};
	template<class T,bool promise_nothrow_at_destruct>
	void swap(base_function_t<T,promise_nothrow_at_destruct>&a,base_function_t<T,promise_nothrow_at_destruct>&b)noexcept{
		a.swap_with(b);
	}
	#endif
}

//file_end

