//attribute.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T,typename attribute_name>
struct attribute:basic_struct{
protected:
	friend T*get_handle(attribute<T,attribute_name>*a);
	constexpr T*get_handle()noexcept{return static_cast<T*>(static_cast<attribute_name*>(this));}
	constexpr const T*get_handle()const noexcept{return static_cast<const T*>(static_cast<const attribute_name*>(this));}
};
template<class T>
T*get_handle(T*a){return a;}
template<class T,typename attribute_name>
T*get_handle(attribute<T,attribute_name>*a){return get_handle(a->get_handle());}

//file_end

