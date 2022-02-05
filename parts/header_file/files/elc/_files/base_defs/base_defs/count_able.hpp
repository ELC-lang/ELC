//count_able.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
struct count_able:is_common_attribute(count_able){
private:
	friend size_t the_number_of(type_info_t<T>)noexcept;
	static size_t _number;
public:
	count_able()noexcept{_number++;}
	~count_able()noexcept{_number--;}
};
template<class T>
[[nodiscard]]inline size_t the_number_of(type_info_t<T>)noexcept{
	if constexpr(type_info<T>.not_has_attribute(count_able))
		template_error("hey.");
	return count_able<T>::_number;
}

//file_end

