//function_data_warpper_t.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/

/*
用处：从任何callable类型（除过函数类型，但可以是其指针）提供operator()
小包装工具，能在意想不到的地方发挥效果（比如要额外携带数据的callable类型定义！见lib_loader.hpp）
*/
template<class Func_t,class T=Func_t*>
class function_data_warpper_t;
template<class T,bool nothrow,class Ret_t,class...Args_t>
struct function_data_warpper_t<Ret_t(Args_t...)noexcept(nothrow),T>{
	static_assert(!::std::is_function_v<T>);

	T _value;

	function_data_warpper_t(T&a)noexcept(construct<T>.nothrow<T>):_value(a){}
	~function_data_warpper_t()noexcept(destruct.nothrow<T>)=default;
	Ret_t operator()(Args_t...args)noexcept(nothrow){
		//BLOCK:constexpr checks
		if constexpr(!invoke<T>.able<Args_t...>)
			template_error("this T can\'t becall as args.");
		if constexpr(type_info<decltype(declvalue(T)(declvalue(Args_t)...))> != type_info<Ret_t>)
			template_error("the return type of T was wrong.");
		//BLOCK_END
		return _value(forward<Args_t>(args)...);
	}
	[[nodiscard]]auto&get_data()noexcept{return _value;}
}

//file_end

