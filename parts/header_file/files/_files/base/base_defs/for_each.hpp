//for_each.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//UF
namespace for_each_n{
	template<class T,class Func>requires was_not_an_ill_form(a(b))
	[[nodiscard]]constexpr_as_auto inline auto for_each(
		range_t<T>a,

	)noexcept(){

	}
	template<class T>
	[[nodiscard]]inline hash_t hash(const T*a,size_t size)noexcept_as(hash(*a)){
		hash_t aret=0;
		while(size--)
			aret=hash(a[size])+aret._value*13;
		return aret;
	}
	template<class T> requires is_not_signal_value_for_array_like<T>
	[[nodiscard]]inline hash_t hash(array_like_view_t<T>a)noexcept_as(hash(declvalue(T))){
		return hash(a.begin(),a.size());
	}
}

//file_end

