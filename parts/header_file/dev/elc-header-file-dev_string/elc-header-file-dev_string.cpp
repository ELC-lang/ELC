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

#include <steve.h>

#define ELC_TEST_CHECK_MEMORY_LACK
#include "../../files/base"
#include "../../files/base_exception"


#include "../../files/_files/_share/_defs.hpp"
namespace elc::defs{
	namespace string_n{
		inline namespace string_data_n{
			template<typename char_T>
			struct base_string_data_t:type_info_t<base_string_data_t<char_T>>::template_name
			with_common_attribute<abstract_base,never_in_array,replace_able,ref_able>,build_by_get_only{
				typedef base_string_data_t<char_T> this_t;
				typedef comn_ptr_t<this_t> ptr_t;
				typedef array_like_view_t<const char_T> string_view_t;

				base_string_data_t()noexcept=default;
				base_string_data_t(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->never_ref_num_zero(); }

				[[nodiscard]]bool is_unique()noexcept{ return get_ref_num(this)==1; }
				virtual void be_replace_as(ptr_t a)=0;

				virtual ~base_string_data_t()=default;

				[[nodiscard]]virtual char_T* get_c_str();
				[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&);
				[[nodiscard]]virtual size_t get_size()=0;
				[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size);
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str);
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str);
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str);
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str);

				//[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str);
				//[[nodiscard]]virtual ptr_t do_insert(size_t pos,ptr_t str);
				[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size);

				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)=0;
				[[nodiscard]]virtual char_T& arec(size_t index)=0;
				virtual void arec_set(size_t index,char_T a,ptr_t&p)=0;
			};
			template<typename char_T>
			void base_string_data_t<char_T>::be_replace_as(ptr_t a){
				replace_able<this_t>::be_replace_as(a.get());
			}

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

				virtual void be_replace_as(ptr_t a)override final{
					_m.clear();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual char_T* get_c_str()override final{ return (char_T*)_m; }
				[[nodiscard]]virtual size_t get_size()override final{ return _m.size()-1; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ copy_assign[size](note::form((const char_T*)_m),note::to(to)); }
				[[nodiscard]]virtual char_T& arec(size_t index)override final{ return _m[index]; }
				virtual void arec_set(size_t index,char_T a,ptr_t&p)override final{
					if(this->is_unique())
						_m[index]=a;
					else
						base_t::arec_set(index,a,p);
				}
			};
			template<typename char_T>
			[[nodiscard]]char_T* base_string_data_t<char_T>::get_c_str(){
				auto comn_data=get<comn_string_data_t<char_T>>(this);
				this->be_replace_as(comn_data);
				return comn_data->get_c_str();
			}
			template<typename char_T>
			[[nodiscard]]char_T* base_string_data_t<char_T>::get_unique_c_str(ptr_t&a){
				auto comn_data=get<comn_string_data_t<char_T>>(this);
				a=comn_data;
				return comn_data->get_c_str();
			}
			template<typename char_T>
			void elc::defs::string_n::string_data_n::base_string_data_t<char_T>::arec_set(size_t index,char_T a,ptr_t& p){
				this->get_unique_c_str(p)[index]=a;
			}

			template<typename char_T>
			struct null_string_data_t final:base_string_data_t<char_T>,instance_struct<null_string_data_t<char_T>>{
				typedef null_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				null_string_data_t()noexcept:base_t(never_ref_num_zero){}

				virtual void be_replace_as(ptr_t a)override final{nothing;}
				[[nodiscard]]virtual char_T* get_c_str()override final{
					static char_T data[1]{};
					return data;
				}
				[[nodiscard]]virtual size_t get_size()override final{ return 0; }
				[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)override final{ return this; }
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{ return str; }
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{ return str; }

				//[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str);
				//[[nodiscard]]virtual ptr_t do_insert(size_t pos,ptr_t str);
				[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)override final{ return this; }

				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ return; }
				[[nodiscard]]virtual char_T& arec(size_t index)override final{ return*(char_T*)null_ptr; }
				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{ nothing; }
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

				virtual void be_replace_as(ptr_t a)override final{
					_to.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual ptr_t get_substr_data(size_t begin,size_t size)override final{ return get<substr_string_data_t<char_T>>(_to,begin+_sub_begin,size); }
				[[nodiscard]]virtual char_T* get_c_str()override final{ return _to->get_c_str()+_sub_begin; }
				[[nodiscard]]virtual size_t get_size()override final{ return _sub_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ _to->copy_part_data_to(to,pos+_sub_begin,size); }
				[[nodiscard]]virtual char_T& arec(size_t index)override final{ return _to->arec(index+_sub_begin); }
				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						_to->arec_set(index+_sub_begin,a,_to);
					else
						base_t::arec_set(index,a,p);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique() && _sub_begin==0){
						_to=_to->apply_str_to_begin(str);
						_sub_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					if(this->is_unique() && _sub_begin==0){
						_to=_to->apply_str_to_begin(str);
						_sub_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()){
						_to=_to->apply_str_to_end(str);
						_sub_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()){
						_to=_to->apply_str_to_end(str);
						_sub_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::get_substr_data(size_t begin,size_t size){
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

				virtual void be_replace_as(ptr_t a)override final{
					_m.clear();
					_to.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _used_size+_to_size; }
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
				[[nodiscard]]virtual char_T& arec(size_t index)override final{
					if(index<_to_size)
						return _to->arec(index);
					else
						return _m[index-_to_size];
				}
				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						if(index<_to_size)
							_to->arec_set(index,a,_to);
						else
							_m[index-_to_size]=a; 
					else
						base_t::arec_set(index,a,p);
				}

				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
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
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					this->shrink();
					return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_begin(str);
						_to_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_begin(str);
						_to_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				void shrink(){
					_m.resize(_used_size);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(string_view_t str){
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

				virtual void be_replace_as(ptr_t a)override final{
					_m.clear();
					_to.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _used_size+_to_size; }
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
				[[nodiscard]]virtual char_T& arec(size_t index)override final{
					if(index<_used_size)
						return _m[index];
					else
						return _to->arec(index-_used_size);
				}
				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						if(index<_used_size)
							_m[index]=a;
						else
							_to->arec_set(index-_used_size,a,_to);
					else
						base_t::arec_set(index,a,p);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
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
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					this->shrink();
					return base_t::apply_str_to_begin(str);
				}
				void shrink(){
					_m.forward_resize(_used_size);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_end(str);
						_to_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_end(str);
						_to_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(string_view_t str){
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

				virtual void be_replace_as(ptr_t a)override final{
					_defore.reset();
					_after.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _defore_size+_after_size; }
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
				[[nodiscard]]virtual char_T& arec(size_t index)override final{
					if(index<_defore_size)
						return _defore->arec(index);
					else
						return _after->arec(index-_defore_size);
				}
				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						if(index<_defore_size)
							_defore->arec_set(index,a,_defore);
						else
							_after->arec_set(index-_defore_size,a,_after);
					else
						base_t::arec_set(index,a,p);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique()){
						_defore=_defore->apply_str_to_begin(str);
						_defore_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					if(this->is_unique()){
						_defore=_defore->apply_str_to_begin(str);
						_defore_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique()){
						_after=_after->apply_str_to_end(str);
						_after_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					if(this->is_unique()){
						_after=_after->apply_str_to_end(str);
						_after_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_end(ptr_t str){
				return get<sum_string_data_t<char_T>>(this,str);
			}
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::apply_str_to_begin(ptr_t str){
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

				virtual void be_replace_as(ptr_t a)override final{
					_to.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _to_size-_erase_size; }
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
				[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)override final{
					if(this->is_unique()){
						if(pos<=_erase_pos && pos+size>=_erase_size){
							_erase_pos=pos;
							_erase_size+=size;
							return this;
						}
					}
					return base_t::do_erase(pos,size);
				}
				[[nodiscard]]virtual char_T& arec(size_t index)override final{
					if(index>_erase_pos)
						return _to->arec(index+_erase_size);
					else
						return _to->arec(index);
				}

				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						if(index>_erase_pos)
							return _to->arec_set(index+_erase_size,a,_to);
						else
							return _to->arec_set(index,a,_to);
					else
						base_t::arec_set(index,a,p);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_begin(str);
						auto strsize=str.size();
						_to_size+=strsize;
						_erase_pos+=strsize;
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					if(this->is_unique()){
						_to = _to->apply_str_to_begin(str);
						auto strsize=str->get_size();
						_to_size+=strsize;
						_erase_pos+=strsize;
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_end(str);
						_to_size+=str.size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					if(this->is_unique()){
						_to=_to->apply_str_to_end(str);
						_to_size+=str->get_size();
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size){
				return get<erased_string_data_t<char_T>>(this,pos,size);
			}
		}
		template<typename char_T>
		struct string_t{
			typedef base_string_data_t<char_T> base_t_w;
			typedef base_t_w::ptr_t ptr_t;
			typedef base_t_w::string_view_t string_view_t;
			typedef string_t<char_T> this_t;
		private:
			mutable ptr_t _m;

			string_t(ptr_t str):_m(str){}
			[[nodiscard]]string_t copy()const{ return*this; }
		public:
			void swap_with(this_t& a){
				swap(_m,a._m);
			}

			string_t(string_view_t str):_m(get<comn_string_data_t<char_T>>(str)){}
			string_t(array_end_by_zero_t<const char_T> str):string_t((string_view_t)(str)){}
			string_t(const char_T* str):string_t(array_end_by_zero_t<const char_T>(str)){}
			string_t(const string_t& str)=default;
			string_t(string_t&&str)=default;

			string_t& operator=(const string_t& str)=default;
			string_t& operator=(string_t&&str)=default;

			[[nodiscard]]string_t operator+(const string_t& str)const&{
				return copy()._m->apply_str_to_end(str._m);
			}
			[[nodiscard]]string_t operator+(string_view_t str)const&{
				return copy()._m->apply_str_to_end(str);
			}
			[[nodiscard]]string_t operator+(const char_T* str)const&{
				return *this+array_end_by_zero_t<const char_T>(str);
			}
			friend [[nodiscard]]string_t operator+(string_view_t str1,const string_t& str2){
				return str2.copy()._m->apply_str_to_begin(str1);
			}
			friend [[nodiscard]]string_t operator+(const char_T* str1,const string_t& str2){
				return array_end_by_zero_t<const char_T>(str1)+str2;
			}

			string_t& operator+=(string_t str)&{
				_m=_m->apply_str_to_end(str._m);
				return*this;
			}
			string_t& operator+=(string_view_t str)&{
				_m=_m->apply_str_to_end(str);
				return*this;
			}
			string_t& operator+=(const char_T* str)&{
				return *this+=array_end_by_zero_t<const char_T>(str);
			}
			template<typename U>
			[[nodiscard]]string_t&& operator+(U&& b)&&noexcept_as(*this+=b) requires was_not_an_ill_form(*this+=b){//对右值的operator+优化为operator+=
				return *this+=b;
			}

			[[nodiscard]]constexpr auto operator<=>(const string_t& a)noexcept(compare.nothrow<char_T>){
				return compare(c_str(),size(),a.c_str(),a.size());
			}
			[[nodiscard]]constexpr auto operator==(const string_t& a)noexcept(equal.nothrow<char_T>){
				return equal(c_str(),size(),a.c_str(),a.size());
			}
			[[nodiscard]]constexpr auto operator<=>(string_view_t a)noexcept(compare.nothrow<char_T>){
				return compare(c_str(),size(),a.begin(),a.size());
			}
			[[nodiscard]]constexpr auto operator==(string_view_t a)noexcept(equal.nothrow<char_T>){
				return equal(c_str(),size(),a.begin(),a.size());
			}
			[[nodiscard]]constexpr auto operator<=>(const char_T* a)noexcept(compare.nothrow<char_T>){
				return operator<=>(array_end_by_zero_t<const char_T>(a));
			}
			[[nodiscard]]constexpr auto operator==(const char_T* a)noexcept(equal.nothrow<char_T>){
				return operator==(array_end_by_zero_t<const char_T>(a));
			}

		private:
			char_T* unique_c_str(){ return _m->get_unique_c_str(_m); }
			class arec_t{
				string_t* _to;
				size_t _index;
			public:
				arec_t(string_t*to,size_t index):_to(_to),_index(index){}
				[[nodiscard]]operator char_T()const noexcept{ return _to->_m->arec(_index); }
				arec_t&operator=(char_T a)noexcept{ _to->_m->arec_set(_index,a,_to->_m); return*this; }
				const arec_t&operator=(char_T a)const noexcept{ _to->_m->arec_set(_index,a,_to->_m); return*this; }
				[[nodiscard]]char_T*operator&()noexcept{ return _to->unique_c_str()+_index; }
				[[nodiscard]]const char_T*operator&()const noexcept{ return _to->c_str()+_index; }
			};
		public:
			[[nodiscard]]arec_t operator[](size_t index){ return{this,index}; }
			[[nodiscard]]const arec_t operator[](size_t index)const{ return{this,index}; }
			
			[[nodiscard]]string_t substr(size_t begin,size_t size){ return _m->get_substr_data(begin,size); }
			[[nodiscard]]char_T* c_str(){ return unique_c_str(); }
			[[nodiscard]]const char_T* c_str()const{ return _m->get_c_str(); }
			[[nodiscard]]size_t size(){ return _m->get_size(); }
			void clear(){
				_m=null_ptr;
			}
		};
		template<typename T>
		inline void swap(string_t<T>& a,string_t<T>& b)noexcept{ a.swap_with(b); }

		template<class T>
		[[nodiscard]]inline auto size_of_array_like(string_t<T>& a)noexcept{ return a.size(); }
		template<class T>
		[[nodiscard]]inline auto begin_of_array_like(string_t<T>& a)noexcept{ return(T*)a.c_str(); }
	}
}
namespace elc{
	using defs::string_n::string_t;
}
#include "../../files/_files/_share/_undefs.hpp"

void ste::stst()
{
	{
		elc::string_t<char> a="";
		stest_accert(a.size()==0);
		a="asd";
		stest_accert(a.size()==3);
		a="asd"+a;
		stest_accert(a.size()==6);
		a+="asd";
		stest_accert("asd"+a=="asdasdasdasd");
		a[1]='e';
		stest_accert(a.substr(0,3)=="aed");
		stest_accert(a[2]=='d');
		a.clear();
		stest_accert(a.size()==0);
	}
	elc::defs::memory::check_memory_lack();
}
