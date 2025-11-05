#pragma once
#include "../PlantAbility.hpp"

static byte __asm_animate[]
{
	MOV_EDI(0),
	INVOKE(0x464480),
	RET
};

namespace PlantAbility
{
	class Explode_O_Nut : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				auto model = plant.GetAnimationPart1();
				if (model.isValid())
				{
					if (plant.Hp * 3 < plant.MaxHp)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF048));
					else if (plant.Hp * 3 <= plant.MaxHp * 2)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF044));
					else
						model.SetImageOverride("awaken_kuijia", PVZ::Image(0));
				}
			}
			else
			{
				SETARG(__asm_animate, 1) = plant.GetBaseAddress();
				PVZ::Memory::Execute(STRING(__asm_animate));
			}

			return false;
		}
		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_ALL);
		}
		void onDeath(MyPlant plant, PlantDyingType dyingtype)
		{
			//创建爆炸
			Creator::CreateLowerSound(LowerSoundType::CherryExplode);
			Creator::CreateLowerSound(LowerSoundType::Juice);
			PVZ::CreateParticleSystem(plant.ImageX + 40, plant.ImageY + 50, 0x61A80, EffectType::CHERRY_BOMB_EXPLODED);
			MyBoard board = plant.GetBoard();
			
			Creator::CreateExplosion(plant.ImageX, plant.ImageY, 115, 1, 0);

			if (plant.Level > 0)
			{
				if (plant.Level >= 5 && plant.AttributeCountdown <= 0)
				{
					plant.AttributeCountdown = 18000;//3分钟CD
					//辣椒爆炸
					Creator::CreateLowerSound(LowerSoundType::JalapenoExplode);
					PVZ::Memory::Execute(AsmBuilder()
						.push_imm32(plant.Row)
						.mov_reg_imm(REG_EBX, board.GetBaseAddress())
						.invoke(0x41D450).ret());
					PVZ::Memory::Execute(AsmBuilder()
						.push_imm32(plant.Row)
						.mov_reg_imm(REG_EDI, plant.GetBaseAddress())
						.invoke(0x4664B0).ret());
					board.GetIcetrace().SetDisappearCountdown(plant.Row, 20);
				}

				//回满血
				PlantAbility::GetAbility(plant.Type)->onCreated(plant);
				plant.HpDisplayCounter = 100;
				//重置状态
				//plant.BloverDisappearCountdown = 200;
				//plant.Squash = false;
				//等级降低
				plant.Level--;
				plant.Experience = (plant.Level <= 0 ? 0 : PlantAbility::PLANT_LEVEL_EXP[plant.Type][plant.Level]);
			}
		}
	};
}
