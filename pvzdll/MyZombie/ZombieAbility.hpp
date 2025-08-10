#include "MyZombie.hpp"

namespace ZombieAbility
{
	class BaseZombie
	{
	public:
		/// @brief 僵尸完成原版的初始化过程后，执行此函数。
		/// @param zombie 僵尸
		virtual void onCreated(MyZombie zombie)
		{
			return;
		}
	};
	typedef BaseZombie* ZombiePTR;
	ZombiePTR GetAbility(ZombieType::ZombieType type);
}
