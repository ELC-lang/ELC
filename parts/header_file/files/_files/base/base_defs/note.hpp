//note.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace note_n{
	#define def_note(name)\
	template<typename T>\
	struct name##_t{\
		T value;\
		constexpr name##_t(T a):value(a){}\
		template<class U>\
		constexpr name##_t(name##_t<U>a):value(a.value){}\
		constexpr operator T(){return value;}\
		constexpr T operator()(){return value;}\
	};\
	template<typename T>\
	constexpr name##_t<T>name(T v){return{v};}

	def_note(from);
	def_note(to);
	def_note(size);

	#undef def_note
	constexpr struct fail_t{}fail{};
}
namespace note=note_n;

//file_end

