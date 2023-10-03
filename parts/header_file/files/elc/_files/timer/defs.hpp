//defs.hpp
//at namespace elc::defs::timer_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/
struct debug_timer_t{
private:
	typedef ::std::chrono::high_resolution_clock my_clock;
	my_clock::time_point _start,_end;
public:
	void start(){ _start = my_clock::now(); }
	void end(){ _end = my_clock::now(); }
	void reset(){ start(); }
	//暂停和继续
	void pause(){ end(); }
	void resume(){_start += my_clock::now() - _end; }
	//获取时间
	typedef ::std::chrono::duration<float_t,::std::nano> nano_t;
	typedef ::std::chrono::duration<float_t,::std::micro> micro_t;
	typedef ::std::chrono::duration<float_t,::std::milli> milli_t;
	typedef ::std::chrono::duration<float_t,::std::ratio<1>> second_t;
	typedef ::std::chrono::duration<float_t,::std::ratio<60>> minute_t;

	template<typename T>
	T get()const{
		return ::std::chrono::duration_cast<T>(_end - _start);
	}
};

BREAK_NAMESPACE
namespace stream_n{
	template<typename value_T,typename ratio_T,text_ostream stream_T>
	decltype(auto) operator<<(stream_T&&stream,const::std::chrono::duration<value_T,ratio_T>& duration)noexcept(noexcept_text_ostream<stream_T>){
		stream << duration.count();
		if constexpr(type_info<ratio_T> == type_info<::std::nano>)
			stream << es"ns";
		elseif constexpr(type_info<ratio_T> == type_info<::std::micro>)
			stream << es"us";
		elseif constexpr(type_info<ratio_T> == type_info<::std::milli>)
			stream << es"ms";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<1>>)
			stream << es"s";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<60>>)
			stream << es"m";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<3600>>)
			stream << es"h";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<86400>>)
			stream << es"d";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<604800>>)
			stream << es"w";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<2592000>>)
			stream << es"M";
		elseif constexpr(type_info<ratio_T> == type_info<::std::ratio<31536000>>)
			stream << es"y";
		else
			template_error_of_type(ratio_T,"Unsupported ratio type.");
		return forward<stream_T>(stream);
	}
}
INTER_NAMESPACE(timer_n)

//file_end

