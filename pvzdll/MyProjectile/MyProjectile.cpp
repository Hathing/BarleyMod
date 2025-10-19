#include "MyProjectile.hpp"
#include "implement/index.hpp"

namespace ProjectileAbility
{
	ProjectilePTR pt_factory[] =
	{
		new Pea(),			  new BaseProjectile(), new Cabbage(),		  new Melon(),			new Puff(),
		new WinterMelon(),	  new Fireball(),		new Star(),			  new BaseProjectile(), new BaseProjectile(),
		new Kernel(),		  new Cobbig(),			new Butter(),		  new ZombiePea(),		new BaseProjectile(),
		new GoldSpike(),	  new BaseProjectile(), new BaseProjectile(), new Diamond(),		new BaseProjectile(),
		new BaseProjectile(), new BaseProjectile(), new GoldMelon(),	  new BaseProjectile(),	new BaseProjectile(),
	};
}

ProjectileAbility::ProjectilePTR ProjectileAbility::GetAbility(ProjectileType::ProjectileType type)
{
	return ProjectileAbility::pt_factory[type];
}

PVZ::DamageFlags MyProjectile::GetDamageFlags(int zombie_addr)
{
	return static_cast<PVZ::DamageFlags>(PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, zombie_addr)
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x46D230)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret())
	);
}

byte __asm__AdjustRow[37]
{
	MOV_ESI(0),
	MOV_EAX(0),
	MOV_EDI(0),
	MOV_EBX(0),
	INVOKE(0x41C650),
	0x89,0x46,0x1C,//mov [esi+1c],eax
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



void MyProjectile::MakePiercing(int piercingnum,float xspeed,float yspeed)
{
	// 在Projectile上DataArrayAlloc
	// 有bug，干脆直接创建个正常子弹算了
	// int ghostaddr = PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EDI, PVZ::Memory::ReadPointer(this->GetBaseAddress()+0x04,0xC8)).invoke(0x41DF60).mov_mem_reg(PVZ::Memory::Variable,REG_EAX).ret());
	int ghostaddr = PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, this->GetBoard().GetBaseAddress()).push_imm32(0).push_imm32(0).push_imm32(0).push_imm32(0).push_imm32(0).invoke(0x40D620).mov_mem_reg(PVZ::Memory::Variable, REG_EAX).ret());
	MyProjectile ghostproj{ ghostaddr };
	ghostproj.IsGhost = true;
	ghostproj.AttachmentID = 0;

	ghostproj.PiercingID1 = 0;
	ghostproj.PiercingID2 = 0;
	ghostproj.PiercingID3 = 0;
	ghostproj.PiercingID4 = 0;
	ghostproj.PiercingID5 = 0;
	ghostproj.PiercingID6 = 0;
	ghostproj.PiercingID7 = 0;
	ghostproj.PiercingID8 = 0;
	ghostproj.PiercingMaxCount = max(min(piercingnum, 8), 1);//穿透数不能<1，不能>8
	ghostproj.PiercingCount = 0;

	this->GhostID = ghostproj.Id;
	this->Motion = MotionType::Piercing;
	this->XSpeed = xspeed;
	this->YSpeed = yspeed;
}

void MyProjectile::DeriveProperty(MyProjectile proj)
{
	this->Row = proj.Row;
	this->DamageAbility = proj.DamageAbility;
	this->SourceType = proj.SourceType;
	this->SourceLevel = proj.SourceLevel;
	this->ParentID = proj.ParentID;
}