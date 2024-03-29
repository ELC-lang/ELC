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
namespace abstract_base_n{
	/*
	ELC的虚基类模块，适用于unget
	处理虚基类到实例类的偏移与模板实例化差异
	原理：
		虚函数间接调用以从base_t&转为T&
		运行时计算偏移以从T*转换base_t*
	用法：is_common_attribute
	*/
	common_attribute_t abstract_base;
	common_attribute_t instance_struct;
}
namespace get_n{
	using abstract_base_n::abstract_base;
	using abstract_base_n::instance_struct;
	//struct build_by_get_only{};已定义于 "../../base_defs/special_attribute.hpp"

	/*!
	向后减小数据块大小并转移原有实例的生命周期，但并不析构旧的实例

	@param arg 原有数据块
	@param to_size 新的数据块大小
	*/
	template<typename T>
	void alloc_size_cut(T*&arg,size_t to_size)noexcept{
		realloc(arg,to_size);
	}
	/*!
	向后扩大数据块大小并转移原有实例的生命周期，但并不构造新的实例

	@param arg 原有数据块
	@param to_size 新的数据块大小
	*/
	template<typename T>
	void alloc_size_grow(T*&arg,size_t to_size)noexcept(move.trivial<T> or move.nothrow<T>){
		if constexpr(move.trivial<T>)
			realloc(arg,to_size);
		else{
			T*tmp=alloc<T>(to_size);
			const auto from_size=get_size_of_alloc(arg);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[from_size](note::from(arg),note::to(tmp));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[from_size](note::from(arg),note::to(tmp));
			}
			free(arg);
			arg=tmp;
		}
	}
	/*!
	在指定位置插入未初始化数据块并转移原有实例的生命周期，但并不构造新的实例

	@param arg 原有数据块
	@param insert_pos 插入位置
	@param insert_size 插入大小
	*/
	template<typename T>
	void alloc_size_grow_with_insert_uninitialized_data(T*&arg,size_t insert_pos,size_t insert_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		const auto to_size=from_size+insert_size;
		const auto size_before_insert=insert_pos;
		#if in_debug || defined(ELC_CHECK_MEMORYS_ERROR)
			if(from_size<insert_pos)
				__debugbreak();//？
		#endif
		const auto size_after_insert=from_size-insert_pos;
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			memmove(arg+size_before_insert+insert_size,arg+size_before_insert,size_after_insert*sizeof(T));
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[size_before_insert](note::from(arg),note::to(tmp));
					move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[size_before_insert](note::from(arg),note::to(tmp));
				move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
			}
			free(arg);
			arg=tmp;
		}
	}
	/*!
	在指定位置插入未初始化数据块并转移原有实例的生命周期，但并不构造新的实例
	若有多余的大小，追加到末尾

	@param arg 原有数据块
	@param to_size 新的数据块大小
	@param insert_pos 插入位置
	@param insert_size 插入大小

	@returns 详细的操作信息
	*/
	template<typename T>
	force_inline auto alloc_size_grow_with_insert_uninitialized_data(T*&arg,size_t to_size,size_t insert_pos,size_t insert_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		const auto size_before_insert=insert_pos;
		#if in_debug || defined(ELC_CHECK_MEMORYS_ERROR)
			if(from_size<insert_pos)
				__debugbreak();//？
		#endif
		const auto size_after_insert=from_size-insert_pos;
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			memmove(arg+size_before_insert+insert_size,arg+size_before_insert,size_after_insert*sizeof(T));
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[size_before_insert](note::from(arg),note::to(tmp));
					move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[size_before_insert](note::from(arg),note::to(tmp));
				move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
			}
			free(arg);
			arg=tmp;
		}
		struct result_t{
			size_t from_size;
			size_t to_size;
			size_t size_before_insert;
			size_t size_after_insert;
			size_t insert_pos;
			size_t insert_size;
			size_t end_grow_size()const noexcept{return to_size-from_size-insert_size;}
			T* origin_data_ptr(T*arg)const noexcept{return arg;}
			T* insert_data_ptr(T*arg)const noexcept{return arg+insert_pos;}
			T* end_growth_data_ptr(T*arg)const noexcept{return arg+to_size-end_grow_size();}
		} result{
			from_size,
			to_size,
			size_before_insert,
			size_after_insert,
			insert_pos,
			insert_size
		};
		return result;
	}
	/*!
	假设尾部有未使用的对象前提下，在指定位置插入未初始化数据块并转移原有实例的生命周期，允许新的参数描述已使用的对象多少，并不构造新的实例
	若有多余的大小，追加到末尾

	@param arg 原有数据块
	@param to_size 新的数据块大小
	@param insert_pos 插入位置
	@param insert_size 插入大小
	@param used_size 已使用的对象多少

	@returns 详细的操作信息
	*/
	template<typename T>
	force_inline auto part_used_block_alloc_size_grow_with_insert_uninitialized_data(T*&arg,size_t to_size,size_t insert_pos,size_t insert_size,size_t used_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		const auto size_before_insert=insert_pos;
		const auto size_after_insert=used_size-insert_pos;
		const auto unused_size_before_insert=from_size-used_size;
		const auto unused_size_after_insert=to_size-insert_size-used_size;
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			//先移动插入末尾的元素
			memmove(arg+size_before_insert+insert_size,arg+insert_pos,size_after_insert*sizeof(T));
			//插入前的元素没有移动
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
					//这里我们需要移动插入前的元素因为这是新的空间
					move[size_before_insert](note::from(arg),note::to(tmp));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[size_after_insert](note::from(arg+insert_pos),note::to(tmp+insert_pos+insert_size));
				//这里我们需要移动插入前的元素因为这是新的空间
				move[size_before_insert](note::from(arg),note::to(tmp));
			}
			free(arg);
			arg=tmp;
		}
		struct result_t{
			size_t from_size;
			size_t to_size;
			size_t unused_size_before_insert;
			size_t unused_size_after_insert;
			size_t used_size;
			size_t size_before_insert;
			size_t size_after_insert;
			size_t insert_pos;
			size_t insert_size;
			size_t end_grow_size()const noexcept{return to_size-from_size-insert_size;}
			T* origin_data_ptr(T*arg)const noexcept{return arg;}
			T* insert_data_ptr(T*arg)const noexcept{return arg+insert_pos;}
			T* end_growth_data_ptr(T*arg)const noexcept{return arg+to_size-used_size;}
		} result{
			from_size,
			to_size,
			unused_size_before_insert,
			unused_size_after_insert,
			used_size,
			size_before_insert,
			size_after_insert,
			insert_pos,
			insert_size
		};
		return result;
	}
	/*!
	向前减小数据块大小并转移原有实例的生命周期，但并不析构旧的实例

	@param arg 原有数据块
	@param to_size 新的数据块大小
	*/
	template<typename T>
	void forward_alloc_size_cut(T*&arg,size_t to_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		#if in_debug || defined(ELC_CHECK_MEMORYS_ERROR)
			if(to_size>from_size)
				__debugbreak();//？
		#endif
		const auto cut_size=from_size-to_size;
		if constexpr(move.trivial<T>){
			::std::memmove(arg,add_const(arg+cut_size),to_size*sizeof(T));
			realloc(arg,to_size);
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[to_size](note::from(arg+cut_size),note::to(tmp));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[to_size](note::from(arg+cut_size),note::to(tmp));
			}
			free(arg);
			arg=tmp;
		}
	}
	/*!
	向前扩大数据块大小并转移原有实例的生命周期，但并不构造新的实例

	@param arg 原有数据块
	@param to_size 新的数据块大小
	*/
	template<typename T>
	void forward_alloc_size_grow(T*&arg,size_t to_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		#if in_debug || defined(ELC_CHECK_MEMORYS_ERROR)
			if(to_size<from_size)
				__debugbreak();//？
		#endif
		const auto grow_size=to_size-from_size;
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			::std::memmove(arg+grow_size,add_const(arg),from_size*sizeof(T));
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[from_size](note::from(arg),note::to(tmp+grow_size));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[from_size](note::from(arg),note::to(tmp+grow_size));
			}
			free(arg);
			arg=tmp;
		}
	}
	/*!
	在指定位置插入未初始化数据块并转移原有实例的生命周期，但并不构造新的实例
	若有多余的大小，追加到前端

	@param arg 原有数据块
	@param to_size 新的数据块大小
	@param insert_pos 插入位置
	@param insert_size 插入大小

	@returns 详细的操作信息
	*/
	template<typename T>
	force_inline auto forward_alloc_size_grow_with_insert_uninitialized_data(T*&arg,size_t to_size,size_t insert_pos,size_t insert_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);
		const auto grow_size=to_size-from_size;
		#if in_debug || defined(ELC_CHECK_MEMORYS_ERROR)
			if(grow_size<insert_size)
				__debugbreak();//？
		#endif
		const auto before_grow_size=grow_size-insert_size;
		const auto size_before_insert=insert_pos;
		const auto size_after_insert=from_size-insert_pos;
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			const auto origin_data_ptr=arg+before_grow_size;
			::std::memmove(origin_data_ptr+size_before_insert+insert_size,arg+size_before_insert,size_after_insert*sizeof(T));
			::std::memmove(origin_data_ptr,arg,size_before_insert*sizeof(T));
		}
		else{
			T*tmp=alloc<T>(to_size);
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					const auto origin_data_ptr=tmp+before_grow_size;
					move[size_before_insert](note::from(arg),note::to(origin_data_ptr));
					move[size_after_insert](note::from(arg+size_before_insert),note::to(origin_data_ptr+size_before_insert+insert_size));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				const auto origin_data_ptr=tmp+before_grow_size;
				move[size_before_insert](note::from(arg),note::to(origin_data_ptr));
				move[size_after_insert](note::from(arg+size_before_insert),note::to(origin_data_ptr+size_before_insert+insert_size));
			}
			free(arg);
			arg=tmp;
		}
		struct result_t{
			size_t from_size;
			size_t to_size;
			size_t before_grow_size;
			size_t size_before_insert;
			size_t size_after_insert;
			size_t insert_pos;
			size_t insert_size;
			T* origin_data_ptr(T*arg)const noexcept{return arg+before_grow_size;}
			T* insert_data_ptr(T*arg)const noexcept{return origin_data_ptr(arg)+insert_pos;}
		} result{
			from_size,
			to_size,
			before_grow_size,
			size_before_insert,
			size_after_insert,
			insert_pos,
			insert_size
		};
		return result;
	}
	/*!
	假设头部有未使用的对象前提下，在指定位置插入未初始化数据块并转移原有实例的生命周期，允许新的参数描述已使用的对象多少，并不构造新的实例
	若有多余的大小，追加到前端

	@param arg 原有数据块
	@param to_size 新的数据块大小
	@param insert_pos 插入位置
	@param insert_size 插入大小
	@param used_size 已使用的对象多少

	@returns 详细的操作信息
	*/
	template<typename T>
	force_inline auto part_used_block_forward_alloc_size_grow_with_insert_uninitialized_data(T*&arg,size_t to_size,size_t insert_pos,size_t insert_size,size_t used_size)noexcept(move.trivial<T> or move.nothrow<T>){
		const auto from_size=get_size_of_alloc(arg);//原有数据块大小
		const auto grow_size=to_size-from_size;//扩大的大小
		const auto used_size_after_insert=used_size+insert_size;//插入后的已使用大小
		const auto unused_size_after_insert=to_size-used_size_after_insert;//插入后的未使用大小
		const auto unused_size_before_insert=from_size-used_size;//插入前的未使用大小

		const auto after_insert_size=from_size-insert_pos;//插入位置之后的数据块的大小
		const auto before_insert_size=insert_pos-unused_size_before_insert;//插入位置之前的数据块的大小
		insert_pos-=unused_size_before_insert;//插入位置
		if constexpr(move.trivial<T>){
			realloc(arg,to_size);
			const auto new_data_ptr=arg+unused_size_after_insert;
			const auto origin_data_ptr=arg+unused_size_before_insert;
			//先移动插入末尾的元素
			::std::memmove(new_data_ptr+insert_pos+insert_size,origin_data_ptr+insert_pos,after_insert_size*sizeof(T));
			//再移动插入前的元素
			::std::memmove(new_data_ptr,origin_data_ptr,before_insert_size*sizeof(T));
		}
		else{
			T*tmp=alloc<T>(to_size);
			const auto new_data_ptr=tmp+unused_size_after_insert;
			const auto origin_data_ptr=arg+unused_size_before_insert;
			if constexpr(!move.nothrow<T>){
				template_warning("the move of T was not noexcept,this may cause memory lack.");
				try{
					move[after_insert_size](note::from(origin_data_ptr+insert_pos),note::to(new_data_ptr+insert_pos+insert_size));
					move[before_insert_size](note::from(origin_data_ptr),note::to(new_data_ptr));
				}catch(...){
					free(tmp);
					throw;
				}
			}else{
				move[after_insert_size](note::from(origin_data_ptr+insert_pos),note::to(new_data_ptr+insert_pos+insert_size));
				move[before_insert_size](note::from(origin_data_ptr),note::to(new_data_ptr));
			}
			free(arg);
			arg=tmp;
		}
		struct result_t{
			size_t from_size;
			size_t to_size;
			size_t unused_size_before_insert;
			size_t unused_size_after_insert;
			size_t used_size_after_insert;
			size_t before_insert_size;
			size_t after_insert_size;
			size_t used_size;
			size_t grow_size;
			size_t insert_pos;
			size_t insert_size;
			T* new_data_ptr(T*arg)const noexcept{return arg+unused_size_after_insert;}
			T* origin_data_ptr(T*arg)const noexcept{return arg+unused_size_before_insert;}
			size_t before_grow_size()const noexcept{return grow_size+used_size-insert_size;}
			T* insert_data_ptr(T*arg)const noexcept{return arg+unused_size_after_insert+insert_pos;}
		} result{
			from_size,
			to_size,
			unused_size_before_insert,
			unused_size_after_insert,
			used_size_after_insert,
			before_insert_size,
			after_insert_size,
			used_size,
			grow_size,
			insert_pos,
			insert_size
		};
		return result;
	}

	struct base_get_t{};
	template<typename T>
	struct get_t:base_get_t{
		typedef base_get_t base_t;
		template<class...Args>
		static constexpr bool able=construct<T>.able<Args...>&&destruct.able<T>;
		template<class...Args>
		static constexpr bool nothrow=construct<T>.nothrow<Args...>;

		/*!
		Constructs an object of type T.

		@param rest The arguments to pass to the constructor of T.

		@returns A pointer to the newly constructed object.
		*/
		template<class...Args> requires able<Args...>
		[[nodiscard]]T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
			const APIs::alloc::source_location_guard slg;
			return construct<T>[alloc<T>()](forward<Args>(rest)...);
		}

		/*!
		Allocates an array of T with the specified size.
		*/
		struct array_get_t{
			size_t _size;
			/*!
			Constructs an array of objects of type T.

			@param rest The arguments to pass to the constructor of T.

			@returns An array of objects of type T.
			*/
			template<class...Args> requires able<Args...>
			[[nodiscard]]T* operator()(Args&&...rest)const noexcept(nothrow<Args...>){
				const APIs::alloc::source_location_guard slg;
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t get an array for never_in_array type.");
				return construct<T>[alloc<T>(_size)][_size](forward<Args>(rest)...);
			}
		};
		[[nodiscard]]constexpr array_get_t operator[](size_t size)const noexcept{return{size};}

		static constexpr struct as_array_t{
			template<typename U>
			static constexpr bool able=copy_construct.able<T>&&destruct.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=copy_construct.nothrow<T>;

			[[nodiscard]]T* operator()(array_like_view_t<const T>a)const noexcept(nothrow<void>){
				const APIs::alloc::source_location_guard slg;
				if constexpr(type_info<T>.has_attribute(never_in_array))
					template_error("You can\'t get an array for never_in_array type.");
				const auto size=a.size();
				auto aret=alloc<T>[size]();
				copy_construct[size](note::from(a.begin()),note::to(aret));
				return aret;
			}
		}as_array{};

		static constexpr struct apply_end_t{
			template<typename U>
			static constexpr bool able=copy_construct.able<T>&&move.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=copy_construct.nothrow<T>&&move.nothrow<T>;

			T* operator()(note::to_t<T*&> to,array_like_view_t<const T>a)const noexcept(nothrow<void>){
				const APIs::alloc::source_location_guard slg;
				auto&ptr=to.value;
				auto from_size=get_size_of_alloc(ptr);
				auto a_size=a.size();
				alloc_size_grow(ptr,from_size+a_size);
				copy_construct[a_size](note::from(a.begin()),note::to(ptr+from_size));
				return ptr;
			}
			T* operator()(note::to_t<T*&> to,const T&a)const noexcept(nothrow<void>){
				const APIs::alloc::source_location_guard slg;
				auto&ptr=to.value;
				auto from_size=get_size_of_alloc(ptr);
				alloc_size_grow(ptr,from_size+1);
				copy_construct(note::from(&a),note::to(ptr+from_size));
				return ptr;
			}
			T* operator()(note::to_t<T*&> to,T&&a)const noexcept(nothrow<void>){
				const APIs::alloc::source_location_guard slg;
				auto&ptr=to.value;
				auto from_size=get_size_of_alloc(ptr);
				alloc_size_grow(ptr,from_size+1);
				move_construct(note::from(&a),note::to(ptr+from_size));
				return ptr;
			}
		}apply_end{};

		static constexpr struct remove_t{
			template<typename U>
			static constexpr bool able=destruct.able<T>&&move.able<T>&&is_array_like_for<T,U>;
			template<typename U>
			static constexpr bool nothrow=destruct.nothrow<T>&&move.nothrow<T>;

			bool operator()(array_like_view_t<const T>a,note::from_t<T*>from)const noexcept(nothrow<void>){
				const APIs::alloc::source_location_guard slg;
				auto ptr=from.value;
				auto from_size=get_size_of_alloc(ptr);
				T*ptr_to_a=in_range(a,{ptr,note::size(from_size)});
				auto a_size=a.size();
				if(!ptr_to_a)
					return false;

				destruct[a_size](ptr_to_a);
				move[(ptr+from_size)-(ptr_to_a+a_size)](note::from(ptr_to_a+a_size),note::to(ptr_to_a));
				alloc_size_cut(ptr,from_size-a_size);
				return true;
			}
		}remove{};
	};
	template<typename T>
	constexpr get_t<T>get{};


	constexpr struct unget_t{
		template<typename T>
		static constexpr bool able=destruct.able<T>;
		template<typename T>
		static constexpr bool nothrow=destruct.nothrow<T>;

		template<typename T> requires able<T>
		void operator()(T*a)const noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg;
			if(a!=null_ptr){
				if constexpr(::std::is_polymorphic_v<T> && !::std::is_final_v<T>)
					if constexpr(type_info<T>.has_attribute(abstract_base)){
						#if defined(_MSC_VER)
							[[gsl::suppress(f.6)]]
						#endif
						attribute_ptr_cast<abstract_base>(a)->abstract_method_unget_this();
						return;
					}
					elseif constexpr(type_info<T>.not_has_attribute(instance_struct))
						template_warning("T is polymorphic, but there is no elc::abstract_base attribute to provide information to elc's memory system, which could lead to incorrect unget handling.");
				if constexpr(!destruct.nothrow<T>)
					template_warning("the destructer of T was not noexcept,this may cause memory lack.");
				if constexpr(type_info<T>.has_attribute(never_in_array))
					destruct(a);
				else
					destruct[get_size_of_alloc(a)](a);
				free(a);
			}
		}
		/*适用于unget(this,not destruct);*/
		template<typename T> requires able<T>
		void operator()(T*a,decltype(destruct)::not_t)const noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg;
			free(a);
		}
	}unget{};

	constexpr struct get_resize_t{
		template<typename T>
		static constexpr bool able=construct<T>.able<>&&destruct.able<T>&&move.able<T>;
		template<typename T>
		static constexpr bool nothrow=type_info<T>.not_has_attribute(abstract_base)&&construct<T>.nothrow<>&&destruct.nothrow<T>&&move.nothrow<T>;

		template<typename T> requires able<T>
		static void base_call(T*&arg,const size_t to_size)noexcept(nothrow<T>){
			if constexpr(type_info<T>.has_attribute(never_in_array)){
				template_warning("For never_in_array type,get_resize will unget ptr when new_size=0 else do nothing.");
				if(to_size)
					return;
				unget(arg);
				arg=null_ptr;
			}elseif constexpr(type_info<T>.has_attribute(abstract_base)){
				arg=attribute_ptr_cast<abstract_base>(arg)->abstract_method_get_resize_this(to_size);
			}else{
				const size_t from_size=get_size_of_alloc(arg);
				if(from_size==to_size)
					return;
				elseif(from_size > to_size){
					destruct[from_size-to_size](arg+to_size);
					alloc_size_cut(arg,to_size);
				}elseif(from_size){
					alloc_size_grow(arg,to_size);
					construct<T>[arg+from_size][to_size-from_size]();
				}else
					arg=get<T>[to_size]();
			}
		}

		template<typename T> requires able<T>
		inline void operator()(T*&arg,size_t to_size)const noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg;
			base_call(arg,to_size);
		}
		template<typename T> requires able<T>
		[[nodiscard]]inline T* operator()(T*&&arg,size_t to_size)const noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg;
			base_call(arg,to_size);
			return arg;
		}

		static constexpr struct insert_t{
			template<typename T>
			static constexpr bool able=get_resize_t::able<T>;
			template<typename T>
			static constexpr bool nothrow=get_resize_t::nothrow<T>;
			template<typename T> requires(able<T> && construct<T>.able<>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size)const noexcept(nothrow<T>){
				if(insert_size){
					const APIs::alloc::source_location_guard slg;
					alloc_size_grow_with_insert_uninitialized_data(arg,insert_pos,insert_size);
					construct<T>[arg+insert_pos][insert_size]();
				}
			}
			template<typename T> requires(able<T> && copy_construct.able<T>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T&initer)const noexcept(nothrow<T>){
				if(insert_size){
					const APIs::alloc::source_location_guard slg;
					alloc_size_grow_with_insert_uninitialized_data(arg,insert_pos,insert_size);
					copy_construct[insert_size](arg+insert_pos,initer);
				}
			}
			template<typename T> requires(able<T> && copy_construct.able<T>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data)const noexcept(nothrow<T>){
				if(insert_size){
					const APIs::alloc::source_location_guard slg;
					alloc_size_grow_with_insert_uninitialized_data(arg,insert_pos,insert_size);
					copy_construct[insert_size](note::from(insert_data),note::to(arg+insert_pos));
				}
			}
		}insert{};
		static constexpr struct insert_resize_t{
			template<typename T>
			static constexpr bool able=get_resize_t::able<T>;
			template<typename T>
			static constexpr bool nothrow=get_resize_t::nothrow<T>;
			template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size)const noexcept(nothrow<T>){
				const APIs::alloc::source_location_guard slg;
				const auto info=alloc_size_grow_with_insert_uninitialized_data(arg,to_size,insert_pos,insert_size);
				copy_construct[insert_size](note::from(insert_data),note::to(info.insert_data_ptr(arg)));
				construct<T>[info.end_growth_data_ptr(arg)][info.end_grow_size()]();
			}
			static constexpr struct with_used_size_t{
				template<typename T>
				static constexpr bool able=get_resize_t::able<T>;
				template<typename T>
				static constexpr bool nothrow=get_resize_t::nothrow<T>;
				template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
				static void base_call(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size,size_t used_size)noexcept(nothrow<T>){
					const APIs::alloc::source_location_guard slg;
					const auto info=part_used_block_alloc_size_grow_with_insert_uninitialized_data(arg,to_size,insert_pos,insert_size,used_size);
					copy_construct[insert_size](note::from(insert_data),note::to(info.insert_data_ptr(arg)));
					construct<T>[info.end_growth_data_ptr(arg)][info.end_grow_size()]();
				}
				template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
				force_inline void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size,size_t used_size)const noexcept(nothrow<T>){
					base_call(arg,insert_pos,insert_size,insert_data,to_size,used_size);
				}
				struct used_size_index_result_t{
					size_t _used_size;
					template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
					force_inline void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size)const noexcept(nothrow<T>){
						base_call(arg,insert_pos,insert_size,insert_data,to_size,_used_size);
					}
				};
				[[nodiscard]]constexpr used_size_index_result_t operator[](size_t used_size)const noexcept{return{used_size};}
			}with_used_size{};
		}insert_with_resize{};
	}get_resize{};

	constexpr struct get_forward_resize_t{
		template<typename T>
		static constexpr bool able=construct<T>.able<>&&destruct.able<T>&&move.able<T>;
		template<typename T>
		static constexpr bool nothrow=type_info<T>.not_has_attribute(abstract_base)&&construct<T>.nothrow<>&&destruct.nothrow<T>&&move.nothrow<T>;

		template<typename T> requires able<T>
		static void base_call(T*&arg,const size_t to_size)noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg{1};
			if constexpr(type_info<T>.has_attribute(never_in_array)){
				template_warning("For never_in_array type,get_forward_resize will unget ptr when new_size=0 else do nothing.");
				if(to_size)
					return;
				unget(arg);
				arg=null_ptr;
			}elseif constexpr(type_info<T>.has_attribute(abstract_base)){
				arg=attribute_ptr_cast<abstract_base>(arg)->abstract_method_get_forward_resize_this(to_size);
			}else{
				const size_t from_size=get_size_of_alloc(arg);
				if(from_size==to_size)
					return;
				elseif(from_size > to_size){
					destruct[from_size-to_size](arg);
					forward_alloc_size_cut(arg,to_size);
				}elseif(from_size){
					forward_alloc_size_grow(arg,to_size);
					construct<T>[arg][to_size-from_size]();
				}else
					arg=get<T>[to_size]();
			}
		}

		template<typename T> requires able<T>
		inline void operator()(T*&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
		}
		template<typename T> requires able<T>
		[[nodiscard]]inline T* operator()(T*&&arg,size_t to_size)const noexcept(nothrow<T>){
			base_call(arg,to_size);
			return arg;
		}

		static constexpr struct insert_t{
			template<typename T>
			static constexpr bool able=get_resize_t::able<T>;
			template<typename T>
			static constexpr bool nothrow=get_resize_t::nothrow<T>;
			template<typename T> requires(able<T> && construct<T>.able<>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size)const noexcept(nothrow<T>){
				const APIs::alloc::source_location_guard slg;
				get_resize.insert(arg,insert_pos,insert_size);
			}
			template<typename T> requires(able<T> && copy_construct.able<T>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data)const noexcept(nothrow<T>){
				const APIs::alloc::source_location_guard slg;
				get_resize.insert(arg,insert_pos,insert_size,insert_data);
			}
		}insert{};
		static constexpr struct insert_resize_t{
			template<typename T>
			static constexpr bool able=get_resize_t::able<T>;
			template<typename T>
			static constexpr bool nothrow=get_resize_t::nothrow<T>;
			template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
			void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size)const noexcept(nothrow<T>){
				if(insert_size){
					const APIs::alloc::source_location_guard slg;
					const auto info=forward_alloc_size_grow_with_insert_uninitialized_data(arg,to_size,insert_pos,insert_size);
					copy_construct[insert_size](note::from(insert_data),note::to(info.insert_data_ptr(arg)));
					construct<T>[arg][info.before_grow_size]();
				}
			}
			static constexpr struct with_used_size_t{
				template<typename T>
				static constexpr bool able=get_resize_t::able<T>;
				template<typename T>
				static constexpr bool nothrow=get_resize_t::nothrow<T>;
				template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
				static void base_call(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size,size_t used_size)noexcept(nothrow<T>){
					if(insert_size){
						const APIs::alloc::source_location_guard slg;
						const auto info=part_used_block_forward_alloc_size_grow_with_insert_uninitialized_data(arg,to_size,insert_pos,insert_size,used_size);
						copy_construct[insert_size](note::from(insert_data),note::to(info.insert_data_ptr(arg)));
						construct<T>[arg][info.before_grow_size()]();
					}
				}
				template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
				force_inline void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size,size_t used_size)const noexcept(nothrow<T>){
					base_call(arg,insert_pos,insert_size,insert_data,to_size,used_size);
				}
				struct used_size_index_result_t{
					size_t _used_size;
					template<typename T> requires(able<T> && construct<T>.able<> && copy_construct.able<T>)
					force_inline void operator()(T*&arg,size_t insert_pos,size_t insert_size,const T*insert_data,size_t to_size)const noexcept(nothrow<T>){
						base_call(arg,insert_pos,insert_size,insert_data,to_size,_used_size);
					}
				};
				[[nodiscard]]constexpr used_size_index_result_t operator[](size_t used_size)const noexcept{return{used_size};}
			}with_used_size{};
		}insert_with_resize{};
	}get_forward_resize{};

	constexpr struct get_size_of_get_t{
		template<typename T>
		static constexpr bool able=true;
		template<typename T>
		static constexpr bool nothrow=noexcept(get_size_of_alloc(declvalue(const T*)));

		template<typename T> requires able<T>
		[[nodiscard]]inline static size_t base_call(const T*arg)noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg{1};
			return get_size_of_alloc(arg);
		}

		template<typename T> requires able<T>
		[[nodiscard]]inline size_t operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}get_size_of_get{};

	constexpr struct copy_get_t{
		template<typename T>
		static constexpr bool able=copy_construct.able<T> || type_info<T>.has_attribute(abstract_base);
		template<typename T>
		static constexpr bool nothrow=copy_construct.nothrow<T>;

		template<typename T> requires able<T>
		[[nodiscard]]inline static T*base_call(const T*arg)noexcept(nothrow<T>){
			const APIs::alloc::source_location_guard slg{1};
			if constexpr(type_info<T>.has_attribute(abstract_base))
				return remove_const(attribute_ptr_cast<abstract_base>(arg))->abstract_method_copy_get_this();
			else
				return copy_construct(note::from(arg),note::to(copy_alloc(arg)),get_size_of_get(arg));
		}

		template<typename T> requires able<T>
		[[nodiscard]]inline T*operator()(const T*arg)const noexcept(nothrow<T>){
			return base_call(arg);
		}
	}copy_get{};
}

//file_end

