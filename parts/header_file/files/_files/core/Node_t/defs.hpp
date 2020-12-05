//defs.hpp
//at namespace elc::defs::core::Node_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace Node_n{
	template<typename T>
	class Node_t{
	protected:
		ptr _value;
	public:
		typedef Node_t<T> this_t;
	private:

	public:
		[[nodiscard]]operator T&()noexcept(ptr::get_nothrow){
			return static_cast<T*>(_value.get())
		}
		[[nodiscard]]logical_bool operator ==(node_like&a){
			return a.equal(*_value);
		}
		[[nodiscard]]ptr operator &()noexcept{
			return _value;
		}
		[[nodiscard]]explicit operator ptr()noexcept{
			return _value;
		}
		[[nodiscard]]operator[]
		operator()
		eval
		[[nodiscard]]operator logical_bool
		[[nodiscard]]explicit operator bool
	};
	template<typename T>
	inline void swap(Node_t<T>&a,Node_t<T>&b)noexcept{a.swap_with(b);}
}

//file_end

