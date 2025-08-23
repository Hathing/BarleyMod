#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SeaShroom : public BasePlant
	{
		void TransferInfo(MyPlant primary, MyPlant secondary)
		{
			secondary.AnotherCounter = primary.AnotherCounter;
			secondary.RelatedPlantID1 = primary.RelatedPlantID1;
			secondary.RelatedPlantID2 = primary.RelatedPlantID2;
			secondary.RelatedPlantID3 = primary.RelatedPlantID3;
			secondary.RelatedPlantID4 = primary.RelatedPlantID4;
			secondary.OwnerID = 0;
		}

		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 550;
			rect->Height = plant.Height;
			return true;
		}
		bool TickAbility(MyPlant plant)
		{
			static const int cooldown[6] = {12000, 12000, 9000, 9000, 6000, 6000};
			static const int max_cnt[6] = {1, 2, 2, 3, 3, 3};

			if (plant.OnBoard && !plant.OwnerID)
			{
				plant.AnotherCounter++;
				int lvl = min(MyPlant::MAX_LEVEL, plant.Level);
				if (plant.AnotherCounter >= cooldown[lvl])
				{
					// subindex 是否被使用
					static bool idx_used[5] = { false, false, false, false, false };
					// 对应编号的 RelatedPlantID 是否被使用
					static bool pos_used[5] = { true, false, false, false, false };
					idx_used[plant.SubIndex] = true;

					auto creep = MyPlant::GetByID(plant.RelatedPlantID1);
					if (creep.isValid())
					{
						idx_used[creep.SubIndex] = true;
						pos_used[1] = true;
					}

					creep = MyPlant::GetByID(plant.RelatedPlantID2);
					if (creep.isValid())
					{
						idx_used[creep.SubIndex] = true;
						pos_used[2] = true;
					}

					creep = MyPlant::GetByID(plant.RelatedPlantID3);
					if (creep.isValid())
					{
						idx_used[creep.SubIndex] = true;
						pos_used[3] = true;
					}

					creep = MyPlant::GetByID(plant.RelatedPlantID4);
					if (creep.isValid())
					{
						idx_used[creep.SubIndex] = true;
						pos_used[4] = true;
					}

					int cnt = 0;
					for (int i = 0; i < sizeof(idx_used); i++)
						if (idx_used[i])
							cnt++;

					if (cnt >= max_cnt[lvl])
						plant.AnotherCounter--;
					else
					{
						static const int x_delta[5] = {0, 24, -14, -26, 18};
						static const int y_delta[5] = {0, -12, -21, 14, 12};
						static const int layer_delta[5] = {0, -24, -36, 10, 10};

						plant.AnotherCounter = 0;
						MyPlant newcreep = Creator::CreatePlant(SeedType::Seashroom, plant.Row, plant.Column);
						newcreep.Experience = plant.Experience;
						newcreep.Level = plant.Level;
						newcreep.OwnerID = plant.Id;

						for (int i = 1; i <= 4; i++)
							if (!idx_used[i])
								newcreep.SubIndex = i;

						int nxt_pos = 0;
						for (int i = 1; i <= 4; i++)
							if (!pos_used[i])
							{
								nxt_pos = i;
								break;
							}

						switch (nxt_pos)
						{
						case 1: plant.RelatedPlantID1 = newcreep.Id; break;
						case 2: plant.RelatedPlantID2 = newcreep.Id; break;
						case 3: plant.RelatedPlantID3 = newcreep.Id; break;
						case 4: plant.RelatedPlantID4 = newcreep.Id; break;
						}
						newcreep.ImageX += x_delta[nxt_pos];
						newcreep.ImageY += y_delta[nxt_pos];
						newcreep.Layer += layer_delta[nxt_pos];
					}
				}
			}

			return false;
		}
		void onDie(MyPlant plant)
		{
			if (plant.OnBoard && !plant.OwnerID)
			{
				auto creep = MyPlant::GetByID(plant.RelatedPlantID1);
				if (creep.isValid())
				{
					plant.RelatedPlantID1 = 0;
					TransferInfo(plant, creep);
					return;
				}

				creep = MyPlant::GetByID(plant.RelatedPlantID2);
				if (creep.isValid())
				{
					plant.RelatedPlantID2 = 0;
					TransferInfo(plant, creep);
					return;
				}

				creep = MyPlant::GetByID(plant.RelatedPlantID3);
				if (creep.isValid())
				{
					plant.RelatedPlantID3 = 0;
					TransferInfo(plant, creep);
					return;
				}

				creep = MyPlant::GetByID(plant.RelatedPlantID4);
				if (creep.isValid())
				{
					plant.RelatedPlantID4 = 0;
					TransferInfo(plant, creep);
					return;
				}
			}
		}
		void onGainXP(MyPlant plant, int val, bool kill_credit)
		{
			if (plant.IsPrime())
			{
				auto creep = MyPlant::GetByID(plant.RelatedPlantID1);
				if (creep.isValid())
					creep.AddExperience(val);

				creep = MyPlant::GetByID(plant.RelatedPlantID2);
				if (creep.isValid())
					creep.AddExperience(val);

				creep = MyPlant::GetByID(plant.RelatedPlantID3);
				if (creep.isValid())
					creep.AddExperience(val);

				creep = MyPlant::GetByID(plant.RelatedPlantID4);
				if (creep.isValid())
					creep.AddExperience(val);
			}
			return;
		}
	};
}
