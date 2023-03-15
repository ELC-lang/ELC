//_body.hpp
//at namespace elc::defs::stream_n
/*
未完成的elc解释器base_stream文件
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
//warp for not_unreadable_istream to make it unreadable
template<class stream_T> requires not_unreadable_istream_class<stream_T>
struct unreadable_wrap:stream_T,instance_struct<unreadable_wrap<stream_T>>,virtual conditional<
	noexcept_stream_class<stream_T>,
		conditional<data_stream_class<stream_T>,
			noexcept_data_istream_t
		,
			noexcept_text_istream_t<typename stream_T::char_type>
	>,
		conditional<data_stream_class<stream_T>,
			data_istream_t
		,
			text_istream_t<typename stream_T::char_type>
	>
>{
private:
	typedef stream_T base_t;
	typedef unreadable_wrap<stream_T> this_t;
	override_instance_struct;
public:
	using stream_T::stream_T;
	typedef stream_T::data_type data_type;
private:
	static constexpr size_t default_buffer_size=1<<10;

	array_t<data_type> _buffer{note::size(default_buffer_size)};
	size_t _using_size=default_buffer_size,_reading_pos=default_buffer_size;
protected:
	virtual size_t base_read_impl_for_wrapper(data_type*data,size_t size)noexcept_as(stream_T::read(nullptr,0)){
		return stream_T::read(data,size);
	}
	virtual bool base_waitting_for_data_impl_for_wrapper()noexcept_as(stream_T::waitting_for_data()){
		return stream_T::waitting_for_data();
	}
public:
	virtual size_t read(data_type*data,size_t size)noexcept_as(stream_T::read(nullptr,0)) override{
		if(size==0)return 0;
		size_t aret=0;
		if(_reading_pos!=_using_size){
			const auto read_size=min(size,_using_size-_reading_pos);
			copy_assign[read_size](note::from(add_const(_buffer.data()+_reading_pos)),note::to(data));
			_reading_pos+=read_size;
			size-=read_size;
			aret+=read_size;
			data+=read_size;
		}
		aret+=base_read_impl_for_wrapper(data,size);
		return aret;
	}
	virtual void unread(const data_type*data,size_t size)noexcept override{
		if(size==0)return;
		if(_reading_pos<size){//unread size more than buffer can hold
			const auto more_size=size-_reading_pos;
			_buffer.forward_resize(more_size);
			_using_size+=more_size;
			_reading_pos+=more_size;
		}
		_reading_pos-=size;
		copy_assign[size](note::from(data),note::to(_buffer.data()+_reading_pos));
	}
	virtual bool is_end()noexcept_as(stream_T::is_end())override{
		return _reading_pos==_using_size && stream_T::is_end();
	}
	virtual bool waitting_for_data()noexcept_as(stream_T::waitting_for_data())override{
		return _reading_pos==_using_size && stream_T::waitting_for_data();
	}
};

//file_end

