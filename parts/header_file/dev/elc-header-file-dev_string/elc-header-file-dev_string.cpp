// elc-header-file-dev_string.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
未完成的elc解释器core文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#include <cstdlib>
namespace std{//cpp20还未实现，占位。
	#if defined(_MSC_VER)||defined(__clang__)
	[[nodiscard]]inline void* aligned_alloc([[maybe_unused]] std::size_t alignment,std::size_t size){ return malloc(size); }
	#endif
}
#include "../../files/base"
#include "../../files/base_exception"


#include "../../files/_files/_share/_defs.hpp"
namespace elc::defs{
	namespace string_n{
		inline namespace string_data_n{
			template<typename char_T>
			struct base_string_data_t:type_info_t<base_string_data_t<char_T>>::template_name with_common_attribute<abstract_base,never_in_array>,ref_able<base_string_data_t<char_T>>,build_by_get_only{
				typedef base_string_data_t<char_T> this_t;
				typedef comn_ptr_t<this_t> ptr_t;
				typedef array_like_view_t<const char_T> string_view_t;

				base_string_data_t()noexcept=default;
				base_string_data_t(never_ref_num_zero_t)noexcept:ref_able<this_t>(never_ref_num_zero){}

				bool is_unique()noexcept{ return get_ref_num(this)==1; }

				virtual ~base_string_data_t()=default;

				virtual char_T* get_c_str(ptr_t&);
				virtual size_t get_size()=0;
				virtual ptr_t get_substr_data(size_t begin,size_t size);
				virtual ptr_t apply_str_to_begin(string_view_t str);
				virtual ptr_t apply_str_to_begin(ptr_t str);
				virtual ptr_t apply_str_to_end(string_view_t str);
				virtual ptr_t apply_str_to_end(ptr_t str);

				//virtual ptr_t do_insert(size_t pos,string_view_t str);
				//virtual ptr_t do_insert(size_t pos,ptr_t str);
				virtual ptr_t do_erase(size_t pos,size_t size);

				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)=0;
				virtual char_T& arec(size_t index)=0;
			};

			template<typename char_T>
			struct comn_string_data_t final:base_string_data_t<char_T>,instance_struct<comn_string_data_t<char_T>>{
				typedef comn_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				array_t<char_T> _m;

				comn_string_data_t(string_view_t str):_m(note::size(str.size()+1)){
					copy_assign[str.size()](note::form(str.begin()),note::to((char_T*)_m));
				}
				comn_string_data_t(ptr_t str):_m(note::size(str->get_size()+1)){
					str->copy_part_data_to((char_T*)_m,0,this->get_size());
				}

				virtual char_T* get_c_str(ptr_t&)override final{ return (char_T*)_m; }
				virtual size_t get_size()override final{ return _m.size()-1; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ copy_assign[size](note::form((const char_T*)_m),note::to(to)); }
				virtual char_T& arec(size_t index)override final{ return _m[index]; }
			};
			template<typename char_T>
			char_T* base_string_data_t<char_T>::get_c_str(ptr_t& a){
				auto comn_data=get<comn_string_data_t<char_T>>(a);
				a=comn_data;
				return comn_data->get_c_str(a);
			}

			template<typename char_T>
			struct null_string_data_t final:base_string_data_t<char_T>,instance_struct<null_string_data_t<char_T>>{
				typedef null_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				null_string_data_t()noexcept:base_t(never_ref_num_zero){}

				virtual char_T* get_c_str(ptr_t&)override final{
					static char_T data[1]{};
					return data;
				}
				virtual size_t get_size()override final{ return 0; }
				virtual ptr_t get_substr_data(size_t begin,size_t size)override final{ return this; }
				virtual ptr_t apply_str_to_begin(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				virtual ptr_t apply_str_to_begin(ptr_t str)override final{ return str; }
				virtual ptr_t apply_str_to_end(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				virtual ptr_t apply_str_to_end(ptr_t str)override final{ return str; }

				//virtual ptr_t do_insert(size_t pos,string_view_t str);
				//virtual ptr_t do_insert(size_t pos,ptr_t str);
				virtual ptr_t do_erase(size_t pos,size_t size)override final{ return this; }

				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ return; }
				virtual char_T& arec(size_t index)override final{ return*(char_T*)null_ptr; }
			};
			template<typename char_T>
			inline null_string_data_t<char_T> null_string_data{};
			template<typename char_T>
			base_string_data_t<char_T>* get_null_ptr(const base_string_data_t<char_T>*){ return&null_string_data<char_T>; }

			template<typename char_T>
			struct substr_string_data_t final:base_string_data_t<char_T>,instance_struct<substr_string_data_t<char_T>>{
				typedef substr_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				ptr_t _to;
				size_t _sub_begin;
				size_t _sub_size;

