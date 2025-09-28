#pragma once
#include "MyBoard.hpp"
#include "MyPlant/MyPlant.hpp"
#include "MyProjectile/MyProjectile.hpp"
#include "MyZombie/MyZombie.hpp"

typedef PVZEvent::PlantDamageZombieEvent::PZDamageInfo<MyPlant, MyZombie> PZDamageEvent;

void onPlantDamageZombie(PZDamageEvent* info);

namespace PVZ
{
	template<typename T>
	T GetByID(int id)
	{
		if (id)
		{
			T tmp = T(ID_INDEX(id));
			if (tmp.Id != id)
				return T(INVALID_BASEADDRESS);
			else
				return tmp;
		}
		else
			return T(INVALID_BASEADDRESS);
	}
	template<typename T>
	T GetByID(unsigned int id)
	{
		if (id)
		{
			T tmp = T(ID_INDEX(id));
			if (tmp.Id != id)
				return T(INVALID_BASEADDRESS);
			else
				return tmp;
		}
		else
			return T(INVALID_BASEADDRESS);
	}

	void ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags = PVZ::DAMAGEF_NONE);

	class ZombieDrawPosition
	{
	public:
		int HeadX;
		int HeadY;
		int ArmY;
		float BodyY;
		float ImageOffsetX;
		float ImageOffsetY;
		float ClipHeight;
	};
}

class MyAttachEffect : public PVZ::AttachEffect
{
public:
	MyAttachEffect(int address) : PVZ::AttachEffect(address) {};
	MyAttachEffect(const PVZ::AttachEffect& attacheffect) : PVZ::AttachEffect(attacheffect.GetBaseAddress()) {};
};

class MyAttachment : public PVZ::Attachment
{
public:
	MyAttachment(int address) : PVZ::Attachment(address) {};
	MyAttachment(const PVZ::Attachment& attachment) : PVZ::Attachment(attachment.GetBaseAddress()) {};

	INT_PROPERTY(mNumEffects, __get_mNumEffects, __set_mNumEffects, 0x300);
};