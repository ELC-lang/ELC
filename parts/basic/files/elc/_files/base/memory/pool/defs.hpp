//defs.hpp
//at namespace elc::defs::memory
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
//copy from old ELC & adepted.
namespace pool_n{//通过提前分配大量空间以节省空间（减少overhead数量）并加快分配速度
	template<class T,::std::uint_fast16_t ment_size>
	struct ment final:cons_t<ment<T,ment_size>>{
		typedef ::std::uint_fast16_t uint;
		typedef cons_t<ment<T,ment_size>> cons;
		static_assert(ment_size,"are you crazy?");
		static constexpr uint ment_index_max=ment_size-1;

		uint _unuse_indexes_index;
		uint _unuse_indexes[ment_size];
		data_block<T>_data[ment_size];

		constexpr ment(){
			for(uint a=0;a!=ment_size;a++)unuse_indexes[a]=a;
			_unuse_indexes_index=ment_size;
		}
		[[nodiscard]]T*get_new()noexcept{
			auto&uii=_unuse_indexes_index;
			auto&ui=_unuse_indexes;
			if(!uii)return nullptr;
			return&data_cast<T>(_data[ui[--uii]]);
		}
		[[nodiscard]]bool use_end(T*a)noexcept{
			if(in_range(a,{_data,note::size(ment_index_max)})){
				_unuse_indexes[_unuse_indexes_index++]=a-reinterpret_cast<T*>(_data);
				return true;
			}
			return false;
		}
		[[nodiscard]]bool empty()noexcept{return _unuse_indexes_index==ment_size;}
	};
	template<class T,::std::uint_fast16_t ment_size>
	class pool_t final:list_t<ment<T,ment_size>>,non_copyable,non_moveable{
		typedef ment<T,ment_size> ment;
		typedef list_t<ment> list;
		using list::begin;
		using list::head;
		using list::end;
	public:
		pool_t()noexcept=default;
		~pool_t()noexcept{
			auto i=begin(),e=end();
			while(i!=e)
				unget(i++);
		}
		[[nodiscard]]T*get_new()noexcept{
			{
				T*tmp;
				auto i=head(),e=end();
				while(--e!=i){//从后向前遍历，一般情况下可以加快分配速度😎
					tmp=e->get_new();
					if(tmp)return tmp;
				}
			}
			auto tmp=get<ment>();//失败，加入新单元
			list::add(tmp);
			return tmp->get_new();
		}
		void use_end(T*a)noexcept{
			auto i=head(),e=end();
			while(++i!=e)
				if(i->use_end(a))return;
		}
		bool shrink()noexcept{
			bool shrink_success=false;
			auto i=begin(),e=end(),n;
			while((n=i++)!=e){
				if(n->empty()){
					unget(n);
					shrink_success=true;
				}
			}
			return shrink_success;
		}
		[[nodiscard]]bool empty()noexcept{
			auto i=head(),e=end();
			while(++i!=e)
				if(!i->empty())return false;
			return true;
		}
	};
	template<typename T>
	constexpr ::std::uint_fast16_t get_ment_size(type_arg_t<T>){return 2048;}
	template<typename T>
	struct alloc_by_pool{
		constexpr static ::std::uint_fast16_t pool_ment_size=get_ment_size(type_arg<T>);
	};
	template<typename T>
	inline pool_t<T,alloc_by_pool<T>::pool_ment_size>pool{};
	//为alloc提供方法
	template<typename T>
	inline void*alloc_method(type_arg_t<T>)noexcept{
		return pool<T>.get_new();
	}
	template<typename T>
	inline void*alloc_method(type_arg_t<T>,size_t)noexcept{
		template_error("pool can\'t alloc array.");
	}
	template<typename T>
	constexpr size_t get_size_of_alloc_method(T*arg){
		return 1;
	}
	template<typename T>
	inline void free_method(T*arg)noexcept{
		pool<T>.use_end(arg);
	}
	template<typename T>
	inline void*realloc_method(T*&ptr,size_t new_size)noexcept{
		template_error("pool can\'t alloc array.");
	}
	//
}
