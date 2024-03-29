//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
namespace string_n{
	#include "string_data_t/_body.hpp"
	template<typename char_T>
	struct string_t{
		static_assert(type_info<char_T> != type_info<const char_T>);

		typedef base_string_data_t<char_T>			base_t_w;
		typedef base_t_w::ptr_t						ptr_t;
		typedef base_t_w::string_view_t				string_view_t;
		typedef base_t_w::string_ptr_t				string_ptr_t;
		typedef base_t_w::const_string_ptr_t		const_string_ptr_t;
		typedef const constexpr_str_t<char_T>		constexpr_str_t;
		typedef string_t<char_T>					this_t;
		typedef char_T								value_type;
		typedef value_type							char_type;
		typedef size_t								size_type;
		static constexpr size_t						npos = bit::npos;

	private:
		//_cso_info用于存储不同cso情况下string所用到的数据
		mutable union _cso_info_t{
			ptr_t _mptr;
			const constexpr_str_t* _str;
			char_T _ch;

			push_and_disable_msvc_warning(26495);//未初始化警告diss
			force_inline constexpr _cso_info_t()noexcept{}
			constexpr _cso_info_t(const _cso_info_t&a)noexcept{
				copy_assign[sizeof(_cso_info_t)](cast_to_data(this),cast_to_data(&a));
			}
			pop_msvc_warning();
			force_inline constexpr void operator=(const _cso_info_t&a)noexcept{
				copy_assign[sizeof(_cso_info_t)](cast_to_data(this),cast_to_data(&a));
			}
			force_inline ~_cso_info_t()noexcept{}
		}_cso_info{};
		//_cso_flags用于标识不同的cso情况
		mutable struct _cso_flags_t{
			bool _cso_flag=0;
			bool _str_cso_flag=0;
		}_cso_flags;

		//非cso情况下的成员宏，表示一个指向string data的智能指针
		#define _m _cso_info._mptr

		//BLOCK: 对成员宏`_m`的生命周期手动管理的函数族

		//结束成员宏`_m`的生命周期
		force_inline constexpr void _ncso_destruct_mptr()const noexcept{destruct(&_m);}
		//开始成员宏`_m`的生命周期
		force_inline constexpr void _ncso_construct_mptr()const noexcept{construct<ptr_t>[&_m]();}
		//以特定初始值开始成员宏`_m`的生命周期
		force_inline constexpr void _ncso_construct_mptr(ptr_t p)const noexcept{construct<ptr_t>[&_m](p);}

		//END_BLOCK

		//BLOCK: cso情况判断函数族
		[[nodiscard]]force_inline bool _in_cso()const noexcept{return _cso_flags._cso_flag;}
		[[nodiscard]]force_inline bool _in_str_cso()const noexcept{return _in_cso() && _cso_flags._str_cso_flag;}
		[[nodiscard]]force_inline bool _in_chr_cso()const noexcept{return _in_cso() && !_cso_flags._str_cso_flag;}
		//END_BLOCK
		//BLOCK: cso情况设定函数族
		force_inline constexpr void _set_str_cso()const noexcept{_cso_flags._cso_flag=1;_cso_flags._str_cso_flag=1;}
		force_inline constexpr void _set_chr_cso()const noexcept{_cso_flags._cso_flag=1;_cso_flags._str_cso_flag=0;}
		force_inline constexpr void _set_not_cso()const noexcept{_cso_flags._cso_flag=0;}
		//END_BLOCK

		//BLOCK: cso情况下的信息获取函数族
		[[nodiscard]]force_inline const char_T* _get_cso_data()const noexcept{return _in_str_cso()?_cso_info._str->str():&_cso_info._ch;}
		[[nodiscard]]force_inline size_t _get_cso_size()const noexcept{return _in_str_cso()?_cso_info._str->size():1;}
		[[nodiscard]]force_inline hash_t _get_cso_hash()const noexcept{return _in_str_cso()?hash(_get_cso_constexpr_str()):hash(_cso_info._ch);}
		[[nodiscard]]force_inline constexpr_str_t& _get_cso_constexpr_str()const noexcept{return *_cso_info._str;}
		//END_BLOCK

		//BLOCK: cso情况管理函数族
		force_inline constexpr void _cso_init(constexpr_str_t&str)noexcept{_set_str_cso();_cso_info._str=&str;}
		force_inline constexpr void _cso_reinit(constexpr_str_t&str)noexcept{if(!_in_cso())_ncso_destruct_mptr();_cso_init(str);}
		force_inline constexpr void _cso_init(char_T ch)noexcept{_set_chr_cso();_cso_info._ch=ch;}
		force_inline constexpr void _cso_reinit(char_T ch)noexcept{if(!_in_cso())_ncso_destruct_mptr();_cso_init(ch);}
		force_inline void _cso_fin(bool need_write)const noexcept{
			if(_in_str_cso()&&!need_write)
				_ncso_construct_mptr(get<constexpr_string_data_t<char_T>>(*_cso_info._str));
			else{
				auto str=string_view_t{_get_cso_data(),_get_cso_size()};
				_ncso_construct_mptr(get<comn_string_data_t<char_T>>(str));
			}
			_set_not_cso();
		}
		force_inline void _cso_fin(ptr_t p)noexcept{
			_set_not_cso();
			_ncso_construct_mptr(p);
		}
		//END_BLOCK

		//BLOCK: 已知需要拷贝cso全部内容，判断是否值得结束cso的检查
		static force_inline constexpr bool the_size_worth_to_end_cso(size_t size)noexcept{
			constexpr auto max_size=max(sizeof(comn_string_data_t<char_T>)*2/sizeof(char_T),1u);
			return size>=max_size;
		}
		static force_inline void full_copy_cso_check(const string_t&str)noexcept{
			if(the_size_worth_to_end_cso(str.size()))
				str._cso_check();
		}
		//END_BLOCK

		//若cso，结束它
		force_inline void _cso_check(bool need_write=0)const noexcept{
			if(_in_cso())
				_cso_fin(need_write);
		}

