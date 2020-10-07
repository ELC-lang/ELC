//defs.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	template<class T>
	class function_t;
	template<class Ret_t,class...Args_t,bool nothrow>
	struct function_t<Ret_t(Args_t...)noexcept(nothrow)>:base_function_t<Ret_t(Args_t...),nothrow,true>{
		typedef function_t<Ret_t(Args_t...)noexcept(nothrow)>this_t;
		typedef base_function_t<Ret_t(Args_t...),nothrow,true>base_t;
		using base_t::base_t;
		template<class assign_t,enable_if_not_ill_from(declvalue(base_t)=declvalue(assign_t))>
		this_t&operator=(assign_t&&a)&noexcept_as(declvalue(base_t)=declvalue(assign_t)){
			base_t::operator=(forward<assign_t>(a));
			return*this;
		}
	};
	template<class T>
	void swap(function_t<T>&a,function_t<T>&b)noexcept{
		a.swap_with(b);
	}

	template<class T>
	class may_throw_in_destruct_function_t;
	template<class Ret_t,class...Args_t,bool nothrow>
	//[[deprecated("not safe")]]//用这么长名字的人大概知道自己在干啥
	struct may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept(nothrow)>:base_function_t<Ret_t(Args_t...),nothrow,false>{
		typedef may_throw_in_destruct_function_t<Ret_t(Args_t...)noexcept(nothrow)>this_t;
		typedef base_function_t<Ret_t(Args_t...),nothrow,false>base_t;
		using base_t::base_t;
		template<class assign_t,enable_if_not_ill_from(declvalue(base_t)=declvalue(assign_t))>
		this_t&operator=(assign_t&&a)&noexcept_as(declvalue(base_t)=declvalue(assign_t)){
			base_t::operator=(a);
			return*this;
		}
	};
	template<class T>
	void swap(may_throw_in_destruct_function_t<T>&a,may_throw_in_destruct_function_t<T>&b)noexcept{
		a.swap_with(b);
	}
}

//file_end

