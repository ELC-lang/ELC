//type_name.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
struct type_name_t:string_view_t<char>{
	//对于gcc和clang，这个类型有一个malloced的char*，以便于在析构时释放
	//一切源于abi::__cxa_demangle
	//对于msvc，这个类型析构时不需要做任何事情
	typedef string_view_t<char> base_t;
	//重整器包装
	static base_t demangle(const char*original_name)noexcept{
		size_t len;
		const char*demangled_name;
		#if defined(__clang__)||defined(__GNUC__)
			//gcc和clang
			//使用abi::__cxa_demangle重整
			int status;
			demangled_name=::abi::__cxa_demangle(original_name,nullptr,&len,&status);
			if(status!=0){
				demangled_name=original_name;
				len=::std::strlen(original_name);
				::std::free((void*)demangled_name);
			}
		#else
			//msvc
			//不需要重整
			demangled_name=original_name;
			len=::std::strlen(original_name);
		#endif
		return base_t{demangled_name,len};
	}
	//复制重整结果
	static base_t copy_demangle(base_t demangled_name)noexcept{
		#if defined(__clang__)||defined(__GNUC__)
			//gcc和clang
			//需要复制重整结果（用malloced的char*）
			char*copyed_name=(char*)::std::malloc(demangled_name.size()+1);
			if(copyed_name==nullptr)
				return base_t{nullptr,0};
			::std::memcpy(copyed_name,demangled_name.data(),demangled_name.size());
			copyed_name[demangled_name.size()]='\0';
			return base_t{copyed_name,demangled_name.size()};
		#else
			//msvc
			//不需要复制重整结果
			return demangled_name;
		#endif
	}
	//构造函数
	type_name_t(const char*original_name)noexcept:base_t(demangle(original_name)){}
	//复制构造函数
	type_name_t(const type_name_t&other)noexcept:base_t(copy_demangle(other)){}
	//移动构造函数（swap）
	type_name_t(type_name_t&&other)noexcept:base_t(nullptr,0){
		swap_with(other);
	}
	//析构函数
	~type_name_t()noexcept{
		#if defined(__clang__)||defined(__GNUC__)
			//gcc和clang
			//需要释放malloced的char*
			::std::free((void*)begin());
		#endif
	}
};
[[nodiscard]]inline type_name_t base_type_info_t::get_name()const noexcept{
	return _tid.get_name();
}
[[nodiscard]]inline type_name_t base_type_info_t::type_id_t::get_name()const noexcept{
	return _m->name();
}
template<typename T>
[[nodiscard]]inline type_name_t type_info_t<T>::get_name()const noexcept{
	return base_type_info_t(*this).get_name();
}

//file_end

