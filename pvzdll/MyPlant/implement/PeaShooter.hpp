#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class PeaShooter : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 50;//这是索敌的间隔，不是实际用于重置攻击的间隔
		}

		bool TickAbility(MyPlant plant)
		{
			//真正的索敌函数
			if (!plant.NotExist && !plant.Squash)
			{
				int row = plant.Row;
				int threat_p = INT_MIN, threat_z = 0;//p为外部保存的威胁度，z为遍历时计算的威胁度
				auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
				int targetid = 0;
				for (auto& zombie : zombies)
				{
					//无法被索敌的条件
					if (zombie.Row != row)continue;
					//计算威胁度
					threat_z = 0;
					threat_z += zombie.BodyHealth;
					threat_z += zombie.HelmHealth;
					threat_z += zombie.ShieldHealth;
					//比较威胁度，更新目标
					if (threat_z > threat_p)
					{
						threat_p = threat_z;
						targetid = zombie.Id;
					}
				}
				plant.PeashooterTarget = targetid;
			}
			return true;
		}
	};
}
