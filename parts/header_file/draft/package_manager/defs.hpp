#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <ranges>
using namespace std;

struct package_not_found : runtime_error {
	package_not_found(string const& name) : runtime_error("package not found: " + name) {}
};
struct package_info_t {
	string package_name;
	//Not backward compatible package version
	size_t package_version_major;
	//Backward compatible package version
	size_t package_version_minor;

	bool is_compatible(const package_info_t& other) const {
		return package_version_major == other.package_version_major && package_version_minor >= other.package_version_minor;
	}
	bool same_package_as(const package_info_t& other) const {
		return package_name == other.package_name && package_version_major == other.package_version_major;
	}
};
bool is_this_package_installed(package_info_t& package_info);
bool is_this_package_can_be_found(package_info_t& package_info);
inline bool is_this_package_can_not_be_found(package_info_t& package_info) {
	return !is_this_package_can_be_found(package_info);
}
struct package_relay_list {
	vector<package_info_t> package_list;

	package_relay_list(vector<package_info_t> package_list) {
		this->package_list = package_list;
	}
	package_relay_list() {
	}

	[[nodiscard]] bool already_in_list(package_info_t package_info) const {
		for(auto& package: package_list)
			if(package.same_package_as(package_info))
				return true;
		return false;
	}
	void add_package(package_info_t package_info) {
		if(!already_in_list(package_info)) {
			if(is_this_package_can_not_be_found(package_info))
				throw package_not_found(package_info.package_name);
			package_list.push_back(package_info);
		}
	}
	void add_list(const package_relay_list& other_list) {
		for(auto& package_info: other_list.package_list)
			add_package(package_info);
	}
};

struct base_package_relay_info_t {
	virtual package_relay_list& get_relay_list(package_relay_list&) = 0;
	package_relay_list get_relay_list(){
		package_relay_list relay_list;
		get_relay_list(relay_list);
		return relay_list;
	}
};
//def ptr of base_package_relay_info_t
typedef shared_ptr<base_package_relay_info_t> base_package_relay_info_ptr;

base_package_relay_info_ptr get_package_relay_info(const package_info_t& package_info);

struct common_package_relay_info_t: public base_package_relay_info_t {
	vector<base_package_relay_info_ptr> relay_info_list;

	common_package_relay_info_t(vector<base_package_relay_info_ptr> relay_info_list) {
		this->relay_info_list = relay_info_list;
	}
	virtual package_relay_list& get_relay_list(package_relay_list& package_list) override {
		for(auto relay_info: relay_info_list) {
			relay_info->get_relay_list(package_list);
		}
		return package_list;
	}
};
struct signal_package_relay_info_t: public base_package_relay_info_t {
	package_info_t package_info;
	
	signal_package_relay_info_t(package_info_t package_info) {
		this->package_info = package_info;
	}
	virtual package_relay_list& get_relay_list(package_relay_list& package_list) override {
		package_list.add_package(package_info);
		return package_list;
	}
};
struct pick_one_package_relay_info_t: public base_package_relay_info_t {
	vector<base_package_relay_info_ptr> pick_one_list;
	
	pick_one_package_relay_info_t(vector<base_package_relay_info_ptr> pick_one_list) {
		this->pick_one_list = pick_one_list;
	}
	virtual package_relay_list&	get_relay_list(package_relay_list& package_list) override {
		//Exclude lists containing items that cannot be found
		vector<vector<package_info_t>> package_list_list;
		for(auto relay_info: pick_one_list) {
			auto relay_list	   = package_list;
			try {
				relay_list = relay_info->get_relay_list(relay_list);
			}
			catch(package_not_found&) {
				continue;
			}
			package_list_list.push_back(relay_list.package_list);
		}
		if(package_list_list.size() == 0)
			throw runtime_error("No package can be found");
		//pick the minimum size list
		{
			size_t min_size = package_list_list[0].size();
			for(auto& list: package_list_list)
				if(list.size() < min_size)
					min_size = list.size();
			for(auto it = package_list_list.begin(); it != package_list_list.end();)
				if(it->size() != min_size)
					it = package_list_list.erase(it);
				else
					++it;
		}
		if(package_list_list.size() == 1)
			goto pick_end;
		//pick list with the maximum number of installations
		{
			size_t		   max_installed_count = 0;
			vector<size_t> installed_count_list;
			for(auto& list: package_list_list) {
				size_t installed_count = 0;
				for(auto& package: list)
					if(is_this_package_installed(package))
						++installed_count;
				if(installed_count > max_installed_count)
					max_installed_count = installed_count;
				installed_count_list.push_back(installed_count);
			}
			for(auto it = package_list_list.begin(); it != package_list_list.end();)
				if(installed_count_list[it - package_list_list.begin()] != max_installed_count)
					it = package_list_list.erase(it);
				else
					++it;
		}
		if(package_list_list.size() == 1)
			goto pick_end;
	pick_end:
		size_t index = rand() % package_list_list.size();
		package_list.add_list(package_list_list[index]);
		return package_list;
	}
};

inline package_relay_list get_package_load_list(const package_info_t& package_info) {
	package_relay_list package_list;
	auto			   relay_info = get_package_relay_info(package_info);
	relay_info->get_relay_list(package_list);
	return package_list;
}

//
