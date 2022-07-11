#include "defs.hpp"
#include <iostream>

bool is_this_package_installed(package_info_t& package_info) {
	return true;
}

bool is_this_package_can_be_found(package_info_t& package_info) {
	if(package_info.package_name != "C")
		return true;
	return false;
}

base_package_relay_info_ptr get_package_relay_info(const package_info_t& package_info) {
	//for test:
	//A relay on B
	//B relay on C or (D&E)
	auto packD = (base_package_relay_info_t*)new signal_package_relay_info_t{
		{"D", 0, 0}};
	base_package_relay_info_ptr D(packD);
	auto packE = (base_package_relay_info_t*) new signal_package_relay_info_t{
		{"E", 0, 0}};
	base_package_relay_info_ptr E(packE);
	auto packDandE= (base_package_relay_info_t*) new common_package_relay_info_t{
		{D, E}};
	base_package_relay_info_ptr DandE(packDandE);
	auto packB_name = (base_package_relay_info_t*)new signal_package_relay_info_t{
		{"B", 0, 0}};
	base_package_relay_info_ptr B_name(packB_name);
	auto packC = (base_package_relay_info_t*) new signal_package_relay_info_t{
		{"C", 0, 0}};
	base_package_relay_info_ptr C(packC);
	auto packB_base = (base_package_relay_info_t*)new pick_one_package_relay_info_t{
		 {C, DandE}};
	base_package_relay_info_ptr B_base(packB_base);
	auto packB = (base_package_relay_info_t*)new common_package_relay_info_t{
		   {B_base, B_name}};
	base_package_relay_info_ptr B(packB);
	auto packA_name = (base_package_relay_info_t*)new signal_package_relay_info_t{
		   {"A", 0, 0}};
	base_package_relay_info_ptr A_name(packA_name);
	auto packA = (base_package_relay_info_t*)new common_package_relay_info_t{
		   {B, A_name}};
	base_package_relay_info_ptr A(packA);
	if(package_info.package_name == "A")
		return A;
	else
		return nullptr;
}

int main() {
	auto load_list = get_package_load_list({"A",0,0});
	for(auto package: load_list.package_list)
		cout << package.package_name << " ";
}
