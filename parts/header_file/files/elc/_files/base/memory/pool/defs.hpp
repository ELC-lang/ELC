//defs.hpp
//at namespace elc::defs::memory
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
//copy from old ELC & adepted.
namespace pool_n{
	//pool：通过提前分配大量空间（即ment类）以节省空间（减少overhead与blocklistptr数量）并加快内存分配速度（unuse_indexes快速索引）
	//适用于大量实例且频繁生成的类（如elc中的node）
	//缺点：提前分配空间可能造成空间浪费
	//		无法使用pool进行数组get/数组elc::alloc以生成经典数组（因为unget并不传回数组大小、也不传回分配时是否分配为数组（真糟糕！））
	//使用方法：类定义中加入alloc_by_pool的attribute即可
	//			重载get_ment_size函数模板以自定义ment大小，默认2048
	//注意：alloc_by_pool的类**可以不**通过get/elc::memory::alloc进行(分配&构造)/纯分配，通过unget/elc::memory::free进行(析构&解分配)/纯解分配，但这意味着此部分实例不使用pool进行内存管理
	//		pool接入get模块的原理是重载此类所用的elc::memory::base_alloc方法，这意味着当你直接（或间接）使用**任何**elc提供的数组性质的内存管理机制，此部分实例仍然会不使用pool进行内存管理。同时，这将引起一个警告：我们认为alloc_by_pool的类会被大量使用，使用这种类的传统数组意味着大量的内存碎片与缓慢的内存分配。为了关闭此警告，你可以将pool_s_array_warning函数模板重载为0，亦或使用非elc提供的内存管理机制：如std::malloc、std::new等。当然，这意味着当这些机制遇见内存用尽的情况时，你可能需要手动调用elc::memory::gc。
	//		get/unget模块定义见 "../get/defs.hpp"
	//		（elc的）alloc/free模块定义见 "../alloc/defs.hpp"
	template<class T,::std::uint_fast64_t ment_size>
	struct ment final:cons_t<ment<T,ment_size>>,non_copyable,non_moveable{
		typedef ::std::uint_fast64_t uint;
		typedef cons_t<ment<T,ment_size>> cons;
		static_assert(ment_size!=0,"are you crazy?");
		static_assert(ment_size!=1,"SB");//嘿，或许这个类不需要使用pool :)
		static constexpr uint ment_index_max=ment_size-1;

		uint _unuse_indexes_index;
		uint _unuse_indexes[ment_size];
		data_block<T>_data[ment_size];//无需考虑对齐问题：data_block自动根据类型参数提出对齐要求，它定义于 "../../base_defs/data.hpp"

		constexpr ment()noexcept{
			for(uint a=0;a!=ment_size;a++)_unuse_indexes[a]=a;
			_unuse_indexes_index=ment_size;
		}
		[[nodiscard]]T*get_new()noexcept{
			auto&uii=_unuse_indexes_index;//缩写
			auto&ui=_unuse_indexes;//缩写*2
			if(!uii)return nullptr;//请勿改为null_ptr:在FLAG1中，使用了值转bool的判断，而null_ptr可能被重载
			return&data_cast<T>(_data[ui[--uii]]);
		}
		[[nodiscard]]bool in_pool(T*a)noexcept{
			return in_range(a,{(byte*)_data,note::size(ment_index_max)});
		}
		[[nodiscard]]bool use_end(T*a)noexcept{
			if(in_pool(a)){
				_unuse_indexes[_unuse_indexes_index++]=a-reinterpret_cast<T*>(_data);
				return true;
			}
			return false;
		}
		[[nodiscard]]bool empty()const noexcept{return _unuse_indexes_index==ment_size;}
	};
	template<class T,::std::uint_fast64_t ment_size>
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
				unget((ment*)i++);
		}
		[[nodiscard]]T*get_new()noexcept{
			{
				T*tmp;
				auto i=head(),e=end();
				while(--e!=i){//从后向前遍历，一般情况下可以加快分配速度😎
					tmp=e->get_new();
					if(tmp)return tmp;//FLAG1
				}
			}
			auto tmp=get<ment>();//失败，加入新单元
			list::add(tmp);
			return tmp->get_new();
		}
		bool use_end(T*a)noexcept{
			auto i=head(),e=end();
			while(++i!=e)
				if(i->use_end(a))return 1;
			return 0;
		}
		[[nodiscard]]bool in_pool(T*a)noexcept{
			auto i=head(),e=end();
			while(++i!=e)
				if(i->in_pool(a))return true;
			return false;
		}
		bool shrink()noexcept{
			bool shrink_success=false;
			auto i=begin(),e=end();
			decltype(i)n;
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
	constexpr ::std::uint_fast64_t get_ment_size(type_info_t<T>)noexcept{
		#if defined(_MSC_VER)
			return (::std::uint_fast64_t)min(size_t(2048),size_t(0x7fffffff)/sizeof(T));
		#else
			return 2048;
		#endif
	}
	template<typename T>
	constexpr bool pool_s_array_warning(type_info_t<T>)noexcept{return 1;}
	template<typename T>
	struct alloc_by_pool{};
	template<typename T>
	distinctive inline pool_t<T,get_ment_size(type_info<T>)>pool{};
	//为alloc提供方法
	template<typename T>
	[[nodiscard]]inline void*the_alloc_method(type_info_t<T>)noexcept{
		return pool<T>.get_new();
	}
	template<typename T>
	[[nodiscard]]inline void*the_alloc_method(type_info_t<T>,size_t a)noexcept{
		if constexpr(pool_s_array_warning(type_info<T>))
			template_warning("pool can\'t alloc array.");
		return memory::alloc_n::default_method::alloc_method(type_info<T>,a);
	}
	template<typename T>
	[[nodiscard]]size_t the_get_size_of_alloc_method(T*arg){
		return pool<T>.in_pool(arg)?1:memory::alloc_n::default_method::get_size_of_alloc_method(arg);
	}
	template<typename T>
	inline void the_free_method(T*arg)noexcept{
		if(!pool<T>.use_end(arg))
			memory::alloc_n::default_method::free_method(arg);
	}
	template<typename T>
	inline void*the_realloc_method(T*ptr,size_t new_size)noexcept{
		if constexpr(pool_s_array_warning(type_info<T>))
			template_warning("pool can\'t alloc array.");
		return memory::alloc_n::default_method::realloc_method(ptr,new_size);
	}
}

//file_end

