//base_function_t.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	template<class T>
	class base_func_data_t;
	template<class Ret_t,class...Args_t>
	no_vtable_struct base_func_data_t<Ret_t(Args_t...)>:type_info_t<base_func_data_t<Ret_t(Args_t...)>>::template_name with_common_attribute<abstract_base,ref_able,never_in_array>,build_by_get_only{
		typedef base_func_data_t<Ret_t(Args_t...)>this_t;
		typedef comn_ptr_t<this_t>ptr_t;

		virtual ~base_func_data_t()=default;
		virtual Ret_t call(Args_t...)=0;
		//for equal:
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept=0;
		[[nodiscard]]virtual const void*get_data_begin()const noexcept=0;
		[[nodiscard]]virtual bool equal_with(const void*)const=0;
		[[noreturn]] virtual void throw_self_ptr()const=0;
		[[noreturn]] virtual void throw_self_ptr()=0;
		[[nodiscard]]bool operator==(const this_t&a)const{
			return this->get_type_info()==a.get_type_info()&&this->equal_with(a.get_data_begin());
		}
		template<typename T>
		[[nodiscard]]bool operator==(const T&a)const{
			return this->get_type_info()==type_info<T>&&this->equal_with(addressof(a));
		}
		typedef Ret_t(*func_ptr_t)(Args_t...);
		virtual func_ptr_t get_func_ptr()const noexcept{return nullptr;}
	};

	template<class T,class Func_t>
	class func_data_t;
	template<class T,class Ret_t,class...Args_t>
	struct func_data_t<T,Ret_t(Args_t...)>final:
	type_info_t<func_data_t<T,Ret_t(Args_t...)>>::template_name with_common_attribute<instance_struct>,
	base_func_data_t<Ret_t(Args_t...)>,function_data_wrapper_t<T,Ret_t(Args_t...)>{
		static_assert(!::std::is_function_v<T>);
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;
		typedef func_data_t<T,Ret_t(Args_t...)>this_t;
		typedef function_data_wrapper_t<T,Ret_t(Args_t...)>data_t;
		typedef base_t::ptr_t ptr_t;
		typedef base_t::func_ptr_t func_ptr_t;

		[[nodiscard]]bool is_unique()const noexcept{return get_ref_num((base_t*)this)==1;}

		using data_t::data_t;
		virtual ~func_data_t()override=default;
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<T>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override final{return addressof(data_t::get_data());}
		[[noreturn]] virtual void throw_self_ptr()const override final{throw addressof(data_t::get_data());}
		[[noreturn]] virtual void throw_self_ptr()override final{throw addressof(data_t::get_data());}
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept(equal.able<T>?equal.nothrow<T>:true)override final{
			if constexpr(equal.able<T>)
				return data_t::get_data()==*remove_const(reinterpret_cast<const T*>(a));
			else
				return false;
		}
		[[nodiscard]]virtual Ret_t call(Args_t...args)noexcept(invoke<T>.nothrow<Args_t...>)override final{
			return data_t::operator()(forward<Args_t>(args)...);
		}

		//func ptr convert
		virtual func_ptr_t get_func_ptr()const noexcept override final{
			if constexpr(type_info<const T>.can_convert_to<func_ptr_t>)
				return static_cast<func_ptr_t>(data_t::get_data());
			else
				return nullptr;
		}
	};

	template<class T>
	class default_func_data_t;
	template<class Ret_t,class...Args_t>
	struct default_func_data_t<Ret_t(Args_t...)>final:base_func_data_t<Ret_t(Args_t...)>,instance_struct<default_func_data_t<Ret_t(Args_t...)>>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;
		typedef base_t::ptr_t ptr_t;

		virtual ~default_func_data_t()noexcept override final{}
		virtual Ret_t call(Args_t...)noexcept_as(Ret_t())override final{return Ret_t();}
		[[nodiscard]]virtual base_type_info_t get_type_info()const noexcept override final{return type_info<void>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override final{return null_ptr;}//这玩意实际上用不到，艹
		[[noreturn]] virtual void throw_self_ptr()const override final{throw(const void*)null_ptr;}
		[[noreturn]] virtual void throw_self_ptr()override final{throw(void*)null_ptr;}
		[[nodiscard]]virtual bool equal_with(const void*)const noexcept override final{return true;}
	};
	template<class Ret_t,class...Args_t>
	distinctive inline default_func_data_t<Ret_t(Args_t...)>default_func_data{};

	template<class Ret_t,class...Args_t>
	[[nodiscard]]constexpr base_func_data_t<Ret_t(Args_t...)>*the_get_null_ptr(const base_func_data_t<Ret_t(Args_t...)>*)noexcept{
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

		mutable ptr_t _m;
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
		template<typename T>
		[[nodiscard]]bool was_an()const noexcept{
			try{
				_m->throw_self_ptr();
			}
			catch(const T*p){
				return true;
			}
			catch(const void*){}
			return false;
		}
		template<typename T>
		[[nodiscard]]maybe_fail_reference<T> get_as()noexcept{
			try{
				_m->throw_self_ptr();
			}
			catch(T*p){
				return *p;
			}
			catch(void*){}
			return note::fail;
		}
		suppress_msvc_warning(26440)//nothrow警告diss
		void operator=(const this_t&a){_m=a._m;}
		Ret_t call(Args_t&&...rest)const{return _m->call(forward<Args_t>(rest)...);}
	};

	template<class T,bool promise_nothrow_at_destruct>
	class base_function_t;

	template<class T>
	struct is_function_t_helper {
		constexpr static bool value = false;
	};
	template<class T, bool promise_nothrow_at_destruct>
	struct is_function_t_helper<base_function_t<T, promise_nothrow_at_destruct>> {
		constexpr static bool value = true;
	};
	template<class T>
	constexpr static bool is_function_t = is_function_t_helper<T>::value;
	#if !defined(_MSC_VER)
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

		template<class T>
		static constexpr bool get_data_able_helper(){
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_able_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.able<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_able=get_data_able_helper<T>();
		template<class T>
		static constexpr bool get_data_nothrow_helper() {
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_nothrow_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.nothrow<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_nothrow=get_data_nothrow_helper<T>();

		template<class T> requires get_data_able<T>
		static auto get_data_from(T&&a)noexcept(get_data_nothrow<T>){
			if constexpr(::std::is_function_v<T>)
				return get_data_from(&a);
			elseif constexpr(is_function_t<T>)
				return a._m;
			else{
				//BLOCK:constexpr checks
				if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
					template_error("unexpected assign.");
				if constexpr(nothrow)
					if constexpr(!invoke<T>.nothrow<Args_t...>)
						template_warning("the call of T was not noexcept,this may cause terminate.");
				//BLOCK_END
				return get<func_data_t<remove_cvref<T>>>(a);
			}
		}
		explicit base_function_t(base_t::ptr_t a)noexcept{_m=a;}
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
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		base_function_t(nullptr_t)noexcept:base_function_t(null_ptr){}
		template<class T> requires(get_data_able<T>)
		base_function_t(T&&a)noexcept(get_data_nothrow<T>){
			_m=get_data_from(forward<T>(a));
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		this_t&operator=(const this_t&a)&noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(this_t&&a)&noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(null_ptr_t)&noexcept(promise_nothrow_at_destruct){_m=null_ptr;return *this;}
		this_t&operator=(nullptr_t)&noexcept(promise_nothrow_at_destruct){return *this=null_ptr;}
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}
		template<class T> requires(get_data_able<T>)
		this_t&operator=(T&&a)noexcept(get_data_nothrow<T> && promise_nothrow_at_destruct){
			_m=get_data_from(forward<T>(a));
			return*this;
		}

		this_t deep_copy(){return this_t{copy_get(_m.get())};}

		[[nodiscard]]explicit operator bool()const noexcept{
			return bool(_m);
		}

		Ret_t operator()(Args_t...args)const noexcept(nothrow){
			suppress_msvc_warning(26447)
			return base_t::call(forward<Args_t>(args)...);
		}

	private:
		//以下是突然想加的功能(没什么用<迷惑行为大赏>).
		static thread_local inline base_t::base_t_w* _func_ptr_data_local=nullptr;
		static inline ::std::atomic<decltype(_func_ptr_data_local)> _func_ptr_data_gobal=nullptr;
		static Ret_t _func_ptr_value(Args_t...args)noexcept{
			if(!_func_ptr_data_local)
				_func_ptr_data_local=_func_ptr_data_gobal.load(::std::memory_order_relaxed);
			return _func_ptr_data_local->call(forward<Args_t>(args)...);
		}
	public:
		//自当前function对象生成一个函数指针，具有与当前function对象相同的行为
		//注意：该函数指针的有效性保留到当前线程下一个同类型的函数指针生成时或当前function对象析构时
		//若自该函数指针创建一个新的线程，该函数指针在新线程中的行为与调用时以前所有线程中最后一个生成的同类型的函数指针行为相同
		[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
			func_ptr_t a=(func_ptr_t)_m->get_func_ptr();
			if(a)return a;
			_func_ptr_data_local=base_t::_m.get();
			_func_ptr_data_gobal.store(_func_ptr_data_local,::std::memory_order_relaxed);
			return _func_ptr_value;
		}
	};
	#else
	//MSVC，我滴垃圾堆
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

		template<class T>
		static constexpr bool get_data_able_helper(){
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_able_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.able<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_able=get_data_able_helper<T>();
		template<class T>
		static constexpr bool get_data_nothrow_helper() {
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_nothrow_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.nothrow<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_nothrow=get_data_nothrow_helper<T>();

		template<class T> requires get_data_able<T>
		static auto get_data_from(T&&a)noexcept(get_data_nothrow<T>){
			if constexpr(::std::is_function_v<T>)
				return get_data_from(&a);
			elseif constexpr(is_function_t<T>)
				return a._m;
			else{
				//BLOCK:constexpr checks
				if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
					template_error("unexpected assign.");
				//BLOCK_END
				return get<func_data_t<remove_cvref<T>>>(a);
			}
		}
		explicit base_function_t(base_t::ptr_t a)noexcept{_m=a;}
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
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		base_function_t(nullptr_t)noexcept:base_function_t(null_ptr){}
		template<class T> requires(get_data_able<T>)
		base_function_t(T&&a)noexcept(get_data_nothrow<T>){
			_m=get_data_from(forward<T>(a));
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		this_t&operator=(const this_t&a)noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(this_t&&a)noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(null_ptr_t)noexcept(promise_nothrow_at_destruct){_m=null_ptr;return *this;}
		this_t&operator=(nullptr_t)noexcept(promise_nothrow_at_destruct){return *this=null_ptr;}
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}
		template<class T> requires(get_data_able<T>)
		this_t&operator=(T&&a)noexcept(get_data_nothrow<T> && promise_nothrow_at_destruct){
			_m=get_data_from(forward<T>(a));
			return*this;
		}

		this_t deep_copy(){return this_t{copy_get(_m.get())};}

		[[nodiscard]]explicit operator bool()const noexcept{
			return bool(_m);
		}

		Ret_t operator()(Args_t...args){
			return base_t::call(forward<Args_t>(args)...);
		}
		Ret_t operator()(Args_t...args)const{
			return base_t::call(forward<Args_t>(args)...);
		}

	private:
		//以下是突然想加的功能(没什么用<迷惑行为大赏>).
		static thread_local inline base_t::base_t_w* _func_ptr_data_local=nullptr;
		static inline ::std::atomic<decltype(_func_ptr_data_local)> _func_ptr_data_gobal=nullptr;
		static Ret_t _func_ptr_value(Args_t...args)noexcept{
			if(!_func_ptr_data_local)
				_func_ptr_data_local=_func_ptr_data_gobal.load(::std::memory_order_relaxed);
			return _func_ptr_data_local->call(forward<Args_t>(args)...);
		}
	public:
		//自当前function对象生成一个函数指针，具有与当前function对象相同的行为
		//注意：该函数指针的有效性保留到当前线程下一个同类型的函数指针生成时或当前function对象析构时
		//若自该函数指针创建一个新的线程，该函数指针在新线程中的行为与调用时以前所有线程中最后一个生成的同类型的函数指针行为相同
		[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
			func_ptr_t a=(func_ptr_t)_m->get_func_ptr();
			if(a)return a;
			_func_ptr_data_local=base_t::_m.get();
			_func_ptr_data_gobal.store(_func_ptr_data_local,::std::memory_order_relaxed);
			return _func_ptr_value;
		}
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

		template<class T>
		static constexpr bool get_data_able_helper(){
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_able_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.able<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_able=get_data_able_helper<T>();
		template<class T>
		static constexpr bool get_data_nothrow_helper() {
			if constexpr(base_on_this_t_or_more_stringent_restrictions<T>)
				return true;
			elseif constexpr(is_function_t<T>)
				return false;
			elseif constexpr(::std::is_function_v<T>)
				return get_data_nothrow_helper<decltype(&declvalue(T))>();
			elseif constexpr(invoke<T>.able<Args_t...>)
				return get<func_data_t<remove_cvref<T>>>.nothrow<T>;
			else
				return false;
		}
		template<class T>
		static constexpr bool get_data_nothrow=get_data_nothrow_helper<T>();

		template<class T> requires get_data_able<T>
		static auto get_data_from(T&&a)noexcept(get_data_nothrow<T>){
			if constexpr(::std::is_function_v<T>)
				return get_data_from(&a);
			elseif constexpr(is_function_t<T>)
				return a._m;
			else{
				//BLOCK:constexpr checks
				if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
					template_error("unexpected assign.");
				if constexpr(1)
					if constexpr(!invoke<T>.nothrow<Args_t...>)
						template_warning("the call of T was not noexcept,this may cause terminate.");
				//BLOCK_END
				return get<func_data_t<remove_cvref<T>>>(a);
			}
		}
		explicit base_function_t(base_t::ptr_t a)noexcept{_m=a;}
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
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		base_function_t(nullptr_t)noexcept:base_function_t(null_ptr){}
		template<class T> requires(get_data_able<T>)
		base_function_t(T&&a)noexcept(get_data_nothrow<T>){
			_m=get_data_from(forward<T>(a));
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(true)的参数可自动转为noexcept(false)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		this_t&operator=(const this_t&a)&noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(this_t&&a)&noexcept(promise_nothrow_at_destruct)=default;
		this_t&operator=(null_ptr_t)&noexcept(promise_nothrow_at_destruct){_m=null_ptr;return *this;}
		this_t&operator=(nullptr_t)&noexcept(promise_nothrow_at_destruct){return *this=null_ptr;}
		template<class T> requires base_on_this_t_or_more_stringent_restrictions<T>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}
		template<class T> requires(get_data_able<T>)
		this_t&operator=(T&&a)noexcept(get_data_nothrow<T> && promise_nothrow_at_destruct){
			_m=get_data_from(forward<T>(a));
			return*this;
		}

		this_t deep_copy(){return this_t{copy_get(_m.get())};}

		[[nodiscard]]explicit operator bool()const noexcept{
			return bool(_m);
		}

		Ret_t operator()(Args_t...args)noexcept{
			suppress_msvc_warning(26447)
			return base_t::call(forward<Args_t>(args)...);
		}
		Ret_t operator()(Args_t...args)const noexcept{
			suppress_msvc_warning(26447)
			return base_t::call(forward<Args_t>(args)...);
		}

	private:
		//以下是突然想加的功能(没什么用<迷惑行为大赏>).
		static thread_local inline base_t::base_t_w* _func_ptr_data_local=nullptr;
		static inline ::std::atomic<decltype(_func_ptr_data_local)> _func_ptr_data_gobal=nullptr;
		static Ret_t _func_ptr_value(Args_t...args)noexcept{
			if(!_func_ptr_data_local)
				_func_ptr_data_local=_func_ptr_data_gobal.load(::std::memory_order_relaxed);
			return _func_ptr_data_local->call(forward<Args_t>(args)...);
		}
	public:
		//自当前function对象生成一个函数指针，具有与当前function对象相同的行为
		//注意：该函数指针的有效性保留到当前线程下一个同类型的函数指针生成时或当前function对象析构时
		//若自该函数指针创建一个新的线程，该函数指针在新线程中的行为与调用时以前所有线程中最后一个生成的同类型的函数指针行为相同
		[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
			func_ptr_t a=(func_ptr_t)_m->get_func_ptr();
			if(a)return a;
			_func_ptr_data_local=base_t::_m.get();
			_func_ptr_data_gobal.store(_func_ptr_data_local,::std::memory_order_relaxed);
			return _func_ptr_value;
		}
	};
	#endif
	template<class T,bool promise_nothrow_at_destruct>
	void swap(base_function_t<T,promise_nothrow_at_destruct>&a,base_function_t<T,promise_nothrow_at_destruct>&b)noexcept{
		a.swap_with(b);
	}
}

//file_end