		//便利用，内部使用的构造函数
		force_inline string_t(ptr_t str)noexcept{_ncso_construct_mptr(str);}
		//无论是否在cso中，都保证可以获得一个ptr
		[[nodiscard]]force_inline ptr_t ptr_copy()const noexcept{
			_cso_check();
			return _m;
		}
	public:
		//swap的内部实现
		void swap_with(this_t& a)noexcept{
			if(_in_cso()||a._in_cso()){
				swap(_cso_info,a._cso_info);
				swap(_cso_flags,a._cso_flags);
			}
			else
				swap(_m,a._m);
		}

		//BLOCK: 构造函数
		constexpr string_t()noexcept:string_t(empty_constexpr_str_of<char_T>){}
		constexpr string_t(constexpr_str_t&str)noexcept{_cso_init(str);}
		string_t(string_view_t str)noexcept{_ncso_construct_mptr(get<comn_string_data_t<char_T>>(str));}
		string_t(const_string_ptr_t str)noexcept:string_t(string_view_t(str)){}
		constexpr string_t(char_T ch)noexcept{_cso_init(ch);}
		string_t(const string_t& str)noexcept{
			if(str._in_cso()){
				_cso_info=str._cso_info;
				_cso_flags=str._cso_flags;
			}
			else
				_ncso_construct_mptr(str._m);
		}
		string_t(string_t&& str)noexcept:string_t(){
			swap_with(str);
		}
		string_t(size_t size,char_T ch)noexcept{_ncso_construct_mptr(get<same_value_compress_string_data_t<char_T>>(size,ch));}
		string_t(char_T ch,size_t size)noexcept requires(type_info<size_t>!=type_info<char_T>):string_t(size,ch){}
		string_t(array_t<char_t> base)noexcept{_ncso_construct_mptr(get<comn_string_data_t<char_T>>(move(base)));}

		string_t(size_t size)noexcept{_ncso_construct_mptr(get<comn_string_data_t<char_T>>(size));}
		//END_BLOCK
		//析构函数
		~string_t()noexcept{if(!_in_cso())_ncso_destruct_mptr();}

		//BLOCK: 赋值操作符
		string_t& operator=(const string_t& str)&noexcept{re_construct[this](str);return*this;}
		string_t& operator=(string_t&& str)&noexcept{swap_with(str);return*this;}
		constexpr string_t& operator=(constexpr_str_t&str)&noexcept{_cso_reinit(str);return*this;}
		constexpr string_t& operator=(char_T ch)&noexcept{_cso_reinit(ch);return*this;}
		//END_BLOCK

		//BLOCK: 字符串相加操作符
		[[nodiscard]]string_t operator+(const string_t& str)const noexcept{
			full_copy_cso_check(str);
			if(str._in_cso())
				return operator+(str.to_string_view_t());
			else
				return ptr_copy()->apply_str_to_end(str._m);
		}
		[[nodiscard]]string_t operator+(string_view_t str)const noexcept{
			return ptr_copy()->apply_str_to_end(str);
		}
		[[nodiscard]]string_t operator+(const_string_ptr_t str)const noexcept{
			return *this+string_view_t(str);
		}
		friend [[nodiscard]]string_t operator+(string_view_t str1,const string_t& str2)noexcept{
			return str2.ptr_copy()->apply_str_to_begin(str1);
		}
		friend [[nodiscard]]string_t operator+(const_string_ptr_t str1,const string_t& str2)noexcept{
			return string_view_t(str1)+str2;
		}
		friend [[nodiscard]]string_t operator+(char_T ch,const string_t& str)noexcept{
			return string_view_t{&ch,1}+str;
		}

		string_t& operator+=(const string_t&str)&noexcept{
			push_back(str);
			return *this;
		}
		string_t& operator+=(string_t&&str)&noexcept{
			push_back(move(str));
			return *this;
		}
		string_t& operator+=(string_view_t str)&noexcept{
			push_back(str);
			return *this;
		}
		string_t& operator+=(const_string_ptr_t str)&noexcept{
			return *this+=string_view_t(str);
		}
		string_t& operator+=(char_T ch)&noexcept{
			return *this += string_view_t{&ch,1};
		}
		template<typename U>
		[[nodiscard]]string_t&& operator+(U&& b)&&noexcept_as(*this+=b) requires was_not_an_ill_form(*this+=b){		  //对右值的operator+优化为operator+=
			*this+=forward<U>(b);
			return move(*this);
		}
		//END_BLOCK

