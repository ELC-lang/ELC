//pre_checks.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//__cplusplus needs 20+
#if !defined(__cplusplus)||__cplusplus<202002L
	//if msvc, check _MSVC_LANG
	#if defined(_MSVC_LANG) && _MSVC_LANG>=202002L
		//ok
	#else
		#error "elc files needs c++20 or higher"
	#endif
#endif

//check if the compiler is msvc
#if defined(_MSC_VER)
	//check if _CPPRTTI
	#if !defined(_CPPRTTI)
		#error "elc files needs rtti"
	#endif
	//check _MSC_VER at last 1932
	#if _MSC_VER<1932
		#error "elc files needs at last msvc 19.32 or higher, keep moving!"
	#endif
#endif

//file_end

