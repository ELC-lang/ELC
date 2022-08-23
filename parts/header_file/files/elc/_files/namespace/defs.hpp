//defs.hpp
/*
未完成的elc解释器namespace文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace core{
	template<>
	distinctive inline map_t<ptr,string> long_term_binary_node_storager<string>{};
}
namespace namespace_n{
	distinctive inline struct namespace_t{
		ptr _namespace_root;
		weak_ptr _namespace_now;
		value get_symbol_type_map(ptr thenamespace){
			return (*thenamespace)[es".symbol_type_map"];
		}
		namespace_t()noexcept{
			_namespace_root = get<common_node>;
			auto&nmsp	    = *_namespace_root;
			_namespace_now  = &nmsp;
			#if defined(_MSC_VER)
				#pragma warning(push)
				#pragma warning(disable:26447)//may throw警告diss
			#endif
			nmsp[es".."]	 = the_void;
			nmsp[es"."]		 = nmsp;
			nmsp[es"t"]		 = t;
			nmsp[es"nil"]	 = nil;
			nmsp[es"void"]	 = the_void;
			value nmsptm	 = get_symbol_type_map(&nmsp);
			nmsptm			 = get<common_node>;
			nmsptm[es"void"] = es"void";
			#if defined(_MSC_VER)
				#pragma warning(pop)
			#endif
		}
		ptr get_symbol_namespace(const value index){
			ptr namespace_now_p = _namespace_now;
			while(namespace_now_p && !(*namespace_now_p)[index] && !get_symbol_type_map(namespace_now_p)[index])
				namespace_now_p = (*namespace_now_p)[es".."];
			return namespace_now_p;
		}
		ptr get_symbol_namespace(auto&& index){ return get_symbol_namespace(as_value(index)); }
		bool has_symbol(const value index){
			auto nmsp=get_symbol_namespace(index);
			if((*nmsp)[index] || get_symbol_type_map(nmsp)[index])
				return true;
			return false;
		}
		bool has_symbol(auto&& index){ return has_symbol(as_value(index)); }
		value get_symbol(const value index){
			return (*get_symbol_namespace(index))[index];
		}
		value get_symbol(auto&& index){ return get_symbol(as_value(index)); }
		value get_symbol_type(const value index){
			return get_symbol_type_map(get_symbol_namespace(index))[index];
		}
		value get_symbol_type(auto&& index){ return get_symbol_type(as_value(index)); }

		void inter_namespace(const value namespace_name){
			_namespace_now = get_symbol(namespace_name);
		}
		void inter_namespace(auto&& index){ inter_namespace(as_value(index)); }

		void leave_namespace(){ inter_namespace(es".."); }
	}names{};
}

//file_end

