//defs.hpp
//at namespace elc::defs
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace string_n{
	#include "string_data_t/_body.hpp"
	template<typename char_T>
	struct string_t{
		static_assert(type_info<char_T> != type_info<const char_T>);

		typedef base_string_data_t<char_T>			base_t_w;
		typedef base_t_w::ptr_t						ptr_t;
		typedef base_t_w::string_view_t				string_view_t;
		typedef const constexpr_str_t<char_T>		constexpr_str_t;
		typedef string_t<char_T>					this_t;
		static constexpr size_t						npos = range_n::npos;

	private:
		//_cso_info用于存储不同cso情况下string所用到的数据
		mutable union _cso_info_t{
			ptr_t _mptr;
			const constexpr_str_t* _str;
			char_T _ch;

			#if defined(_MSC_VER)
				#pragma warning(push)
				#pragma warning(disable:26495)//未初始化警告diss
			#endif
			constexpr _cso_info_t()noexcept{}
			constexpr _cso_info_t(const _cso_info_t&a)noexcept{
				copy_assign[sizeof(_cso_info_t)](cast_to_data(this),cast_to_data(&a));
			}
			#if defined(_MSC_VER)
				#pragma warning(pop)
			#endif
			constexpr void operator=(const _cso_info_t&a)noexcept{
				copy_assign[sizeof(_cso_info_t)](cast_to_data(this),cast_to_data(&a));
			}
			~_cso_info_t()noexcept{}
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
		constexpr void _ncso_destruct_mptr()const noexcept{destruct(&_m);}
		//开始成员宏`_m`的生命周期
		constexpr void _ncso_construct_mptr()const noexcept{construct<ptr_t>[&_m]();}
		//以特定初始值开始成员宏`_m`的生命周期
		constexpr void _ncso_construct_mptr(ptr_t p)const noexcept{construct<ptr_t>[&_m](p);}

		//END_BLOCK

		//BLOCK: cso情况判断函数族
		[[nodiscard]]bool _in_cso()const noexcept{return _cso_flags._cso_flag;}
		[[nodiscard]]bool _in_str_cso()const noexcept{return _in_cso() && _cso_flags._str_cso_flag;}
		[[nodiscard]]bool _in_chr_cso()const noexcept{return _in_cso() && !_cso_flags._str_cso_flag;}
		//END_BLOCK
		//BLOCK: cso情况设定函数族
		constexpr void _set_str_cso()const noexcept{_cso_flags._cso_flag=1;_cso_flags._str_cso_flag=1;}
		constexpr void _set_chr_cso()const noexcept{_cso_flags._cso_flag=1;_cso_flags._str_cso_flag=0;}
		constexpr void _set_not_cso()const noexcept{_cso_flags._cso_flag=0;}
		//END_BLOCK

		//BLOCK: cso情况下的信息获取函数族
		[[nodiscard]]const char_T* _get_cso_data()const noexcept{return _in_str_cso()?_cso_info._str->str():&_cso_info._ch;}
		[[nodiscard]]size_t _get_cso_size()const noexcept{return _in_str_cso()?_cso_info._str->size():1;}
		[[nodiscard]]hash_t _get_cso_hash()const noexcept{return _in_str_cso()?hash(_get_cso_constexpr_str()):hash(_cso_info._ch);}
		[[nodiscard]]constexpr_str_t& _get_cso_constexpr_str()const noexcept{return *_cso_info._str;}
		//END_BLOCK

		//BLOCK: cso情况管理函数族
		constexpr void _cso_init(constexpr_str_t&str)noexcept{_set_str_cso();_cso_info._str=&str;}
		constexpr void _cso_reinit(constexpr_str_t&str)noexcept{if(!_in_cso())_ncso_destruct_mptr();_cso_init(str);}
		constexpr void _cso_init(char_T ch)noexcept{_set_chr_cso();_cso_info._ch=ch;}
		constexpr void _cso_reinit(char_T ch)noexcept{if(!_in_cso())_ncso_destruct_mptr();_cso_init(ch);}
		void _cso_fin(bool need_write) const noexcept {
			if(_in_str_cso()&&!need_write)
				_ncso_construct_mptr(get<constexpr_string_data_t<char_T>>(*_cso_info._str));
			else{
				auto str=string_view_t{_get_cso_data(),_get_cso_size()};
				_ncso_construct_mptr(get<comn_string_data_t<char_T>>(str));
			}
			_set_not_cso();
		}
		void _cso_fin(ptr_t p)noexcept{
			_set_not_cso();
			_ncso_construct_mptr(p);
		}
		//END_BLOCK

		//BLOCK: 已知需要拷贝cso全部内容，判断是否值得结束cso的检查
		static constexpr bool the_size_worth_to_end_cso(size_t size)noexcept{
			constexpr auto max_size=max(sizeof(comn_string_data_t<char_T>)*2/sizeof(char_T),(size_t)1);
			return size>=max_size;
		}
		static void full_copy_cso_check(const string_t&str)noexcept{
			if(the_size_worth_to_end_cso(str.size()))
				str._cso_check();
		}
		//END_BLOCK

		//若cso，结束它
		void _cso_check(bool need_write=0)const noexcept{
			if(_in_cso())
				_cso_fin(need_write);
		}

		//便利用，内部使用的构造函数
		string_t(ptr_t str)noexcept{_ncso_construct_mptr(str);}
		//无论是否在cso中，都保证可以获得一个ptr
		[[nodiscard]]ptr_t ptr_copy()const noexcept{
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
		string_t(const char_T* str)noexcept:string_t(string_view_t(str)){}
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
		//END_BLOCK
		//析构函数
		~string_t()noexcept{if(!_in_cso())_ncso_destruct_mptr();}

		//BLOCK: 赋值操作符
		string_t& operator=(const string_t& str)noexcept{re_construct[this](str);return*this;}
		string_t& operator=(string_t&& str)noexcept{swap_with(str);return*this;}
		constexpr string_t& operator=(constexpr_str_t&str)noexcept{_cso_reinit(str);return*this;}
		constexpr string_t& operator=(char_T ch)noexcept{_cso_reinit(ch);return*this;}
		//END_BLOCK

		//BLOCK: 字符串相加操作符
		[[nodiscard]]string_t operator+(const string_t& str)const&noexcept{
			full_copy_cso_check(str);
			if(str._in_cso())
				return operator+(str.to_string_view_t());
			else
				return ptr_copy()->apply_str_to_end(str._m);
		}
		[[nodiscard]]string_t operator+(string_view_t str)const&noexcept{
			return ptr_copy()->apply_str_to_end(str);
		}
		[[nodiscard]]string_t operator+(const char_T* str)const&noexcept{
			return *this+string_view_t(str);
		}
		friend [[nodiscard]]string_t operator+(string_view_t str1,const string_t& str2)noexcept{
			return str2.ptr_copy()->apply_str_to_begin(str1);
		}
		friend [[nodiscard]]string_t operator+(const char_T* str1,const string_t& str2)noexcept{
			return string_view_t(str1)+str2;
		}
		friend [[nodiscard]]string_t operator+(char_T ch,const string_t& str)noexcept{
			return string_view_t{&ch,1}+str;
		}

		string_t& operator+=(const string_t&str)&noexcept{
			push_back(str);
			return *this;
		}
		string_t& operator+=(string_view_t str)&noexcept{
			push_back(str);
			return *this;
		}
		string_t& operator+=(const char_T* str)&noexcept{
			return *this+=string_view_t(str);
		}
		string_t& operator+=(char_T ch)&noexcept{
			return *this += string_view_t{&ch,1};
		}
		template<typename U>
		[[nodiscard]]string_t&& operator+(U&& b)&&noexcept_as(*this+=b) requires was_not_an_ill_form(*this+=b){		  //对右值的operator+优化为operator+=
			*this+=b;
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
			auto ssize = size();
			auto scom = compare(ssize,a.size());//先比较大小，若需要再调用data
			if(scom==0){//大小相等
				if(a._in_str_cso())
					return operator<=>(a._get_cso_constexpr_str());
				elseif(_in_str_cso())
					return compare.reverse(a<=>_get_cso_constexpr_str());
				elseif(a._in_cso())
					return operator<=>(a.to_string_view_t());
				elseif(_in_cso())
					return compare.reverse(a<=>to_string_view_t());
				else
					return _m->compare_with(a._m);
			}
			return scom;
		}
		[[nodiscard]]constexpr auto operator==(const string_t& a)const noexcept(equal.nothrow<char_T>){
			auto ssize = size();
			const auto seq = equal(ssize,a.size());//先比较大小，若需要再调用data
			if(seq){//大小相等
				if(a._in_str_cso())
					return operator==(a._get_cso_constexpr_str());
				elseif(_in_str_cso())
					return a==_get_cso_constexpr_str();
				elseif(a._in_cso())
					return operator==(a.to_string_view_t());
				elseif(_in_cso())
					return a==to_string_view_t();
				else
					return _m->equal_with(a._m);
			}
			return seq;
		}
		[[nodiscard]]constexpr auto operator<=>(string_view_t a)const noexcept(compare.nothrow<char_T>){
			auto ssize = size();
			auto scom = compare(ssize,a.size());//先比较大小，若需要再调用data
			if(scom==0){
				if(_in_cso())
					return compare(data(),a.begin(),ssize);
				else
					return _m->compare_with(a);
			}
			return scom;
		}
		[[nodiscard]]constexpr auto operator==(string_view_t a)const noexcept(equal.nothrow<char_T>){
			auto ssize = size();
			const auto seq = equal(ssize,a.size());//先比较大小，若需要再调用data
			if(seq){
				if(_in_cso())
					return equal(data(),a.begin(),ssize);
				else
					return _m->equal_with(a);
			}
			return seq;
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
			auto tmp=operator==((string_view_t&)a);
			if(!tmp)
				remove_const(this)->_cso_reinit(a);
			return tmp;
		}
		[[nodiscard]]constexpr auto operator<=>(const char_T* a)const noexcept(compare.nothrow<char_T>){
			if(_in_cso())
				return compare(data(),size(),a,end_by_zero);
			else
				return _m->compare_with(a);
		}
		[[nodiscard]]constexpr auto operator==(const char_T* a)const noexcept(equal.nothrow<char_T>){
			if(_in_cso())
				return equal(data(),size(),a,end_by_zero);
			else
				return _m->equal_with(a);
		}
		//END_BLOCK
	private:
		char_T* unique_c_str()noexcept{ _cso_check(1);return _m->get_unique_c_str(_m); }
		char_T	arec(size_t index)noexcept{
			if(_in_cso())
				return _get_cso_data()[index];
			else
				return _m->arec(index);
		}
		void	arec_set(size_t index,char_T a)noexcept{ _cso_check(1);return _m->arec_set(index,a,_m); }

	public:
		class arec_t: non_copyable,non_moveable{
			string_t* _to;
			size_t	  _index;

			friend class string_t;
		public:
			arec_t(string_t* to,size_t index)noexcept:_to(to),_index(index){}
			arec_t(special_init_t,const arec_t&ref)noexcept:_to(ref._to),_index(ref._index){}
			[[nodiscard]]operator char_T()const noexcept{ return _to->arec(_index); }
			arec_t&		 operator=(char_T a)noexcept{
				_to->arec_set(_index,a);
				return *this;
			}
			arec_t&		 operator=(const arec_t&ch)noexcept{ return operator=(ch.operator char_T()); }
			[[nodiscard]]char_T*		operator&()noexcept{ return _to->unique_c_str()+_index; }
			[[nodiscard]]const char_T*	operator&()const noexcept{ return (add_const(_to))->c_str()+_index; }
			[[nodiscard]]operator char_T&()noexcept{ return *operator&(); }
			[[nodiscard]]operator const char_T&()const noexcept{ return *operator&(); }
		};

		[[nodiscard]]arec_t		  operator[](size_t index)noexcept{ return{this,index}; }
		[[nodiscard]]const arec_t operator[](size_t index)const noexcept{ return{remove_const(this),index}; }

		[[nodiscard]]string_t substr(size_t begin,size_t size=npos)const{
			size=min(size,this->size()-begin);
			if(size){
				_cso_check();
				return _m->get_substr_data(begin,size);
			}
			else
				return {};
		}
		[[nodiscard]]const char_T*	data()const noexcept{ if(_in_cso())return _get_cso_data();else return _m->get_data(_m); }
		char_T* writeable_data()noexcept{ return unique_c_str(); }
		[[nodiscard]]const char_T*	c_str()const noexcept{ if(_in_str_cso())return _get_cso_data();else return _m->get_const_c_str(_m); }
		char_T* writeable_c_str()noexcept{ return unique_c_str(); }
		[[nodiscard]]size_t			size()const noexcept{ if(_in_str_cso())return _get_cso_size();else return _m->get_size(); }
		void resize(size_t nsize,char_T ch ={}){
			auto size=this->size();
			if(size > nsize)
				*this=substr(0,nsize);
			elseif(size == nsize)
				return;
			else{
				_cso_check();
				_m=get<end_apply_string_data_t<char_T>>(_m,nsize-size,ch);
			}
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
			constexpr bool operator==(const iterator_base_t& a)const noexcept{ return _to==a._to && _index==a._index; }
			constexpr auto operator<=>(const iterator_base_t& a)const noexcept{ return _to==a._to ? _index<=>a._index : partial_ordering::unordered; }
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

		//

		void push_back(const string_t& str)noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_begin(to_string_view_t()));
			elseif(str._in_cso()){
				push_back(str.to_string_view_t());
			}
			else{
				_cso_check();
				_m=_m->apply_str_to_end(str._m);
			}
		}
		void push_back(string_view_t str)noexcept{ _cso_check();_m=_m->apply_str_to_end(str); }
		void push_back(char_T ch)noexcept{ push_back(string_view_t{&ch,1}); }
		void push_back(const arec_t& ch)noexcept{ push_back(ch.operator char_T()); }
		void push_back(const char_T* str)noexcept{ push_back(string_view_t(str)); }

		void push_front(const string_t& str)noexcept{
			full_copy_cso_check(*this);
			full_copy_cso_check(str);
			if(_in_cso()&&!str._in_cso())
				_cso_fin(str._m->apply_str_to_end(to_string_view_t()));
			elseif(str._in_cso()){
				push_front(str.to_string_view_t());
			}
			else{
				_cso_check();
				_m=_m->apply_str_to_begin(str._m);
			}
		}
		void push_front(string_view_t str)noexcept{ _cso_check();_m=_m->apply_str_to_begin(str); }
		void push_front(char_T ch)noexcept{ push_front(string_view_t{&ch,1}); }
		void push_front(const arec_t& ch)noexcept{ push_front(ch.operator char_T()); }
		void push_front(const char_T* str)noexcept{ push_front(string_view_t(str)); }

		string_t pop_back(size_t size)noexcept{ _cso_check();return _m->do_pop_back(size,_m); }
		string_t pop_front(size_t size)noexcept{ _cso_check();return _m->do_pop_front(size,_m); }
		char_T pop_back()noexcept{ return pop_back(1)[0]; }
		char_T pop_front()noexcept{ return pop_front(1)[0]; }

		//

		operator string_view_t()const&noexcept{ return string_view_t{data(),size()}; }
		auto to_string_view_t()const&noexcept{ return operator string_view_t(); }
		[[nodiscard]]explicit operator hash_t()const noexcept{ return _in_cso()?_get_cso_hash():_m->get_hash(_m); }

		//

		[[nodiscard]]size_t find(const char_T ch)const{
			return in_range_size_t(ch, to_string_view_t());
		}
		[[nodiscard]]size_t reverse_find(const char_T ch)const{
			return in_range_but_reverse_size_t(ch, to_string_view_t());
		}
		[[nodiscard]]size_t find(string_view_t str)const{
			return in_range_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t reverse_find(string_view_t str)const{
			return in_range_but_reverse_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t find(constexpr_str_t&str)const{
			return in_range_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t reverse_find(constexpr_str_t&str)const{
			return in_range_but_reverse_size_t(str, to_string_view_t());
		}
		[[nodiscard]]size_t find(const string_t&str)const{
			if(str._in_cso()){
				if(str._in_str_cso())
					return find(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find(str._cso_info._ch);
			}
			auto result = str._m->get_match_pattern_from_self(str._m).match(to_string_view_t());
			if(result)
				return result - data();
			else
				return npos;
		}
		[[nodiscard]]size_t reverse_find(const string_t&str)const{
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
		[[nodiscard]]size_t find_first_of(string_view_t str)const{
			return range_n::find_first_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(constexpr_str_t&str)const{
			return range_n::find_first_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(const string_t&str)const{
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_first_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find(str._cso_info._ch);
			}
			return find_first_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_first_of(const char_T*str)const{
			return find_first_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_first_of(char_T ch)const{
			return find(ch);
		}
		[[nodiscard]]size_t find_first_of(const arec_t&ch)const{
			return find(ch.operator char_T());
		}
		[[nodiscard]]size_t find_first_not_of(string_view_t str)const{
			return range_n::find_first_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(constexpr_str_t&str)const{
			return range_n::find_first_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(const string_t&str)const{
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_first_not_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find_first_not_of(str.to_string_view_t());
			}
			return find_first_not_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_first_not_of(const char_T*str)const{
			return find_first_not_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_first_not_of(char_T ch)const{
			return find_first_not_of(string_view_t(ch));
		}
		[[nodiscard]]size_t find_first_not_of(const arec_t&ch)const{
			return find_first_not_of(string_view_t(ch.operator char_T()));
		}
		[[nodiscard]]size_t find_last_of(string_view_t str)const{
			return range_n::find_last_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(constexpr_str_t&str)const{
			return range_n::find_last_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(const string_t&str)const{
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_last_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return reverse_find(str._cso_info._ch);
			}
			return find_last_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_last_of(const char_T*str)const{
			return find_last_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_last_of(char_T ch)const{
			return reverse_find(ch);
		}
		[[nodiscard]]size_t find_last_of(const arec_t&ch)const{
			return reverse_find(ch.operator char_T());
		}
		[[nodiscard]]size_t find_last_not_of(string_view_t str)const{
			return range_n::find_last_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(constexpr_str_t&str)const{
			return range_n::find_last_not_of_size_t(str,to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(const string_t&str)const{
			if(str._in_cso()){
				if(str._in_str_cso())
					return find_last_not_of(str._get_cso_constexpr_str());
				elseif(str._in_cso())
					return find_last_not_of(str.to_string_view_t());
			}
			return find_last_not_of(str.to_string_view_t());
		}
		[[nodiscard]]size_t find_last_not_of(const char_T*str)const{
			return find_last_not_of(string_view_t(str));
		}
		[[nodiscard]]size_t find_last_not_of(char_T ch)const{
			return find_last_not_of(string_view_t(ch));
		}
		[[nodiscard]]size_t find_last_not_of(const arec_t&ch)const{
			return find_last_not_of(string_view_t(ch.operator char_T()));
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
		void insert(size_t pos,const char_T* str)&noexcept{
			insert(pos,string_view_t(str));
		}
		void insert(size_t pos,char_T ch)&noexcept{
			insert(pos,string_view_t{&ch,1});
		}

		//

		//contains
		constexpr bool contains(string_view_t str)const noexcept{
			return find(str) != npos;
		}
		constexpr bool contains(constexpr_str_t&str)const noexcept{
			return find(str) != npos;
		}
		constexpr bool contains(const string_t&str)const noexcept{
			return find(str) != npos;
		}
		constexpr bool contains(char_T ch)const noexcept{
			return find(ch) != npos;
		}
		constexpr bool contains(const char_T*str)const noexcept{
			return find(str) != npos;
		}

		//starts_with
		constexpr bool starts_with(string_view_t str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(0,str.size()) == str;
		}
		constexpr bool starts_with(constexpr_str_t&str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(0,str.size()) == str;
		}
		constexpr bool starts_with(const string_t&str)const noexcept{
			if(size()<str.size())
				return false;
			if(str._in_char_cso())
				return starts_with(str._cso_info._ch);
			else
				return substr(0,str.size()) == str;
		}
		constexpr bool starts_with(char_T ch)const noexcept{
			if(size()<1)
				return false;
			return operator[](0) == ch;
		}
		constexpr bool starts_with(const char_T*str)const noexcept{
			return starts_with(string_view_t{str});
		}

		//ends_with
		constexpr bool ends_with(string_view_t str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(size()-str.size()) == str;
		}
		constexpr bool ends_with(constexpr_str_t&str)const noexcept{
			if(size()<str.size())
				return false;
			return substr(size()-str.size()) == str;
		}
		constexpr bool ends_with(const string_t&str)const noexcept{
			if(size()<str.size())
				return false;
			if(str._in_char_cso())
				return ends_with(str._cso_info._ch);
			else
				return substr(size()-str.size()) == str;
		}
		constexpr bool ends_with(char_T ch)const noexcept{
			if(size()<1)
				return false;
			return operator[](size()-1) == ch;
		}
		constexpr bool ends_with(const char_T*str)const noexcept{
			return ends_with(string_view_t{str});
		}
		/*
		compare
		replace
		*/
		#undef _m
	};

	//推导指引
	template<class char_T>
	string_t(const char_T*) -> string_t<char_T>;
	template<class char_T>
	string_t(char_T*) -> string_t<char_T>;

	//string的swap特化
	template<typename T>
	inline void swap(string_t<T>& a,string_t<T>& b)noexcept{ a.swap_with(b); }

	//std ostream支持
	template<typename some_fucking_std_ostream, typename T>
	decltype(auto) operator<<(some_fucking_std_ostream& stream, const string_t<T>& str) {
		typedef some_fucking_std_ostream stream_t;
		typedef stream_t::traits_type	 traits_t;
		typename stream_t::iostate		 state = stream_t::goodbit;

		#if defined(_MSC_VER)
			#pragma warning(push)
			#pragma warning(disable:26494)//未初始化警告diss
		#endif
		size_t pad;
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		size_t size = str.size();
		if(stream.width() <= 0 || static_cast<size_t>(stream.width()) <= size)
			pad = 0;
		else
			pad = static_cast<size_t>(stream.width()) - size;

		const typename stream_t::sentry isok(stream);

		if(!isok)
			state |= stream_t::badbit;
		else {
			try {
				if((stream.flags() & stream_t::adjustfield) != stream_t::left) {
					for(; 0 < pad; --pad) {		  // pad on left
						if(traits_t::eq_int_type(traits_t::eof(), stream.rdbuf()->sputc(stream.fill()))) {
							state |= stream_t::badbit;		 // insertion failed, quit
							break;
						}
					}
				}

				if(state == stream_t::goodbit && stream.rdbuf()->sputn(str.c_str(), static_cast<::std::streamsize>(size)) != static_cast<::std::streamsize>(size))
					state |= stream_t::badbit;
				else {
					for(; 0 < pad; --pad) {		  // pad on right
						if(traits_t::eq_int_type(traits_t::eof(), stream.rdbuf()->sputc(stream.fill()))) {
							state |= stream_t::badbit;		 // insertion failed, quit
							break;
						}
					}
				}

				stream.width(0);
			}
			catch(...) {
				stream.setstate(stream_t::badbit, true);
				return stream;
			}
		}
		stream.setstate(state);
		return stream;
	}

	//array like支持
	template<class T>
	[[nodiscard]]inline auto size_of_array_like(const string_t<remove_cv<T>>& a)noexcept{ return a.size(); }
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(string_t<remove_cv<T>>& a)noexcept{ return(T*)a.c_str(); }
	template<class T>
	[[nodiscard]]inline auto begin_of_array_like(const string_t<remove_cv<T>>& a)noexcept{ return(const T*)a.c_str(); }

	//typedef
	typedef string_t<char_t>string;
}
using string_n::string_t;
using string_n::string;

//file_end