		//获取string占用的内存大小
		float_size_t memory_cost()const noexcept{
			if(_in_cso())
				return 0;
			else
				return _m->get_memory_cost();
		}
	public:
		//BLOCK: 比较运算符
		[[nodiscard]]constexpr auto operator<=>(const string_t& a)const noexcept(compare.nothrow<char_T>){
			if(a._in_str_cso())
				return operator<=>(a._get_cso_constexpr_str());
			elseif(a._in_cso())
				return operator<=>(a.to_string_view_t());
			if(!_in_cso())
				return _m->compare_with(a._m);
			elseif(_in_str_cso())
				return compare.reverse(a<=>_get_cso_constexpr_str());
			else
				return compare.reverse(a<=>to_string_view_t());
		}
		[[nodiscard]]constexpr auto operator==(const string_t& a)const noexcept(equal.nothrow<char_T>){
			if(a._in_str_cso())
				return operator==(a._get_cso_constexpr_str());
			elseif(a._in_cso())
				return operator==(a.to_string_view_t());
			if(!_in_cso())
				return _m->equal_with(a._m);
			elseif(_in_str_cso())
				return a==_get_cso_constexpr_str();
			else
				return a==to_string_view_t();
		}
		[[nodiscard]]constexpr auto operator<=>(string_view_t a)const noexcept(compare.nothrow<char_T>){
			if(_in_cso())
				return compare(data(),a.begin(),size());
			else
				return _m->compare_with(a);
		}
		[[nodiscard]]constexpr auto operator==(string_view_t a)const noexcept(equal.nothrow<char_T>){
			if(_in_cso())
				return equal(data(),a.begin(),size());
			else
				return _m->equal_with(a);
		}
		[[nodiscard]]constexpr auto operator<=>(constexpr_str_t&a)const noexcept(compare.nothrow<char_T>){
			if(_in_cso() && data()==a.str())//同起始优化
				return strong_ordering::equivalent;
			auto tmp=operator<=>((string_view_t&)a);
			if(tmp==0)
				remove_const(this)->_cso_reinit(a);
			return tmp;
		}
		[[nodiscard]]constexpr auto operator==(constexpr_str_t&a)const noexcept(equal.nothrow<char_T>){
			if(_in_cso() && data()==a.str())//同起始优化
				return true;
			const auto tmp=operator==((string_view_t&)a);
			if(tmp)
				remove_const(this)->_cso_reinit(a);
			return tmp;
		}
		[[nodiscard]]constexpr auto operator<=>(const_string_ptr_t a)const noexcept(compare.nothrow<char_T>){
			if(_in_cso())
				return compare(data(),size(),a,end_by_zero);
			else
				return _m->compare_with(a);
		}
		[[nodiscard]]constexpr auto operator==(const_string_ptr_t a)const noexcept(equal.nothrow<char_T>){
			if(_in_cso())
				return equal(data(),size(),a,end_by_zero);
			else
				return _m->equal_with(a);
		}
		//END_BLOCK
	private:
		[[nodiscard]]string_ptr_t unique_c_str()noexcept{ _cso_check(1);return _m->get_unique_c_str(_m); }
	public:
		[[nodiscard]]char_T	arec(size_t index)noexcept{
			if(_in_cso())
				return _get_cso_data()[index];
			else
				return _m->arec(index);
		}
		void	arec_set(size_t index,char_T a)noexcept{ _cso_check(1);return _m->arec_set(index,a,_m); }

		class arec_t: non_copyable,non_moveable{
			string_t* _to;
			size_t	  _index;

			friend class string_t;
		public:
			arec_t(string_t* to,size_t index)noexcept:_to(to),_index(index){}
			arec_t(special_init_t,const arec_t&ref)noexcept:_to(ref._to),_index(ref._index){}
			[[nodiscard]]operator char_T()const&&noexcept{ return _to->arec(_index); }
			arec_t&&	 operator=(char_T a)&&noexcept{
				_to->arec_set(_index,a);
				return move(*this);
			}
			arec_t&&	 operator=(const arec_t&&ch)&&noexcept{ return move(*this).operator=(move(ch).operator char_T()); }
		private:
			[[nodiscard]]char_T*		get_address()noexcept{ return _to->unique_c_str()+_index; }
			[[nodiscard]]const char_T*	get_address()const noexcept{ return (add_const(_to))->c_str()+_index; }
		public:
			[[nodiscard]]char_T*		operator&()&&noexcept{ return get_address(); }
			[[nodiscard]]const char_T*	operator&()const&&noexcept{ return get_address(); }
			/*
			[[nodiscard]]explicit operator char_T&()&&noexcept{ return *get_address(); }
			[[nodiscard]]explicit operator const char_T&()const&&noexcept{ return *get_address(); }
			*/
			template<class text_ostream_T> requires(type_info<text_ostream_T>.base_on<text_ostream_t<char_T>>)
			friend auto& operator<<(text_ostream_T& os,arec_t&& ch)noexcept(noexcept_text_ostream_class<text_ostream_T>){
				return os << move(ch).operator char_T();
			}
			template<class text_istream_T> requires(type_info<text_istream_T>.base_on<text_istream_t<char_T>>)
			friend auto& operator>>(text_istream_T& is,arec_t&& ch)noexcept(noexcept_text_istream_class<text_istream_T>){
				char_T ach;is >> ach;
				move(ch)=ach;
				return is;
			}
		};

		[[nodiscard]]arec_t		  operator[](size_t index)noexcept{ return{this,index}; }
		[[nodiscard]]const arec_t operator[](size_t index)const noexcept{ return{remove_const(this),index}; }

		//front
		[[nodiscard]]arec_t		  front()noexcept{ return{this,0}; }
		[[nodiscard]]const arec_t front()const noexcept{ return{remove_const(this),0}; }
		//back
		[[nodiscard]]arec_t		  back()noexcept{ return{this,size()-1}; }
		[[nodiscard]]const arec_t back()const noexcept{ return{remove_const(this),size()-1}; }


		[[nodiscard]]string_t operator+(const arec_t&& ch)const noexcept{
			return *this+move(ch).operator char_T();
		}
		string_t& operator+=(const arec_t&& ch)&noexcept{
			return *this+=move(ch).operator char_T();
		}

		//floating arec.
	private:
		static constexpr bool floating_arec_result_able=was_not_an_ill_form(
			math::linear_interpolation::get_result(declvalue(size_t),
				 math::linear_interpolation::get_k(declvalue(char_T),declvalue(char_T)),
														   declvalue(float_t))
		);
		static auto floating_arec_result_type_helper()noexcept{
			if constexpr(floating_arec_result_able)
				return math::linear_interpolation::get_result(size_t{},
					 math::linear_interpolation::get_k(char_T{},char_T{}),
															   float_t{});
		}
		typedef decltype(floating_arec_result_type_helper()) floating_arec_result_type;
		static constexpr bool floating_arec_set_able=was_not_an_ill_form(
			math::linear_interpolation::get_reverse_result(
				 math::linear_interpolation::get_k(declvalue(char_T),declvalue(char_T)),
														   declvalue(float_t),declvalue(char_T))
		);
	public:
		static constexpr bool floating_arec_able=floating_arec_result_able||floating_arec_set_able;
		template<class=void> requires floating_arec_able
		class floating_arec_t_impl: non_copyable,non_moveable{
			string_t* _to;
			float_t	  _index;
			size_t	  _index_x1;
			size_t	  _index_x2;

