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
	};
}
