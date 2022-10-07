//non_Xable.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
/// 禁止复制赋值
struct non_copy_assign_able{
	constexpr non_copy_assign_able()noexcept=default;
	constexpr non_copy_assign_able(const non_copy_assign_able&)noexcept=default;
	constexpr non_copy_assign_able(non_copy_assign_able&&)noexcept=default;
	constexpr non_copy_assign_able& operator=(const non_copy_assign_able&)&noexcept=delete;
	constexpr non_copy_assign_able& operator=(non_copy_assign_able&&)&noexcept=default;
};
/// 禁止移动赋值
struct non_move_assign_able{
	constexpr non_move_assign_able()noexcept=default;
	constexpr non_move_assign_able(const non_move_assign_able&)noexcept=default;
	constexpr non_move_assign_able(non_move_assign_able&&)noexcept=default;
	constexpr non_move_assign_able& operator=(const non_move_assign_able&)&noexcept=default;
	constexpr non_move_assign_able& operator=(non_move_assign_able&&)&noexcept=delete;
};

/// 禁止默认合成的赋值
struct non_default_assign_able:non_copy_assign_able,non_move_assign_able{};


/// 禁止复制构造
struct non_copy_construct_able{
	constexpr non_copy_construct_able()=default;
	constexpr non_copy_construct_able(const non_copy_construct_able&)=delete;
	constexpr non_copy_construct_able(non_copy_construct_able&&)=default;
};
/// 禁止移动构造
struct non_move_construct_able{
	constexpr non_move_construct_able()=default;
	constexpr non_move_construct_able(const non_move_construct_able&)=default;
	constexpr non_move_construct_able(non_move_construct_able&&)=delete;
};

/// 禁止默认合成的构造
struct non_default_construct_able:non_copy_construct_able,non_move_construct_able{
	constexpr non_default_construct_able()=delete;
};

/// 禁止默认合成的复制
struct non_copyable:non_copy_assign_able,non_copy_construct_able{};
/// 禁止默认合成的移动
struct non_moveable:non_move_assign_able,non_move_construct_able{};

//file_end

