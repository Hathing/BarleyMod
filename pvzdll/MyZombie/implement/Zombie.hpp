#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Zombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if (zombie.FromWave == WAVE_ELITE2)
			{
				zombie.ReanimShowPrefix((const char*)0x668EC8, -1);
				zombie.ReanimShowPrefix((const char*)0x65851C, -1);
				zombie.ReanimShowPrefix((const char*)0x66EBA0, -1);
				zombie.ReanimShowPrefix((const char*)0x66EB94, -1);

				auto model = zombie.GetAnimation();
				auto track = model.GetTrackInstance("Zombie_body");

				auto head = Creator::CreateReanimation(AnimationType::Sunflower, 0, 0, 0);
				head.AssignRenderGroupToPrefix(-1, "awake");
				head.AssignRenderGroupToPrefix(-1, "easter");
				head.Play((const char*)0x6580D4, 0, 0, 15.0f);

				auto effect = head.AttachTo(track.GetAttachmentID(), 0, 0);
				effect.GetOffset().ScaleRotateTransformMatrix(63.0f, 22.0f, 0.2f, -0.8f, 0.8f);
			}
		}
	};
}