//_body.hpp
//at namespace elc
/*
未完成的elc解释器string文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
class string{
	::std::basic_string<eluint> data;
public:
	typedef iterator data::iterator;
	typedef const_iterator data::const_iterator;

	string()noexcept=default;
	string(string&)=default;
	string(string&&)noexcept=default;
	string&operator=(string&)&=default;
	string&operator=(string&&)noexcept&=default;

	eluint&operator[]
	operator T*
	operator+
	iter begin
	iter head//begin-1
	iter last
	iter end//last+1
	iter find_first(T)
	iter find_last(T)
	size
	empty
	operator==
	operator!=
	operator=
	copy
	move
	for_each
};
