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
				int threat_p = INT_MIN, threat_z = 0;//p为外部保存的威胁度，z为遍历时计算的威胁度
				auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
				int targetid = 0;
				for (auto& zombie : zombies)
				{
					//无法被索敌的条件
					if (!CanTargetZombie(plant, zombie))continue;
					//计算威胁度
					threat_z = 0;
					threat_z += zombie.BodyHealth;
					threat_z += zombie.HelmHealth;
					threat_z += zombie.ShieldHealth;
					//比较威胁度，更新目标
					if (threat_z > threat_p)
					{
						threat_p = threat_z;
						targetid = zombie.GetBaseAddress();
					}
				}
				plant.PeashooterTarget = targetid;
			}
			return true;
		}

		bool onUpdateShooting(MyPlant plant)
		{
			if (plant.ShootingCountdown == 1)
			{
				int targetid = plant.PeashooterTarget;
				if (targetid != 0)
				{
					plant.Fire(0, targetid);
					plant.ShootOrProductCountdown = 300;//真正的重置CD
				}
			}
			plant.ShootingCountdown -= 1;
			if (plant.ShootingCountdown == 0)
			{
				auto anim1 = plant.GetAnimationPart1();
				auto anim2 = plant.GetAnimationPart2();
				if (anim2.isValid() && plant.ShootOrProductInterval > 0)
				{
					StartBlend(20, anim2);
					anim2.SetFramesForLayer("anim_head_idle");
					int base_addr = anim2.GetBaseAddress();
					Memory::WriteMemoryUnsafe<int>((DWORD)base_addr + 0x10, 0);
					anim2.CycleRate = anim1.CycleRate;
					anim2.Speed = anim1.Speed;
				}
				else if (anim1.isValid() && plant.ShootOrProductInterval > 0)
				{
					plant.PlayIdleAnim(anim1.Speed);
				}
				else plant.ShootingCountdown = 1;
			}
			return false;
		}

		int onFindTargetRT(MyPlant plant, MyZombie zombie, int row)
		{
			return CanTargetZombie(plant, zombie) ? 1 : 0;
		}

		bool onAddProjectileBefore(MyPlant plant, ProjectileType::ProjectileType proj_type, int x, int y)
		{
			MyZombie zombie{ plant.PeashooterTarget };
			//TakeDamage(500, PVZ::DAMAGEF_NONE, plant.PeashooterTarget);
			//zombie.LastDamageSourceID = plant.GetBaseAddress();//这行代码会崩溃
			zombie.Hit(500, PVZ::DAMAGEF_NONE);
			//创建特效
			auto particle = PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::ZOMBIE_GET_KERNEL_SHOT);
			//particle.OverrideImage(PVZ::Image(0x6A76A8));//这行代码会崩溃
			PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::HAMMER_BANG);
			Creator::CreateLowerSound(LowerSoundType::CherryExplode);
			return false;
		}

		static bool CanTargetZombie(MyPlant plant, MyZombie zombie)
		{
			ZombieType::ZombieType zombietype = zombie.Type;
			ZombieState::ZombieState zombiestate = zombie.State;
			if (zombie.NotExist || zombie.ZombieHeight == 9 || zombie.Hypnotized || zombie.Blowaway || !zombie.NotDying)//这里+64是ZombieHeight？我看指针表是僵尸运动状态
				return false;
			if (zombietype == ZombieType::Zomboin || zombietype == ZombieType::CatapultZombie)
			{
				if (zombie.BodyHealth < 30)
					return false;
			}
			else if (!zombie.NotDying)
				return false;
			if (zombietype == ZombieType::BungeeZombie)
				return false;
			switch (zombiestate)
			{
			case ZombieState::DYING:
			case ZombieState::DYING_FROM_INSTANT_KILL:
			case ZombieState::DYING_FROM_LAWNMOWER:
			case ZombieState::NEWSPAPER_DESTORYED:
			case ZombieState::DIGGER_DIG:
			case ZombieState::DIGGER_LOST_DIG:
			case ZombieState::DIGGER_IDLE:
			case ZombieState::SNORKEL_SWIM:
				return false;
			default:
				break;
			}
			if (zombie.Row != plant.Row)
				return false;
			if (zombie.ImageX < plant.ImageX || zombie.ImageX > 660)
				return false;
			return true;
		}
	};
}
