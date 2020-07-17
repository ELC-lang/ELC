//_body.hpp
//at namespace elc::defs::log_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
enum log_type{error,warning,info,note};
struct logger:noexcept_ostream{
private:
	noexcept_ostream*_tie_with=nullptr;
	bool _has_info=1;
	void(*_destroyer)(noexcept_ostream*)noexcept=nullptr;
public:
	constexpr logger()noexcept=default;
	~logger()noexcept{
		if(_destroyer && _tie_with)
			_destroyer(_tie_with);
	}
	void tie_with(noexcept_ostream*ostm,void(*destroyer)(noexcept_ostream*)noexcept)noexcept{
		_tie_with=ostm;
		_destroyer=destroyer;
	}
	void new_line()(log_type type=info)noexcept{
		if(_tie_with){
			if(!_has_info)_tie_with->write("no info for this.\n");
			_tie_with->write("[");
			switch type{
				case error:
					_tie_with->write("error");
					//break_point
				case info:
					_tie_with->write("info");
				case warning:
					_tie_with->write("warning");
				default:
					_tie_with->write("note");
			}
			_tie_with->write("]");
			_has_info=0;
		}
	}
	virtual void write(const void*buf,size_t size)noexcept override final{
		_has_info=1;
		if(_tie_with)
			_tie_with->write(buf,size);
	}
};
