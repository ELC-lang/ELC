//_defs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//copy&rewrite from steve02081504/steve.h
#define linux 2
#define windows 3
#define other 4
#if defined(__linux)||defined(__linux__)||defined(__gnu_linux__)
	#define SYSTEM_TYPE linux
#elif defined(WIN32)||defined(_WIN32)||defined(_WIN64)||defined(WIN64)||defined(_WIN128)||defined(WIN128)||defined(_WIN256)||defined(WIN256)//wdnmd
	#define SYSTEM_TYPE windows
#else
	#define SYSTEM_TYPE other
#endif
#define ERROR_MSG_UNABLE_OS "Unable OS,edit this file on your own or contact ELC's producers."

//file_end