			friend class string_t;
			typedef floating_arec_t_impl<> this_t;
		public:
			static constexpr bool result_able=floating_arec_result_able;
			static constexpr bool set_able=floating_arec_set_able;

			floating_arec_t_impl(string_t* to,float_t index)noexcept:_to(to),_index(index){
				_index_x1=static_cast<size_t>(index);
				_index_x2=_index_x1+1;
			}
			floating_arec_t_impl(special_init_t,const this_t&ref)noexcept:_to(ref._to),_index(ref._index){}
			[[nodiscard]]operator floating_arec_result_type()const&&noexcept requires result_able{
				char_T y1=_to->arec(_index_x1);
				char_T y2=_to->arec(_index_x2);
				auto δx=_index-_index_x1;
				return math::linear_interpolation::get_result(y1,
							math::linear_interpolation::get_k(y1,y2),
														   			  δx);
			}
			this_t&& operator=(floating_arec_result_type a)&&noexcept requires set_able{
				char_T y1=_to->arec(_index_x1);
				char_T y2=_to->arec(_index_x2);
				auto k=math::linear_interpolation::get_k(y1,y2);
				auto δx1=_index-_index_x1;
				auto δx2=_index-_index_x2;
				y1=(char_T)math::linear_interpolation::get_reverse_result(k,δx1,a);
				y2=(char_T)math::linear_interpolation::get_reverse_result(k,δx2,a);
				_to->arec_set(_index_x1,y1);
				_to->arec_set(_index_x2,y2);
				return move(*this);
			}
		};
		#define floating_arec_t floating_arec_t_impl<>
		[[nodiscard]]auto operator[](float_t index)noexcept requires floating_arec_able{ return floating_arec_t{this,index}; }
		[[nodiscard]]const auto operator[](float_t index)const noexcept requires floating_arec_able{ return floating_arec_t{remove_const(this),index}; }
		#undef floating_arec_t

		//泛型arec.
		template<arithmetic_type T>
		[[nodiscard]]auto operator[](T index)noexcept{
			if constexpr(float_type<T>)
				return operator[](static_cast<float_t>(index));
			else
				return operator[](static_cast<size_t>(index));
		}
		template<arithmetic_type T>
		[[nodiscard]]const auto operator[](T index)const noexcept{
			if constexpr(float_type<T>)
				return operator[](static_cast<float_t>(index));
			else
				return operator[](static_cast<size_t>(index));
		}

		[[nodiscard]]string_t substr(size_t begin,size_t size=npos)const noexcept{
			size=min(size,this->size()-begin);
			if(size){
				_cso_check();
				return _m->get_substr_data(begin,size);
			}
			else
				return{};
		}
		[[nodiscard]]const char_T*	data()const noexcept{ if(_in_cso())return _get_cso_data();else return _m->get_data(_m); }
		[[nodiscard]]char_T* writeable_data()noexcept{ return unique_c_str(); }
		[[nodiscard]]const_string_ptr_t 	c_str()const noexcept{ if(_in_cso())return _get_cso_data();else return _m->get_const_c_str(_m); }
		[[nodiscard]]string_ptr_t writeable_c_str()noexcept{ return unique_c_str(); }
		[[nodiscard]]size_t			size()const noexcept{ if(_in_cso())return _get_cso_size();else return _m->get_size(); }
		[[nodiscard]]explicit operator bool()const noexcept{ return size(); }
		[[nodiscard]]bool			empty()const noexcept{ return !size(); }
		[[nodiscard]]size_t			length()const noexcept{ return size(); }
		void resize(size_t nsize,char_T ch)noexcept{
			const auto size=this->size();
			if(size > nsize)
				*this=substr(0,nsize);
			elseif(size == nsize)
				return;
			elseif(size){
				_cso_check();
				_m=get<end_apply_string_data_t<char_T>>(_m,nsize-size,ch);
			}
			else
				*this=string_t{nsize,ch};
		}
		void resize(size_t nsize)noexcept{
			const auto size=this->size();
			if(size > nsize)
				*this=substr(0,nsize);
			elseif(size == nsize)
				return;
			elseif(size){
				_cso_check();
				_m=get<end_apply_string_data_t<char_T>>(_m,nsize-size);
			}
			else
				*this=string_t{nsize};
		}
		void clear()noexcept{ re_construct(this); }
	private:
		struct iterator_base_t{
			string_t* _to;
			ptrdiff_t _index;		//rend: -1，使用size_t也不是不可以（标准允许无符号整数溢出），但是operator<=>会出问题

			[[nodiscard]]constexpr iterator_base_t	get_before()const noexcept{ return{_to,_index-1}; }
			[[nodiscard]]constexpr iterator_base_t	get_next()const noexcept{ return{_to,_index+1}; }
			[[nodiscard]]arec_t						get_value()noexcept{ return (*_to)[_index]; }
			[[nodiscard]]const arec_t				get_value()const noexcept{ return (*add_const(_to))[_index]; }
			[[nodiscard]]char_T*					get_handle()noexcept{ return &get_value(); }
			[[nodiscard]]const char_T*				get_handle()const noexcept{ return &get_value(); }
			[[nodiscard]]constexpr bool operator==(const iterator_base_t& a)const noexcept{ return _to==a._to && _index==a._index; }
			[[nodiscard]]constexpr auto operator<=>(const iterator_base_t& a)const noexcept{ return _to==a._to ? _index<=>a._index : partial_ordering::unordered; }
		};
		[[nodiscard]]iterator_base_t get_iterator_data_at(ptrdiff_t index)const noexcept{ return iterator_base_t{(string_t*)this,index}; }
	public:
		typedef iterator_t<char_T,iterator_base_t>				iterator;
		typedef const_iterator_t<char_T,const iterator_base_t>	const_iterator;