				substr_string_data_t(ptr_t str,size_t sub_begin,size_t sub_size):_to(str),_sub_begin(sub_begin),_sub_size(sub_size){}

				virtual char_T* get_c_str(ptr_t&)override final{ return _to->get_c_str(_to)+_sub_begin; }
				virtual size_t get_size()override final{ return _sub_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ _to->copy_part_data_to(to,pos+_sub_begin,size); }
				virtual char_T& arec(size_t index)override final{ return _to->arec(index+_sub_begin); }
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::get_substr_data(size_t begin,size_t size){
				return get<substr_string_data_t<char_T>>(this,begin,size);
			}

			template<typename char_T>
			struct end_apply_string_data_t final:base_string_data_t<char_T>,instance_struct<end_apply_string_data_t<char_T>>{
				typedef end_apply_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				array_t<char_T> _m;
				ptr_t _to;
				size_t _to_size;
				size_t _used_size;

				end_apply_string_data_t(ptr_t str,string_view_t end):
					_to_size(str->get_size()),
					_used_size(end.size()),
					_m(note::size(size_t((_to_size+_used_size)*magic_number::gold_of_resize))),
					_to(str)
				{
					copy_assign[_used_size](note::form(end.begin()),note::to((char_T*)_m));
				}

				virtual size_t get_size()override final{ return _used_size+_to_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
					if(pos+size<_to_size)
						_to->copy_part_data_to(to,pos,size);
					else{
						if(pos<_to_size){
							_to->copy_part_data_to(to,pos,_to_size-pos);
							auto copied_size=_to_size-pos;
							pos=0;
							size-=copied_size;
							to+=copied_size;
						}
						else
							pos-=_to_size;
						copy_assign[size](note::form((const char_T*)_m+pos),note::to(to));
					}
				}
				virtual char_T& arec(size_t index)override final{
					if(index<_to_size)
						return _to->arec(index);
					else
						return _m[index-_to_size];
				}

				virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique()){
						if(_m.size()-_used_size < str.size()){
							auto size_now=this->get_size()+str.size();
							auto size_new=size_t(size_now*magic_number::gold_of_resize);
							_m.resize(size_new);
						}
						copy_assign[str.size()](note::form(str.begin()),note::to((char_T*)_m+_used_size));
						_used_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				virtual ptr_t apply_str_to_end(ptr_t str)override final{
					this->shrink();
					return base_t::apply_str_to_end(str);
				}
				void shrink(){
					_m.resize(_used_size);
				}
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(string_view_t str){
				return get<end_apply_string_data_t<char_T>>(this,str);
			}

			template<typename char_T>
			struct head_apply_string_data_t final:base_string_data_t<char_T>,instance_struct<head_apply_string_data_t<char_T>>{
				typedef head_apply_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				array_t<char_T> _m;//意义为向前延续的数组：当需要向前附加内容时向前拓展以避免重新分配内存
				ptr_t _to;
				size_t _to_size;
				size_t _used_size;

				head_apply_string_data_t(ptr_t str,string_view_t head):
					_to_size(str->get_size()),
					_used_size(head.size()),
					_m(note::size(size_t((_to_size+_used_size)*magic_number::gold))),
					_to(str)
				{
					copy_assign[_used_size](note::form(head.begin()),note::to((char_T*)_m));
				}

				virtual size_t get_size()override final{ return _used_size+_to_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
					if(pos<_used_size){
						const char_T* head_begin=_m.end()-_used_size;
						const char_T* head_end=_m.end();
						const char_T* copy_begin=pos+head_begin;
						size_t size_of_copy_from_head=min(size_t(head_end-copy_begin),size);

						copy_assign[size_of_copy_from_head](note::form(copy_begin),note::to(to));
						if(size!=size_of_copy_from_head){
							size_t size_left=size-size_of_copy_from_head;
							char_T* next_copy_begin_pos=to+size_of_copy_from_head;
							_to->copy_part_data_to(next_copy_begin_pos,0,size_left);
						}
					}
					else
						_to->copy_part_data_to(to,pos-_used_size,size);
				}
				virtual char_T& arec(size_t index)override final{
					if(index<_used_size)
						return _m[index];
					else
						return _to->arec(index-_used_size);
				}
				virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique()){
						if(_m.size()-_used_size<str.size()){
							auto size_now=this->get_size()+str.size();
							auto size_new=size_t(size_now*magic_number::gold_of_resize);
							_m.forward_resize(size_new);
						}
						copy_assign[str.size()](note::form(str.begin()),note::to(_m.end()-_used_size-str.size()));
						_used_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					this->shrink();
					return base_t::apply_str_to_begin(str);
				}
				void shrink(){
					_m.forward_resize(_used_size);
				}
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(string_view_t str){
				return get<head_apply_string_data_t<char_T>>(this,str);
			}

			template<typename char_T>
			struct sum_string_data_t final:base_string_data_t<char_T>,instance_struct<sum_string_data_t<char_T>>{
				typedef sum_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				ptr_t _defore;
				ptr_t _after;
				size_t _defore_size;
				size_t _after_size;

				sum_string_data_t(ptr_t defore,ptr_t after):_defore(defore),_after(after),_defore_size(_defore->get_size()),_after_size(_after->get_size()){}

				virtual size_t get_size()override final{ return _defore_size+_after_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
					if(pos<_defore_size){
						auto copy_defore_begin=pos;
						auto copy_defore_end=min(pos+size,_defore_size);
						auto copy_defore_size=copy_defore_end-copy_defore_begin;
						_defore->copy_part_data_to(to,copy_defore_begin,copy_defore_size);
						if(size!=copy_defore_size){
							auto copy_after_size=size-copy_defore_size;
							to+=copy_defore_size;
							_after->copy_part_data_to(to,0,copy_after_size);
						}
					}
					else
						_after->copy_part_data_to(to,pos-_defore_size,size);
				}
				virtual char_T& arec(size_t index)override final{
					if(index<_defore_size)
						return _defore->arec(index);
					else
						return _after->arec(index-_defore_size);
				}
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(ptr_t str){
				return get<sum_string_data_t<char_T>>(this,str);
			}
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(ptr_t str){
				return get<sum_string_data_t<char_T>>(str,this);
			}

			template<typename char_T>
			struct erased_string_data_t final:base_string_data_t<char_T>,instance_struct<erased_string_data_t<char_T>>{
				typedef erased_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				ptr_t _to;
				size_t _to_size;
				size_t _erase_pos;
				size_t _erase_size;

				erased_string_data_t(ptr_t str,size_t erase_pos,size_t erase_size):_to(str),_to_size(_to->get_size()),_erase_pos(erase_pos),_erase_size(erase_size){}

				virtual size_t get_size()override final{ return _to_size-_erase_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
					if(pos+size<_erase_pos)
						_to->copy_part_data_to(to,pos,size);
					elseif(pos>_erase_pos)
						_to->copy_part_data_to(to,pos+_erase_size,size);
					else{
						auto size_defore_erase_pos=_erase_pos-pos;
						auto size_after_erase_pos=size-size_defore_erase_pos;
						_to->copy_part_data_to(to,pos,size_defore_erase_pos);
						_to->copy_part_data_to(to+size_defore_erase_pos,_erase_pos+_erase_size,size_after_erase_pos);
					}
				}
				virtual ptr_t do_erase(size_t pos,size_t size)override final{
					if(this->is_unique()){
						if(pos<=_erase_pos && pos+size>=_erase_size){
							_erase_pos=pos;
							_erase_size+=size;
							return this;
						}
					}
					return base_t::do_erase(pos,size);
				}
				virtual char_T& arec(size_t index)override final{
					if(index>_erase_pos)
						return _to->arec(index+_erase_size);
					else
						return _to->arec(index);
				}
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size){
				return get<erased_string_data_t<char_T>>(this,pos,size);
			}
		}
		template<typename char_T>
		struct string_t{
			typedef base_string_data_t<char_T> base_t_w;
			typedef base_t_w::ptr_t ptr_t;
			typedef base_t_w::string_view_t string_view_t;
		private:
			ptr_t _m;

			string_t(ptr_t str):_m(ptr_t){}
		public:
			string_t(string_view_t str):_m(get<comn_string_data_t<char_T>>(str)){}
			string_t(array_end_by_zero_t<const char_T> str):string_t((string_view_t)(str)){}
			string_t(const char_T* str):string_t(array_end_by_zero_t<const char_T>(str)){}
			string_t(const string_t& str)=default;
			string_t(string_t&&str)=default;

			string_t& operator=(const string_t& str)=default;
			string_t& operator=(string_t&&str)=default;

			string_t& operator+=(string_t str){
				_m=_m->apply_str_to_end(str._m);
				return*this;
			}
			string_t& operator+=(string_view_t str){
				_m=_m->apply_str_to_end(str);
				return*this;
			}
			string_t& operator+=(const char_T* str){
				return *this+=array_end_by_zero_t<const char_T>(str);
			}

			size_t size(){ return _m->get_size(); }
		};
	}
}
namespace elc{
	using defs::string_n::string_t;
}
#include "../../files/_files/_share/_undefs.hpp"

#include <steve.h>

void ste::stst()
{
	elc::string_t<char> a="";
	a="asd";
	stest_accert(a.size()==3);
	a+="asd";
	stest_accert(a.size()==6);
}
