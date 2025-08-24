#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Barley : public BasePlant
	{
		bool IsXPRecipient(MyPlant plant)
		{
			return false;
		}
		bool TickAbility(MyPlant plant)
		{
			plant.BarleyCounter++;
			if (plant.BarleyCounter == 3010)
			{
				plant.Remove();

				SeedType::SeedType type;
				while (true)
				{
					type = (SeedType::SeedType)Creator::Rand(80);

					switch (type)
					{
					case SeedType::Barley:
					case SeedType::CherryBomb:
					case SeedType::GraveBuster:
					case SeedType::Doomshroom:
					case SeedType::Jalapeno:
					case SeedType::TangleKelp:
					case SeedType::Spickweed:
					case SeedType::Torchwood:
					case SeedType::Seashroom:
					case SeedType::Pumpkin:
					case SeedType::FlowerPot:
					case SeedType::CoffeeBean:
					case SeedType::CobCannon:
					case SeedType::Imitater:
					case SeedType::GiantWallnut:
					case 53:
					case SeedType::Refresh:
					case SeedType::Crater:
					case SeedType::Sun:
					case SeedType::Diamond:
					case SeedType::SnorkedZombie:
					case SeedType::Trophy:
					case SeedType::Zombie:
					case SeedType::ConeheadZombie:
					case SeedType::PoleVaultingZombie:
					case SeedType::BucketheadZombie:
					case SeedType::LadderZombie:
					case SeedType::DiggerZombie:
					case SeedType::BungeeZombie:
					case SeedType::FootballZombie:
					case SeedType::BalloonZombie:
					case SeedType::ScreenDoorZombie:
					case SeedType::Zomboin:
					case SeedType::PogoZombie:
					case SeedType::DancingZombie:
					case SeedType::Gigagargantuar:
					case SeedType::Imp:

					case SeedType::Peashooter:
					case SeedType::Iceshroom:
					case SeedType::DiamondShroom:
					case SeedType::Explodenut:
					case SeedType::UmbrellaLeaf:
					case SeedType::SpringEquinoxVine:
					case SeedType::ThymeWarp:
						continue;
					default:
						break;
					}
				}

				MyPlant creep = Creator::CreatePlant(type, plant.Row, plant.Column);
				if (creep.Type == SeedType::RestingBarley)
					Creator::CreatePlant(SeedType::Jalapeno, plant.Row, plant.Column).CreateEffect();
				else
				{
					creep.FromBarley = true;
					creep.Level = Creator::Rand(1 + MyPlant::MAX_LEVEL);
				}

				PVZ::CreateParticleSystem(plant.ImageX + 40.0f, plant.ImageY + 40.0f, 0x618A0, EffectType::IMITATER_TRANSFORMING);
			}

			return true;
		}
	};
}