		[[nodiscard]]iterator		get_iterator_at(ptrdiff_t index)noexcept{ return get_iterator_data_at(index); }
		[[nodiscard]]const_iterator get_iterator_at(ptrdiff_t index)const noexcept{ return get_iterator_data_at(index); }
		[[nodiscard]]iterator		begin()noexcept{ return get_iterator_at(0); }
		[[nodiscard]]const_iterator begin()const noexcept{ return get_iterator_at(0); }
		[[nodiscard]]const_iterator cbegin()const noexcept{ return begin(); }
		[[nodiscard]]iterator		end()noexcept{ return get_iterator_at(size()); }
		[[nodiscard]]const_iterator end()const noexcept{ return get_iterator_at(size()); }
		[[nodiscard]]const_iterator cend()const noexcept{ return end(); }

		typedef reverse_iterator_t<char_T,iterator_base_t>				reverse_iterator;
		typedef reverse_const_iterator_t<char_T,const iterator_base_t>	reverse_const_iterator;

		[[nodiscard]]reverse_iterator		get_reverse_iterator_at(ptrdiff_t index)noexcept{ return get_iterator_data_at(index); }
		[[nodiscard]]reverse_const_iterator get_reverse_iterator_at(ptrdiff_t index)const noexcept{ return get_iterator_data_at(index); }
		[[nodiscard]]reverse_iterator		rbegin()noexcept{ return get_reverse_iterator_at(size()-1); }
		[[nodiscard]]reverse_const_iterator rbegin()const noexcept{ return get_reverse_iterator_at(size()-1); }
		[[nodiscard]]reverse_const_iterator rcbegin()const noexcept{ return rbegin(); }
		[[nodiscard]]reverse_iterator		rend()noexcept{ return get_reverse_iterator_at(-1); }
		[[nodiscard]]reverse_const_iterator rend()const noexcept{ return get_reverse_iterator_at(-1); }
		[[nodiscard]]reverse_const_iterator rcend()const noexcept{ return rend(); }

		//自迭代器构造string
		string_t(const_iterator begin,const_iterator end)noexcept{
			const auto& beg = begin.get_base();
			const auto& ed	= end.get_base();
			if(beg._to!=ed._to)return;
			_ncso_construct_mptr(beg._to->substr(beg._index, ed._index - beg._index)._m);
		}
		//

