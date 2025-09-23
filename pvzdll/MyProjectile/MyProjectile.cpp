#include "MyProjectile.hpp"
#include "implement/index.hpp"

namespace ProjectileAbility
{
	ProjectilePTR pt_factory[] =
	{
		new BaseProjectile(), new BaseProjectile(), new Cabbage(),		  new Melon(),			new Puff(),
		new WinterMelon(),	  new Fireball(),		new Star(),			  new BaseProjectile(), new BaseProjectile(),
		new Kernel(),		  new Cobbig(),			new Butter(),		  new BaseProjectile(), new BaseProjectile(),
		new GoldSpike(),	  new BaseProjectile(), new BaseProjectile(), new Diamond(),		new BaseProjectile(),
		new BaseProjectile(), new BaseProjectile(), new GoldMelon(),	  new BaseProjectile(),	new BaseProjectile(),
	};
}

ProjectileAbility::ProjectilePTR ProjectileAbility::GetAbility(ProjectileType::ProjectileType type)
{
	return ProjectileAbility::pt_factory[type];
}

byte __asm__AdjustRow[37]
{
	MOV_EBX(0),
	MOV_EAX(0),
	MOV_ECX(0),
	MOV_EDX(0),
	INVOKE(0x41C550),
	0x89,0x43,0x1C,//mov [ebx+1c],eax
	RET
};

void MyProjectile::AdjustRow()
{
	SETARG(__asm__AdjustRow, 1) = this->GetBaseAddress();
	SETARG(__asm__AdjustRow, 6) = this->ImageX;
	SETARG(__asm__AdjustRow, 11) = this->ImageY + 40;//40照抄的三线的CT
	SETARG(__asm__AdjustRow, 16) = this->GetBoard().GetBaseAddress();
	PVZ::Memory::Execute(STRING(__asm__AdjustRow));
}