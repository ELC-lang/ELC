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

#define STEVE_H_TEST
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
				base_string_data_t(never_ref_num_zero_t)noexcept{ attribute_ptr_cast<ref_able>(this)->init_never_ref_num_zero(); }

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

				[[nodiscard]]virtual ptr_t do_insert(size_t pos,string_view_t str);
				[[nodiscard]]virtual ptr_t do_insert(size_t pos,ptr_t str);
				[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size);

				[[nodiscard]]virtual ptr_t do_pop_back(size_t size,ptr_t& self){
					auto pos	= this->get_size() - size;
					auto defore = get_substr_data(0, pos);
					auto after	= get_substr_data(pos, size);
					self		= defore;
					return after;
				}
				[[nodiscard]]virtual ptr_t do_pop_front(size_t size,ptr_t& self){
					auto pos	= size;
					auto defore = get_substr_data(0, pos);
					auto after	= get_substr_data(pos, this->get_size() - size);
					self		= after;
					return defore;
				}
				/*
				TODO:

				find
				rfind
				find_first_of
				find_first_not_of
				find_last_of
				find_last_not_of

				compare
				starts_with
				ends_with
				contains
				replace
				*/

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
				comn_string_data_t(ptr_t str,size_t pos,size_t size):_m(note::size(size+1)){
					str->copy_part_data_to((char_T*)_m,pos,size);
				}

				virtual void be_replace_as(ptr_t a)override final{
					_m.clear();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual char_T* get_c_str()override final{ return (char_T*)_m; }
				[[nodiscard]]virtual char_T* get_unique_c_str(ptr_t&a)override final{
					if(this->is_unique())
						return (char_T*)_m;
					else
						return base_t::get_unique_c_str(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _m.size()-1; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{ copy_assign[size](note::form((const char_T*)_m+pos),note::to(to)); }
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
				[[nodiscard]]virtual ptr_t get_substr_data([[maybe_unused]]size_t begin,[[maybe_unused]]size_t size)override final{ return this; }
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{ return str; }
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{ return str; }

				[[nodiscard]]virtual ptr_t do_insert([[maybe_unused]]size_t pos,[[maybe_unused]]string_view_t str)override final{ return get<comn_string_data_t<char_T>>(str); }
				[[nodiscard]]virtual ptr_t do_insert([[maybe_unused]]size_t pos,[[maybe_unused]]ptr_t str)override final{ return str; }
				[[nodiscard]]virtual ptr_t do_erase([[maybe_unused]]size_t pos,[[maybe_unused]]size_t size)override final{ return this; }

				virtual void copy_part_data_to([[maybe_unused]]char_T* to,[[maybe_unused]]size_t pos,[[maybe_unused]]size_t size)override final{ return; }
				[[nodiscard]]virtual char_T& arec([[maybe_unused]]size_t index)override final{ return*(char_T*)null_ptr; }
				virtual void arec_set([[maybe_unused]]size_t index,[[maybe_unused]]char_T a,[[maybe_unused]]ptr_t& p)override final{ nothing; }
				[[nodiscard]]virtual ptr_t do_pop_back([[maybe_unused]]size_t size,[[maybe_unused]]ptr_t& self)override final{ return this; }
				[[nodiscard]]virtual ptr_t do_pop_front([[maybe_unused]]size_t size,[[maybe_unused]]ptr_t& self)override final{ return this; }
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
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _sub_begin==0) {
						auto aret = _to->do_pop_front(size,_to);
						_sub_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _sub_begin+_sub_size==_to->get_size()) {
						auto aret = _to->do_pop_back(size,_to);
						_sub_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_back(size,self);
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
					_to(str)
				{
					_m.resize(size_t((_to_size+_used_size)*magic_number::gold_of_resize));
					copy_assign[_used_size](note::form(end.begin()),note::to((char_T*)_m));
				}
				end_apply_string_data_t(ptr_t str,size_t count,char_T ch):
					_to_size(str->get_size()),
					_used_size(count),
					_to(str)
				{
					_m.resize(size_t((_to_size+_used_size)*magic_number::gold_of_resize));
					copy_assign[_used_size](ch,note::to((char_T*)_m));
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
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique()) {
						auto aret = _to->do_pop_front(size, _to);
						_to_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _used_size>=size) {
						_used_size -= size;
						return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m + _used_size,size});
					}
					else
						return base_t::do_pop_back(size, self);
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
					_to(str)
				{
					_m.resize(size_t((_to_size+_used_size)*magic_number::gold));
					copy_assign[_used_size](note::form((const char_T*)head.begin()),note::to((char_T*)_m.end()-_used_size));
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
						_used_size+=str.size();
						copy_assign[str.size()](note::form((const char_T*)str.begin()),note::to((char_T*)_m.end()-_used_size));
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
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _used_size>=size) {
						_used_size -= size;
						return get<comn_string_data_t<char_T>>(string_view_t{(char_T*)_m.end() - _used_size - size, size});
					}
					else
						return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique()) {
						auto aret = _to->do_pop_back(size, _to);
						_to_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_back(size, self);
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

				ptr_t  _defore;
				ptr_t  _after;
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
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _defore_size>=size) {
						auto aret = _defore->do_pop_front(size, _defore);
						_defore_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _after_size>=size) {
						auto aret = _after->do_pop_back(size, _after);
						_after_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_back(size, self);
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

				ptr_t  _to;
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
						if(pos<=_erase_pos && pos+size>=_erase_pos+_erase_size){
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
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _erase_pos > size){
						auto aret = _to->do_pop_front(size, _to);
						_to_size -= size;
						_erase_pos -= size;
						return aret;
					}
					else
						return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique() && _erase_pos+_erase_size <= _to_size-size) {
						auto aret = _to->do_pop_back(size, _to);
						_to_size -= size;
						return aret;
					}
					else
						return base_t::do_pop_back(size, self);
				}
			};
			template<typename char_T>
			[[nodiscard]]base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_erase(size_t pos,size_t size){
				return get<erased_string_data_t<char_T>>(this,pos,size);
			}

			template<typename char_T>
			struct inserted_string_data_t final: base_string_data_t<char_T>, instance_struct<inserted_string_data_t<char_T>> {
				typedef inserted_string_data_t<char_T> this_t;
				typedef base_string_data_t<char_T> base_t;
				using base_t::ptr_t;
				using base_t::string_view_t;

				ptr_t  _to;
				ptr_t  _insert_data;
				size_t _to_size;
				size_t _insert_pos;
				size_t _insert_size;

				inserted_string_data_t(ptr_t to, ptr_t insert_data, size_t insert_pos):_to(to),_insert_data(insert_data),_insert_pos(insert_pos),_to_size(to->get_size()),_insert_size(insert_data->get_size()) {}

				virtual void be_replace_as(ptr_t a)override final{
					_to.reset();
					_insert_data.reset();
					base_t::be_replace_as(a);
				}
				[[nodiscard]]virtual size_t get_size()override final{ return _to_size+_insert_size; }
				virtual void copy_part_data_to(char_T* to,size_t pos,size_t size)override final{
					if(pos+size<_insert_pos)
						_to->copy_part_data_to(to,pos,size);
					elseif(pos>_insert_pos+_insert_size)
						_to->copy_part_data_to(to,pos-_insert_size,size);
					else{
						if(_insert_pos>pos){
							auto size_defore_insert_pos=_insert_pos-pos;
							_to->copy_part_data_to(to,pos,size_defore_insert_pos);
							to+=size_defore_insert_pos;
							size-=size_defore_insert_pos;
						}
						auto size_defore_insert_end=min(size,_insert_size);
						_insert_data->copy_part_data_to(to,0,size_defore_insert_end);
						to+=size_defore_insert_end;
						size-=size_defore_insert_end;
						if(size)
							_to->copy_part_data_to(to,_insert_pos,size);
					}
				}
				[[nodiscard]]virtual ptr_t do_erase(size_t pos,size_t size)override final{
					if(this->is_unique()){
						if(pos>=_insert_pos && pos+size<=_insert_pos+_insert_size){
							_insert_data=_insert_data->do_erase(pos-_insert_pos,size);
							_insert_size-=size;
							return this;
						}
						elseif(pos+size<_insert_pos){
							_to=_to->do_erase(pos,size);
						}
						elseif(pos>_insert_pos+_insert_size) {
							_to=_to->do_erase(pos-_insert_size,size);
						}
					}
					return base_t::do_erase(pos,size);
				}
				[[nodiscard]]virtual char_T& arec(size_t index)override final{
					if(index>=_insert_pos && index<_insert_pos+_insert_size)
						return _insert_data->arec(index-_insert_pos);
					elseif(index>=_insert_pos+_insert_size)
						return _to->arec(index-_insert_size);
					else
						return _to->arec(index);
				}

				virtual void arec_set(size_t index,char_T a,ptr_t& p)override final{
					if(this->is_unique())
						if(index>=_insert_pos && index<_insert_pos+_insert_size)
							_insert_data->arec_set(index-_insert_pos,a,p);
						elseif(index>=_insert_pos+_insert_size)
							_to->arec_set(index-_insert_size,a,p);
						else
							_to->arec_set(index,a,p);
					else
						base_t::arec_set(index,a,p);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(string_view_t str)override final{
					if(this->is_unique()){
						if(_insert_pos==0){
							_insert_data=_insert_data->apply_str_to_begin(str);
							_insert_size+=str.size();
						}
						else{
							_to=_to->apply_str_to_begin(str);
							_to_size+=str.size();
						}
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_begin(ptr_t str)override final{
					if(this->is_unique()){
						if(_insert_pos==0){
							_insert_data=_insert_data->apply_str_to_begin(str);
							_insert_size+=str->get_size();
						}
						else{
							_to=_to->apply_str_to_begin(str);
							_to_size+=str->get_size();
						}
						return this;
					}
					else
						return base_t::apply_str_to_begin(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(string_view_t str)override final{
					if(this->is_unique()){
						if(_insert_pos==_to_size){
							_insert_data=_insert_data->apply_str_to_end(str);
							_insert_size+=str.size();
						}
						else{
							_to=_to->apply_str_to_end(str);
							_to_size+=str.size();
						}
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]]virtual ptr_t apply_str_to_end(ptr_t str)override final{
					if(this->is_unique()){
						if(_insert_pos==_to_size){
							_insert_data=_insert_data->apply_str_to_end(str);
							_insert_size+=str->get_size();
						}
						else{
							_to=_to->apply_str_to_end(str);
							_to_size+=str->get_size();
						}
						return this;
					}
					else
						return base_t::apply_str_to_end(str);
				}
				[[nodiscard]] virtual ptr_t do_pop_front(size_t size, ptr_t& self) override final {
					if(this->is_unique()) {
						if(_insert_pos > size) {
							auto aret = _to->do_pop_front(size, _to);
							_to_size -= size;
							_insert_pos -= size;
							return aret;
						}
						elseif(_insert_pos==0 && _insert_size>=size){
							auto aret = _insert_data->do_pop_front(size, _insert_data);
							_insert_size -= size;
						}
					}
					return base_t::do_pop_front(size, self);
				}
				[[nodiscard]] virtual ptr_t do_pop_back(size_t size, ptr_t& self) override final {
					if(this->is_unique()) {
						if(_insert_pos+_insert_size <= _to_size-size) {
							auto aret = _to->do_pop_back(size, _to);
							_to_size -= size;
							return aret;
						}
						elseif(_insert_pos==_to_size && _insert_size>=size) {
							auto aret = _insert_data->do_pop_front(size, _insert_data);
							_insert_size -= size;
						}
					}
					return base_t::do_pop_back(size, self);
				}
			};
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos, ptr_t str) {
				return get<inserted_string_data_t<char_T>>(this,str,pos);
			}
			template<typename char_T>
			base_string_data_t<char_T>::ptr_t base_string_data_t<char_T>::do_insert(size_t pos, string_view_t str) {
				return this->do_insert(pos,get<comn_string_data_t<char_T>>(str));
			}
		}
		template<typename char_T>
		struct string_t{
			typedef base_string_data_t<char_T> base_t_w;
			typedef base_t_w::ptr_t			   ptr_t;
			typedef base_t_w::string_view_t	   string_view_t;
			typedef string_t<char_T>		   this_t;
			static constexpr size_t			   npos = size_t(-1);

		private:
			mutable ptr_t _m;

			string_t(ptr_t str):_m(str){}
			[[nodiscard]]string_t copy()const{ return*this; }
		public:
			void swap_with(this_t& a)noexcept{
				swap(_m,a._m);
			}

			string_t(string_view_t str):_m(get<comn_string_data_t<char_T>>(str)) {}
			string_t(array_end_by_zero_t<const char_T> str):string_t((string_view_t)(str)) {}
			string_t(const char_T* str):string_t(array_end_by_zero_t<const char_T>(str)) {}
			string_t(const string_t& str) = default;
			string_t(string_t&& str)	  = default;

			string_t& operator=(const string_t& str) = default;
			string_t& operator=(string_t&& str) = default;

			[[nodiscard]] string_t operator+(const string_t& str) const& {
				return copy()._m->apply_str_to_end(str._m);
			}
			[[nodiscard]] string_t operator+(string_view_t str) const& {
				return copy()._m->apply_str_to_end(str);
			}
			[[nodiscard]] string_t operator+(const char_T* str) const& {
				return *this + array_end_by_zero_t<const char_T>(str);
			}
			friend [[nodiscard]] string_t operator+(string_view_t str1, const string_t& str2) {
				return str2.copy()._m->apply_str_to_begin(str1);
			}
			friend [[nodiscard]] string_t operator+(const char_T* str1, const string_t& str2) {
				return array_end_by_zero_t<const char_T>(str1) + str2;
			}

			string_t& operator+=(string_t str) & {
				_m = _m->apply_str_to_end(str._m);
				return *this;
			}
			string_t& operator+=(string_view_t str) & {
				_m = _m->apply_str_to_end(str);
				return *this;
			}
			string_t& operator+=(const char_T* str) & {
				return *this += array_end_by_zero_t<const char_T>(str);
			}
			template<typename U>
			[[nodiscard]] string_t&& operator+(U&& b) && noexcept_as(*this += b) requires was_not_an_ill_form(*this += b) {		  //对右值的operator+优化为operator+=
				return *this += b;
			}

			[[nodiscard]] constexpr auto operator<=>(const string_t& a) const noexcept(compare.nothrow<char_T>) {
				return compare(c_str(), size(), a.c_str(), a.size());
			}
			[[nodiscard]] constexpr auto operator==(const string_t& a) const noexcept(equal.nothrow<char_T>) {
				return equal(c_str(), size(), a.c_str(), a.size());
			}
			[[nodiscard]] constexpr auto operator<=>(string_view_t a) const noexcept(compare.nothrow<char_T>) {
				return compare(c_str(), size(), a.begin(), a.size());
			}
			[[nodiscard]] constexpr auto operator==(string_view_t a) const noexcept(equal.nothrow<char_T>) {
				return equal(c_str(), size(), a.begin(), a.size());
			}
			[[nodiscard]] constexpr auto operator<=>(const char_T* a) const noexcept(compare.nothrow<char_T>) {
				return operator<=>(array_end_by_zero_t<const char_T>(a));
			}
			[[nodiscard]] constexpr auto operator==(const char_T* a) const noexcept(equal.nothrow<char_T>) {
				return operator==(array_end_by_zero_t<const char_T>(a));
			}

		private:
			char_T* unique_c_str() { return _m->get_unique_c_str(_m); }
			char_T& arec(size_t index) { return _m->arec(index); }
			void	arec_set(size_t index, char_T a) { return _m->arec_set(index, a, _m); }

			class arec_t: non_copyable, non_moveable {
				string_t* _to;
				size_t	  _index;

				friend class string_t;
				arec_t(string_t* to, size_t index):_to(to),_index(index) {}

			public:
				[[nodiscard]] operator char_T() const noexcept { return _to->arec(_index); }
				arec_t&		  operator=(char_T a) noexcept {
					_to->arec_set(_index, a);
					return *this;
				}
				[[nodiscard]] char_T*		operator&() noexcept { return _to->unique_c_str() + _index; }
				[[nodiscard]] const char_T* operator&() const noexcept { return ((const string_t*)(_to))->c_str() + _index; }
				[[nodiscard]] operator char_T&() noexcept { return *operator&(); }
				[[nodiscard]] operator const char_T&() const noexcept { return *operator&(); }
			};

		public:
			[[nodiscard]] arec_t	   operator[](size_t index) { return {this, index}; }
			[[nodiscard]] const arec_t operator[](size_t index) const { return {(string_t*)this, index}; }

			[[nodiscard]] string_t substr(size_t begin, size_t size = npos) const {
				size = min(size, this->size() - begin);
				return _m->get_substr_data(begin, size);
			}
			[[nodiscard]] char_T*		c_str() { return this->unique_c_str(); }
			[[nodiscard]] const char_T* c_str() const { return _m->get_c_str(); }
			[[nodiscard]] size_t		size() const { return _m->get_size(); }
			void resize(size_t count, char_T ch = {}) {
				auto size = this->size();
				if(size > count)
					*this = substr(0, count);
				elseif(size == count)
					return;
				else
					_m = get<end_apply_string_data_t<char_T>>(_m, count - size, ch);
			}
			void clear() { _m = null_ptr; }
		private:
			struct iterator_base_t {
				string_t* _to;
				ptrdiff_t _index;		//rend: -1，使用size_t也不是不可以（标准允许无符号整数溢出），但是operator<=>会出问题

				[[nodiscard]] constexpr iterator_base_t get_before() const noexcept { return {_to, _index - 1}; }
				[[nodiscard]] constexpr iterator_base_t get_next() const noexcept { return {_to, _index + 1}; }
				[[nodiscard]] arec_t					get_value() noexcept { return (*_to)[_index]; }
				[[nodiscard]] const arec_t				get_value() const noexcept { return (*(const string_t*)_to)[_index]; }
				[[nodiscard]] char_T*					get_handle() noexcept { return &get_value(); }
				[[nodiscard]] const char_T*				get_handle() const noexcept { return &get_value(); }
				constexpr bool							operator==(const iterator_base_t& a) const noexcept { return _to == a._to && _index == a._index; }
				constexpr auto							operator<=>(const iterator_base_t& a) const noexcept { return _to == a._to ? _index <=> a._index : NAN <=> NAN; }
			};
			[[nodiscard]] iterator_base_t get_iterator_data_at(ptrdiff_t index) const { return iterator_base_t{(string_t*)this, index}; }
		public:
			typedef iterator_t<char_T, iterator_base_t>				iterator;
			typedef const_iterator_t<char_T, const iterator_base_t> const_iterator;

			[[nodiscard]] iterator		 get_iterator_at(ptrdiff_t index) { return get_iterator_data_at(index); }
			[[nodiscard]] const_iterator get_iterator_at(ptrdiff_t index) const { return get_iterator_data_at(index); }
			[[nodiscard]] iterator		 begin() { return get_iterator_at(0); }
			[[nodiscard]] const_iterator begin() const { return get_iterator_at(0); }
			[[nodiscard]] const_iterator cbegin() const { return begin(); }
			[[nodiscard]] iterator		 end() { return get_iterator_at(size()); }
			[[nodiscard]] const_iterator end() const { return get_iterator_at(size()); }
			[[nodiscard]] const_iterator cend() const { return end(); }

			typedef reverse_iterator_t<char_T, iterator_base_t>				reverse_iterator;
			typedef reverse_const_iterator_t<char_T, const iterator_base_t> reverse_const_iterator;

			[[nodiscard]] reverse_iterator		 get_reverse_iterator_at(ptrdiff_t index) { return get_iterator_data_at(index); }
			[[nodiscard]] reverse_const_iterator get_reverse_iterator_at(ptrdiff_t index) const { return get_iterator_data_at(index); }
			[[nodiscard]] reverse_iterator		 rbegin() { return get_reverse_iterator_at(size() - 1); }
			[[nodiscard]] reverse_const_iterator rbegin() const { return get_reverse_iterator_at(size() - 1); }
			[[nodiscard]] reverse_const_iterator rcbegin() const { return rbegin(); }
			[[nodiscard]] reverse_iterator		 rend() { return get_reverse_iterator_at(-1); }
			[[nodiscard]] reverse_const_iterator rend() const { return get_reverse_iterator_at(-1); }
			[[nodiscard]] reverse_const_iterator rcend() const { return rend(); }

			//

			void push_back(const string_t& str) { _m = _m->apply_str_to_end(str._m); }
			void push_back(string_view_t str) { _m = _m->apply_str_to_end(str); }
			void push_back(char_T ch) { push_back(string_view_t{&ch, 1}); }
			void push_back(const char_T* str) { push_back(array_end_by_zero_t<const char_T>(str)); }

			void push_front(const string_t& str) { _m = _m->apply_str_to_begin(str._m); }
			void push_front(string_view_t str) { _m = _m->apply_str_to_begin(str); }
			void push_front(char_T ch) { push_front(string_view_t{&ch, 1}); }
			void push_front(const char_T* str) { push_front(array_end_by_zero_t<const char_T>(str)); }

			
			string_t pop_back(size_t size) {
				return _m->do_pop_back(size, _m);
			}
			string_t pop_front(size_t size) {
				return _m->do_pop_front(size, _m);
			}
			char_T pop_back() {
				return pop_back(1)[0];
			}
			char_T pop_front() {
				return pop_front(1)[0];
			}
			/*
			TODO:

			find
			rfind
			find_first_of
			find_first_not_of
			find_last_of
			find_last_not_of
			[[nodiscard]] string_t operator[](size_t index, char_T delimiter) const {
				auto pos = find(delimiter);
				auto end = find(delimiter,pos);
				while(index--) {
					pos = end;
					end = find(delimiter, pos);
				}
				return substr(pos, end - pos);
			}
			[[nodiscard]] string_t operator[](size_t index, string_t delimiter_str) const {
				auto pos = find(delimiter_str);
				auto end = find(delimiter_str, pos);
				while(index--) {
					pos = end;
					end = find(delimiter_str, pos);
				}
				return substr(pos, end - pos);
			}
			*/
			void erase(size_t pos,size_t size=1){
				_m = _m->do_erase(pos, size);
			}
			void insert(size_t pos,string_t str) {
				_m = _m->do_insert(pos, str);
			}
			void insert(size_t pos,string_view_t str) {
				_m = _m->do_insert(pos, str);
			}
			void insert(size_t pos,const char_T* str) {
				insert(pos,array_end_by_zero_t<const char_T>(str));
			}
			void insert(size_t pos,char_T ch) {
				insert(pos,string_view_t{&ch,1});
			}
			/*
			compare
			starts_with
			ends_with
			contains
			replace
			*/
		};
		template<typename T>
		inline void swap(string_t<T>& a,string_t<T>& b)noexcept{ a.swap_with(b); }

		template<class T>
		[[nodiscard]]inline auto size_of_array_like(const string_t<T>& a)noexcept{ return a.size(); }
		template<class T>
		[[nodiscard]]inline auto begin_of_array_like(string_t<T>& a)noexcept{ return(T*)a.c_str(); }
		template<class T>
		[[nodiscard]]inline auto begin_of_array_like(const string_t<remove_cv<T>>& a)noexcept{ return(const T*)a.c_str(); }
	}
}
namespace elc{
	using defs::string_n::string_t;
}
#include "../../files/_files/_share/_undefs.hpp"

void ste::stst()
{
	{
		elc::string_t a=L"";
		stest_accert(a.size()==0);
		a=L"asd";
		stest_accert(a.size()==3);
		a=L"asd"+a;
		stest_accert(a.size()==6);
		a+=L"asd";
		stest_accert(L"asd"+a==L"asdasdasdasd");
		a[1]=L'e';
		stest_accert(a.substr(0,3)==L"aed");
		stest_accert(a.substr(3).size()==6);
		stest_accert(a[2]==L'd');
		a.clear();
		stest_accert(a.size()==0);
		a.resize(3,L'd');
		stest_accert(a==L"ddd");
		for(const wchar_t&c:a)
			stest_accert(c==L'd');
		a=L"abc";
		stest_accert(a.begin()==a.cbegin());
		stest_accert(*a.begin()==L'a');
		stest_accert(a.begin()<=a.cend());
		stest_accert(*a.rbegin()==L'c');
		stest_accert(a.rbegin()<=a.rend());
		stest_accert(a.rbegin()==a.end()-1);
		a.push_back(L"as");
		a.push_front(L'p');
		stest_accert(a == L"pabcas");
		a.erase(1, 3);
		a.erase(1);
		stest_accert(a == L"ps");
		stest_accert(a.pop_front() == L'p');
		stest_accert(a.pop_back() == L's');
		stest_accert(a.size() == 0);
		a = L"abc";
		stest_accert(a.pop_back(2) == L"bc");
		stest_accert(a.size() == 1);
		a.push_front(L"wqer");
		stest_accert(a==L"wqera");
		a.insert(2, L"123");
		a.insert(0, L"123");
		a.insert(a.size(), L"123");
		stest_accert(a[2] == L'3');
		a[6] = L'6';
		a[7] = L'6';
		a[8] = L'6';
		stest_accert(a == L"123wq1666ra123");
		for(wchar_t& c: a)
			c ='7';
		stest_accert(a == L"77777777777777");
	}
	elc::defs::memory::check_memory_lack();
	stest_puts(L"测试完了");
}