		void push_back(const string_t& str)&noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_begin(to_string_view_t()));
			elseif(str._in_cso())
				push_back(str.to_string_view_t());
			else{
				_cso_check();
				_m=_m->apply_str_to_end(str._m);
			}
		}
		void push_back(string_t&& str)&noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_begin(to_string_view_t()));
			elseif(str._in_cso())
				push_back(str.to_string_view_t());
			else{
				_cso_check();
				_m=_m->apply_str_to_end(move(str._m));
			}
		}
		void push_back(string_view_t str)&noexcept{ _cso_check();_m=_m->apply_str_to_end(str); }
		void push_back(char_T ch)&noexcept{ push_back(string_view_t{&ch,1}); }
		void push_back(const arec_t&& ch)&noexcept{ push_back(move(ch).operator char_T()); }
		void push_back(const_string_ptr_t str)&noexcept{ push_back(string_view_t(str)); }
		//
		void push_back(char_T ch,size_t count)&noexcept{
			if(count==0)return;
			_cso_check();
			string_t str{ch,count};
			_m=_m->apply_str_to_end(str._m);
		}
		void push_back(const arec_t&& ch,size_t count)&noexcept{
			push_back(move(ch).operator char_T(),count);
		}

		void push_front(const string_t& str)&noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_end(to_string_view_t()));
			elseif(str._in_cso())
				push_front(str.to_string_view_t());
			else{
				_cso_check();
				_m=_m->apply_str_to_begin(str._m);
			}
		}
		void push_front(string_t&& str)&noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_end(to_string_view_t()));
			elseif(str._in_cso())
				push_front(str.to_string_view_t());
			else{
				_cso_check();
				_m=_m->apply_str_to_begin(move(str._m));
			}
		}
		void push_front(string_view_t str)&noexcept{ _cso_check();_m=_m->apply_str_to_begin(str); }
		void push_front(char_T ch)&noexcept{ push_front(string_view_t{&ch,1}); }
		void push_front(const arec_t&& ch)&noexcept{ push_front(move(ch).operator char_T()); }
		void push_front(const_string_ptr_t str)&noexcept{ push_front(string_view_t(str)); }
		//
		void push_front(char_T ch,size_t count)&noexcept{
			if(count==0)return;
			_cso_check();
			string_t str{ch,count};
			_m=_m->apply_str_to_begin(str._m);
		}
		void push_front(const arec_t&& ch,size_t count)&noexcept{
			push_front(move(ch).operator char_T(),count);
		}

		void remove_back(size_t size=1)noexcept{ _cso_check();_m=_m->do_remove_back(size); }
		void remove_front(size_t size=1)noexcept{ _cso_check();_m=_m->do_remove_front(size); }
		#define pop_function_nodiscard nodiscard("if you don't want to use the return value, use remove_back() or remove_front() instead to avoid unnecessary cost")
		[[pop_function_nodiscard]]string_t pop_back(size_t size)noexcept{ _cso_check();return _m->do_pop_back(size,_m); }
		[[pop_function_nodiscard]]string_t pop_front(size_t size)noexcept{ _cso_check();return _m->do_pop_front(size,_m); }
		[[pop_function_nodiscard]]char_T pop_back()noexcept{ return pop_back(1)[0]; }
		[[pop_function_nodiscard]]char_T pop_front()noexcept{ return pop_front(1)[0]; }
		#undef pop_function_nodiscard

		//

		[[nodiscard]]operator string_view_t()const&noexcept{ return string_view_t{data(),size()}; }
		[[nodiscard]]auto to_string_view_t()const&noexcept{ return operator string_view_t(); }
		[[nodiscard]]auto view()&noexcept{ return array_like_view_t<char_T>{writeable_data(),size()}; }
		[[nodiscard]]auto view()const&noexcept{ return array_like_view_t<const char_T>{data(),size()}; }
		[[nodiscard]]explicit operator hash_t()const noexcept{ return _in_cso()?_get_cso_hash():_m->get_hash(_m); }

		//
		static constexpr bool find_nothrow=equal.nothrow<char_T>;
		[[nodiscard]]size_t find(const char_T ch)const noexcept(find_nothrow){
			if(_in_cso()){
				if(_in_str_cso())
					return in_range_size_t(ch,_get_cso_constexpr_str());
				else//if(_in_cso())
					return _cso_info._ch==ch?0:npos;
			}
			return in_range_size_t(ch, to_string_view_t());
		}
		[[nodiscard]]size_t find(const char_T ch,size_t begin)const noexcept(find_nothrow){
			return in_range_size_t(ch, to_string_view_t().substr(begin));
		}
		[[nodiscard]]size_t reverse_find(const char_T ch)const noexcept(find_nothrow){
			if(_in_cso()){
				if(_in_str_cso())
					return in_range_but_reverse_size_t(ch,_get_cso_constexpr_str());
				else//if(_in_cso())
					return _cso_info._ch==ch?0:npos;
			}
			return in_range_but_reverse_size_t(ch, to_string_view_t());
		}
		[[nodiscard]]size_t find(string_view_t str,size_t begin=0)const noexcept(find_nothrow){
			return in_range_size_t(str, to_string_view_t().substr(begin));
		}
		[[nodiscard]]size_t reverse_find(string_view_t str)const noexcept(find_nothrow){
			return in_range_but_reverse_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t find(constexpr_str_t&str,size_t begin=0)const noexcept(find_nothrow){
			return in_range_size_t(str, to_string_view_t().substr(begin));
		}
		[[nodiscard]]size_t reverse_find(constexpr_str_t&str)const noexcept(find_nothrow){
			return in_range_but_reverse_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t find(const string_t&str,size_t begin=0)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return find(str._get_cso_constexpr_str(),begin);
				else//if(str._in_cso())
					return find(str._cso_info._ch,begin);
			}
			auto result = str._m->get_match_pattern_from_self(str._m).match(to_string_view_t().substr(begin));
			if(result)
				return result - data();
			else
				return npos;
		}
		[[nodiscard]]size_t reverse_find(const string_t&str)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return reverse_find(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return reverse_find(str._cso_info._ch);
			}
			auto result = str._m->get_reverse_match_pattern_from_self(str._m).match(to_string_view_t());
			if(result)
				return result - data();
			else
				return npos;
		}
		//
		[[nodiscard]]size_t find_first_of(string_view_t str)const noexcept(find_nothrow){
			return range_n::find_first_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(constexpr_str_t&str)const noexcept(find_nothrow){
			return range_n::find_first_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(const string_t&str)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_first_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find(str._cso_info._ch);
			}
			return find_first_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(const_string_ptr_t str)const noexcept(find_nothrow){
			return find_first_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_first_of(char_T ch)const noexcept(find_nothrow){
			return find(ch);
		}
		[[nodiscard]]size_t find_first_of(const arec_t&&ch)const noexcept(find_nothrow){
			return find(move(ch).operator char_T());
		}
		[[nodiscard]]size_t find_first_not_of(string_view_t str)const noexcept(find_nothrow){
			return range_n::find_first_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(constexpr_str_t&str)const noexcept(find_nothrow){
			return range_n::find_first_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(const string_t&str)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_first_not_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find_first_not_of(str.to_string_view_t());
			}
			return find_first_not_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(const_string_ptr_t str)const noexcept(find_nothrow){
			return find_first_not_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_first_not_of(char_T ch)const noexcept(find_nothrow){
			return find_first_not_of(string_view_t(ch));
		}
		[[nodiscard]]size_t find_first_not_of(const arec_t&&ch)const noexcept(find_nothrow){
			return find_first_not_of(string_view_t(move(ch).operator char_T()));
		}
		[[nodiscard]]size_t find_last_of(string_view_t str)const noexcept(find_nothrow){
			return range_n::find_last_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(constexpr_str_t&str)const noexcept(find_nothrow){
			return range_n::find_last_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(const string_t&str)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_last_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return reverse_find(str._cso_info._ch);
			}
			return find_last_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(const_string_ptr_t str)const noexcept(find_nothrow){
			return find_last_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_last_of(char_T ch)const noexcept(find_nothrow){
			return reverse_find(ch);
		}
		[[nodiscard]]size_t find_last_of(const arec_t&&ch)const noexcept(find_nothrow){
			return reverse_find(move(ch).operator char_T());
		}
		[[nodiscard]]size_t find_last_not_of(string_view_t str)const noexcept(find_nothrow){
			return range_n::find_last_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(constexpr_str_t&str)const noexcept(find_nothrow){
			return range_n::find_last_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(const string_t&str)const noexcept(find_nothrow){
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_last_not_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find_last_not_of(str.to_string_view_t());
			}
			return find_last_not_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(const_string_ptr_t str)const noexcept(find_nothrow){
			return find_last_not_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_last_not_of(char_T ch)const noexcept(find_nothrow){
			return find_last_not_of(string_view_t{&ch,1});
		}
		[[nodiscard]]size_t find_last_not_of(const arec_t&&ch)const noexcept(find_nothrow){
			return find_last_not_of(move(ch).operator char_T());
		}

		//

		[[nodiscard]]string_t arec(size_t index,char_T delimiter)const{
			auto pos=find(delimiter);
			auto end=find(delimiter,pos);
			while(index--){
				pos=end;
				end=find(delimiter,pos);
			}
			return substr(pos,end-pos);
		}
		[[nodiscard]]string_t arec(size_t index,string_view_t delimiter_str)const{
			auto pos=find(delimiter_str);
			auto end=find(delimiter_str,pos);
			while(index--){
				pos=end;
				end=find(delimiter_str,pos);
			}
			return substr(pos,end-pos);
		}
		[[nodiscard]]string_t arec(size_t index,const string_t&delimiter_str)const{
			auto pos=find(delimiter_str);
			auto end=find(delimiter_str,pos);
			while(index--){
				pos=end;
				end=find(delimiter_str,pos);
			}
			return substr(pos,end-pos);
		}
		[[nodiscard]]string_t arec(size_t index,constexpr_str_t&delimiter_str)const{
			auto pos=find(delimiter_str);
			auto end=find(delimiter_str,pos);
			while(index--){
				pos=end;
				end=find(delimiter_str,pos);
			}
			return substr(pos,end-pos);
		}

		//

		void erase(size_t pos,size_t size=1)&noexcept{
			_cso_check();_m=_m->do_erase(pos,size);
		}
		void insert(size_t pos,const string_t& str)&noexcept{
			_cso_check();
			full_copy_cso_check(str);
			if(str._in_cso())
				_m=_m->do_insert(pos,str.to_string_view_t());
			else
				_m=_m->do_insert(pos,str);
		}
		void insert(size_t pos,string_view_t str)&noexcept{
			_cso_check();_m=_m->do_insert(pos,str);
		}
		void insert(size_t pos,const_string_ptr_t str)&noexcept{
			insert(pos,string_view_t(str));
		}
		void insert(size_t pos,char_T ch)&noexcept{
			insert(pos,string_view_t{&ch,1});
		}

		//

		//contains
		[[nodiscard]]constexpr bool contains(string_view_t str)const noexcept{
			return find(str) != npos;
		}
		[[nodiscard]]constexpr bool contains(constexpr_str_t&str)const noexcept{
			return find(str) != npos;
		}
		[[nodiscard]]constexpr bool contains(const string_t&str)const noexcept{
			return find(str) != npos;
		}
		[[nodiscard]]constexpr bool contains(char_T ch)const noexcept{
			return find(ch) != npos;
		}
		[[nodiscard]]constexpr bool contains(const_string_ptr_t str)const noexcept{
			return find(str) != npos;
		}

		//starts_with
		[[nodiscard]]constexpr bool starts_with(string_view_t str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(0,str.size()) == str;
		}
		[[nodiscard]]constexpr bool starts_with(constexpr_str_t&str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(0,str.size()) == str;
		}
		[[nodiscard]]constexpr bool starts_with(const string_t&str)const noexcept{
			if(size()<str.size())
				return false;
			if(str._in_chr_cso())
				return starts_with(str._cso_info._ch);
			else
				return substr(0,str.size()) == str;
		}
		[[nodiscard]]constexpr bool starts_with(char_T ch)const noexcept{
			if(size()<1)
				return false;
			return operator[](0) == ch;
		}
		[[nodiscard]]constexpr bool starts_with(const_string_ptr_t str)const noexcept{
			return starts_with(string_view_t{str});
		}

		//ends_with
		[[nodiscard]]constexpr bool ends_with(string_view_t str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(size()-str.size()) == str;
		}
		[[nodiscard]]constexpr bool ends_with(constexpr_str_t&str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(size()-str.size()) == str;
		}
		[[nodiscard]]constexpr bool ends_with(const string_t&str)const noexcept{
			if(size()<str.size())
				return false;
			if(str._in_chr_cso())
				return ends_with(str._cso_info._ch);
			else
				return substr(size()-str.size()) == str;
		}
		[[nodiscard]]constexpr bool ends_with(char_T ch)const noexcept{
			if(size()<1)
				return false;
			return operator[](size()-1) == ch;
		}
		[[nodiscard]]constexpr bool ends_with(char_T ch,size_t count)const noexcept{
			if(size() < count)
				return false;
			return find_last_not_of(ch) <= size()-count;
		}
		[[nodiscard]]constexpr bool ends_with(const_string_ptr_t str)const noexcept{
			return ends_with(string_view_t{str});
		}

		this_t& append(size_t size,char_T ch)noexcept{
			return *this+=string_t{size,ch};
		}

		//pad_left
		[[nodiscard]]this_t pad_left(size_t size,char_T ch)const noexcept{
			if(size<=this->size())
				return *this;
			return string_t{size-this->size(),ch}+*this;
		}
		[[nodiscard]]this_t pad_left(char_T ch,size_t size)const noexcept{
			return pad_left(size,ch);
		}
		//pad_right
		[[nodiscard]]this_t pad_right(size_t size,char_T ch)const noexcept{
			if(size<=this->size())
				return *this;
			return *this+string_t{size-this->size(),ch};
		}
		[[nodiscard]]this_t pad_right(char_T ch,size_t size)const noexcept{
			return pad_right(size,ch);
		}

		//pre_allocate
		void pre_alloc_before_begin(size_t size)noexcept{
			_cso_check();
			_m=_m->pre_alloc_before_begin(size);
		}
		void pre_alloc_after_end(size_t size)noexcept{
			_cso_check();
			_m=_m->pre_alloc_after_end(size);
		}

		/*
		compare
		*/

		//replace
		//TODO: 右值版replace
		[[nodiscard]]this_t replace(size_t pos,size_t size,const string_t& str)const noexcept{
			return substr(0,pos)+str+substr(pos+size);
		}
		[[nodiscard]]this_t replace(char_T ch,char_T ch2)const noexcept{
			this_t aret;
			size_t pos=0;
			floop{
				auto pos2=find(ch,pos);
				if(pos2==npos)
					break;
				aret+=substr(pos,pos2-pos)+ch2;
				pos=pos2+1;
			}
			aret+=substr(pos);
			return aret;
		}
		[[nodiscard]]this_t replace(char_T ch,const string_t& str)const noexcept{
			this_t aret;
			size_t pos=0;
			floop{
				auto pos2=find(ch,pos);
				if(pos2==npos)
					break;
				aret+=substr(pos,pos2-pos)+str;
				pos=pos2+1;
			}
			aret+=substr(pos);
			return aret;
		}
		[[nodiscard]]this_t replace(const string_t& str1,const string_t& str2)const noexcept{
			this_t aret;
			size_t pos=0;
			floop{
				auto pos2=find(str1,pos);
				if(pos2==npos)
					break;
				aret+=substr(pos,pos2-pos)+str2;
				pos=pos2+str1.size();
			}
			aret+=substr(pos);
			return aret;
		}

		//iostream
		template<class text_ostream_T> requires(type_info<text_ostream_T>.base_on<text_ostream_t<char_T>>)
		friend auto& operator<<(text_ostream_T& os,const string_t& str)noexcept(noexcept_text_ostream_class<text_ostream_T>){
			if(str._in_cso())
				os<<str.to_string_view_t();
			else{
				push_and_disable_msvc_warning(26447);//nothrow警告diss
				str._m->do_output(os);
				pop_msvc_warning();
			}
			return os;
		}
		template<class text_istream_T> requires(type_info<text_istream_T>.base_on<text_istream_t<char_T>>)
		friend auto& operator>>(text_istream_T& is,string_t& str)noexcept(noexcept_text_istream_class<text_istream_T>){
			str.clear();
			using namespace char_set;
			while(isspace(is.peek()))
				is.get();
			/*/
			size_t insert_pos=0;
			while(!is.is_end()){
				const auto c=is.peek();
				switch (c){
				case char_T(38)://right
					is.get();
					if(insert_pos<str.size())
						insert_pos++;
					break;
				case char_T(37)://left
					is.get();
					if(insert_pos>0)
						insert_pos--;
					break;
				case char_T(8)://backspace
					is.get();
					if(insert_pos>0)
						str.erase(--insert_pos);
					break;
				
				default:
					if(isspace(c))
						return is;
					else
						str.insert(insert_pos++,is.get());
					break;
				}
			}
			/*/
			while(!is.is_end()&&!isspace(is.peek()))
				str+=is.get();
			//*/
			return is;
		}
		template<class text_istream_T> requires(type_info<text_istream_T>.base_on<text_istream_t<char_T>>)
		friend auto& getline(text_istream_T& is,string_t& str)noexcept(noexcept_text_istream_class<text_istream_T>){
			str.clear();
			using namespace char_set;
			while(isspace(is.peek()))
				is.get();
			/*/
			size_t insert_pos=0;
			while(!is.is_end()){
				const auto c=is.peek();
				switch (c){
				case char_T(38)://right
					is.get();
					if(insert_pos<str.size())
						insert_pos++;
					break;
				case char_T(37)://left
					is.get();
					if(insert_pos>0)
						insert_pos--;
					break;
				case char_T(8)://backspace
					is.get();
					if(insert_pos>0)
						str.erase(--insert_pos);
					break;
				case char_T('\r'):
				case char_T('\n'):
					is>>endline;//处理结尾
					return str;
				default:
					str.push_back(is.get());
					break;
				}
			}
			/*/
			while(!is.is_end()&&is.peek()!='\r'&&is.peek()!='\n')
				str+=is.get();
			//*/
			is>>endline;//处理结尾
			return str;
		}
		#undef _m
	};
}

namespace stream_n{
	//单参数getline
	template<text_istream_class text_istream_T>
	[[nodiscard]]inline auto getline(text_istream_T& is)noexcept(noexcept_text_istream_class<text_istream_T>){
		typedef typename remove_cvref<text_istream_T>::char_type char_T;
		string_n::string_t<char_T> str;
		return getline(is,str);
	}
}

namespace string_n{
	//推导指引
	template<class char_T>
	string_t(const char_T*) -> string_t<char_T>;
	template<class char_T>
	string_t(char_T*) -> string_t<char_T>;

	//string的swap特化
	template<typename T>
	inline void swap(string_t<T>& a,string_t<T>& b)noexcept{ a.swap_with(b); }

	//other iostream
	template<typename char_T,typename traits>
	inline auto& operator<<(::std::basic_ostream<char_T,traits>& os,const string_t<char_T>& str)noexcept{
		typedef ::std::basic_ostream<char_T,traits> stream_t;
		typedef stream_t::traits_type	 traits_t;
		typename stream_t::iostate		 state = stream_t::goodbit;

		size_t size = str.size();
		push_and_disable_msvc_warning(26494);//未初始化警告diss
		size_t pad;
		if(os.width() <= 0 || static_cast<size_t>(os.width()) <= size)
			pad = 0;
		else
			pad = static_cast<size_t>(os.width()) - size;
		pop_msvc_warning();

		const typename stream_t::sentry isok(os);

		if(!isok)
			state |= stream_t::badbit;
		else{
			try{
				if((os.flags() & stream_t::adjustfield) != stream_t::left){
					for(; 0 < pad; --pad){// pad on left
						if(traits_t::eq_int_type(traits_t::eof(), os.rdbuf()->sputc(os.fill()))){
							state |= stream_t::badbit;// insertion failed, quit
							break;
						}
					}
				}
				if(state == stream_t::goodbit){
					stream_n::std_ostream_wrap wrap(os);
					wrap<<str;
				}
				if(os.fail())
					state |= stream_t::badbit;
				else{
					for(; 0 < pad; --pad){// pad on right
						if(traits_t::eq_int_type(traits_t::eof(), os.rdbuf()->sputc(os.fill()))){
							state |= stream_t::badbit;// insertion failed, quit
							break;
						}
					}
				}
				os.width(0);
			}
			catch(...){
				os.setstate(stream_t::badbit, true);
				return os;
			}
		}
		os.setstate(state);
		return os;
	}
	template<typename char_T,typename traits>
	inline auto& operator>>(::std::basic_istream<char_T,traits>& is,string_t<char_T>& str)noexcept{
		stream_n::std_istream_wrap wrap{is};
		wrap>>str;
		return is;
	}

	//array like支持
	template<class T>
	[[nodiscard]]inline auto size_of_array_like(const string_t<remove_cv<T>>& a)noexcept{ return a.size(); }
	push_and_disable_msvc_warning(26460);//参数1可以为const 但是这是有意的
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(string_t<remove_cv<T>>& a)noexcept{ return a.c_str(); }
	pop_msvc_warning();
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(const string_t<remove_cv<T>>& a)noexcept{ return a.c_str(); }

	//typedef
	typedef string_t<char_t>string;
}
using string_n::string_t;
using string_n::string;

//file_end

