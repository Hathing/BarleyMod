#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class CatapultZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 650;
			zombie.BodyMaxHealth = 650;
			zombie.SummonCounter = 3;

			if (zombie.FromWave == WAVE_ELITE1)
			{
				zombie.ReanimShowPrefix("Zombie_catapult_driver_mouth", -1);
				zombie.ReanimShowPrefix("Zombie_catapult_driver_head", -1);
	
				auto model = zombie.GetAnimation();
				auto track = model.GetTrackInstance("Zombie_catapult_driver_body");

				auto head = Creator::CreateReanimation(AnimationType::AnimationType(151), 0, 0, 0);
				head.AssignRenderGroupToPrefix(-1, "awake");
				head.AssignRenderGroupToPrefix(-1, "easter");
				head.Play("anim_idle", 0, 0, 15.0f);

				auto effect = head.AttachTo(track.GetAttachmentID(), 0, 0);
				effect.GetOffset().ScaleRotateTransformMatrix(80.0f, 0, 0.2f, -0.8f, 0.8f);

				zombie.InvulnerableDuration = 9999999;
			}
		}
	};
}
