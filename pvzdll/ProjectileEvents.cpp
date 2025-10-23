#include "pch.h"
#include "MyProjectile/ProjectileAbility.hpp"
#include "MyPlant/PlantAbility.hpp"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
{
	plant.InitAddProjectile(proj);
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = (byte)plant.Row;
	return PlantAbility::GetAbility(plant.Type)->onAddProjectile(plant, proj, zombie, PlantWeapon);
}

int onProjDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType type, int subtarget_num, int damage)
{
	int mydamage = ProjectileAbility::GetAbility(proj.Type)->OverrideDamage(proj, zombie, type, subtarget_num, damage);
	if (mydamage == 0)
		return 0;

	if (!zombie.NotExist)
	{
		auto plant = MyPlant::GetByID(proj.ParentID);
		if (plant.isValid())
		{
			//植物伤害僵尸事件
			auto event = new PZDamageEvent(zombie, plant, proj.GetDamageFlags(zombie.GetBaseAddress()), mydamage, PVZEvent::PLANTDAMAGETYPE_PROJECTILE);
			onPlantDamageZombie(event);
			mydamage = event->damage;
			delete event;
			/*
			if (type == PVZEvent::ProjDmgType::DAMAGE_SINGULAR)
				zombie.LastDamageSourceID = caster.Id;
			else
			{
				if (caster.isValid() && caster.Row == zombie.Row)
					zombie.LastDamageSourceID = caster.Id;
			}
			*/
		}
		ProjectileAbility::GetAbility(proj.Type)->onDamageZombie(proj, zombie, type, subtarget_num, mydamage);
	}

	return mydamage;
}

int GetProjectileImage(MyProjectile proj, PVZEvent::ProjectileImgParam param)
{
	if (param == PVZEvent::PROJECTILE_IMAGEROW)
		return 0;
	else
		return ProjectileAbility::GetAbility(proj.Type)->GetImage(proj);
}

float GetProjectileImageSize(MyProjectile proj, float original_val)
{
	return ProjectileAbility::GetAbility(proj.Type)->GetImageSize(proj,original_val);
}

void onProjectileUpdate(MyProjectile proj)
{
	return;
}

bool onProjectileRemove(MyProjectile proj)
{
	// 幽灵子弹直接Remove无需多言
	if (proj.IsGhost)
		return false;
	// 处理弹跳子弹
	if (proj.Motion == MotionType::Throw && proj.BounceCount > 0 && proj.X > 0.0f && proj.X < 1000.0f)
	{
		proj.BounceCount -= 1;
		proj.XSpeed *= 0.45f;
		proj.HeightSpeed *= -0.75f;
		return true;
	}
	// 处理穿透子弹
	if (proj.Motion == MotionType::Piercing && proj.GhostID != 0)
	{
		MyProjectile ghost = PVZ::GetByID<MyProjectile>(proj.GhostID);
		ghost.PiercingCount += 1;
		if (ghost.PiercingCount < ghost.PiercingMaxCount)
			return true;
		ghost.Remove();
	}
	return false;
}

bool onProjectileSlideMotion(MyProjectile proj)
{
	//static constexpr int CYCLE_TIME = 50;
	if (proj.SourceType == 18)//三线射手
	{
		proj.YSpeed -= 0.001f * (proj.Y - proj.OriginalY);//简谐运动，对于a = kx,周期T= 2*pi/sqrt(k)
		proj.Y += proj.YSpeed;
		proj.AdjustRow();
		//之前的伪正弦运动，本质上是两段指数运动
		/*
		if (proj.SpecialFlags == PSF_THREEPEATER_SLIDE_OUT)
		{
			proj.YSpeed *= 0.97f;
			if (proj.ExistedTime % CYCLE_TIME == CYCLE_TIME - 1)
			{
				proj.YSpeed *= -1.0f;
				proj.SpecialFlags = PSF_THREEPEATER_SLIDE_IN;
			}
		}
		else if (proj.SpecialFlags == PSF_THREEPEATER_SLIDE_IN)
		{
			proj.YSpeed /= 0.97f;
			if (proj.ExistedTime % CYCLE_TIME == CYCLE_TIME - 1)
			{
				proj.YSpeed *= -1.0f;
				proj.SpecialFlags = PSF_THREEPEATER_SLIDE_OUT;
			}
		}
		*/
		proj.ShadowY += proj.YSpeed;
		return false;
	}
	return true;
}

void onProjectileInitAfter(MyProjectile proj)
{
	proj.ParentID = 0;
	proj.SpecialFlags = 0;
	proj.SpecialStack = 0;
	proj.SpecialType = PST_NONE;
	proj.SourceLevel = 0;
	proj.SourceType = 0;
	proj.BounceCount = 0;

	proj.IsGhost = false;

	proj.Unknown = 0;
}

void onFireballInitColor(MyProjectile proj, PVZ::Animation anim)
{
	switch (proj.SpecialType)
	{
	case PST_ORANGE_FIREBALL:
		anim.SetColor({ 0xFF,0x70,0x20,0xFF });
		break;
	case PST_RED_FIREBALL:
		anim.SetColor({ 0xFF,0x10,0x10,0xFF });
		break;
	case PST_BLUE_FIREBALL:
		anim.SetColor({ 0x00,0x00,0xFF,0xFF });
		break;
	case PST_PURPLE_FIREBALL:
		anim.SetColor({ 0x70,0x00,0xFF,0xFF });
		break;
	default:
		break;
	}
}

bool IsProjExpire(MyProjectile proj)
{
	if (proj.Motion == MotionType::ShortDirect && proj.ExistedTime >= 175)
		return true;
	if (proj.Motion == MotionType::Piercing && proj.ExistedTime >= 30)
		return true;
	return false;
}

void onPlantAddProjDamageRangeFlags(MyProjectile proj, MyPlant plant)
{
	if (proj.Type == ProjectileType::WinterMelon && proj.SpecialType == PST_CANNON_WINTERMELON)
		proj.DamageAbility = 0;
}

bool onProjectileImpact(MyProjectile proj, MyZombie zombie)
{
	if (zombie.isValid())
	{
		if (zombie.Type == ZombieType::WallnutZombie && zombie.VariantType == ZombieVariantType::UmbrellaHead && Creator::Rand(2))
		{
			if (proj.Motion == MotionType::Direct)
			{
				proj.Motion = MotionType::Throw;
				proj.XSpeed = -3.0f;
				proj.HeightSpeed = -5.0f;
				proj.HeightAcceleration = 0.15f;
			}
			if (proj.Motion == MotionType::Throw)
			{
				proj.XSpeed = -proj.XSpeed;
				proj.HeightSpeed *= -0.8f;
			}

			int dist = zombie.ImageX + PVZ::Memory::ReadMemory<int>(zombie.GetBaseAddress() + 0x8C)
				- proj.ImageX - proj.Width;
			if (dist > 0)
			{
				dist--;
				proj.X += dist;
				float tmp = dist / proj.XSpeed * proj.HeightSpeed + proj.Height;
				proj.Height = tmp;
				proj.ImageY = tmp + proj.Y;
			}

			zombie.JustGotShotCounter = 20;
			Creator::CreateSound(LowerSoundType::ConeheadHit);
			return false;
		}
	}
	//注意，zombie*可能=0，因此一般子弹先要检查zombie是否isValid
	//冰瓜大炮不命中僵尸，也不直接对僵尸操作，因此无需判定
	if (proj.Type == ProjectileType::WinterMelon && proj.SpecialType == PST_CANNON_WINTERMELON)
	{
		auto zombies = proj.GetBoard().GetAllZombies<MyZombie>();
		for (auto& zombie_ : zombies)
		{
			if (zombie_.Row == proj.Row && zombie_.EffectedBy(PVZ::DRF_GROUND | PVZ::DRF_DYING | PVZ::DRF_SUBMERGED) && (zombie_.X<proj.X + 100 && zombie_.X >proj.X - 100))
			{
				int dmg = onProjDamageZombie(proj, zombie_, PVZEvent::DAMAGE_SINGULAR, 0, 0);
				zombie_.Hit(dmg, PVZ::DAMAGEF_HITS_SHIELD_AND_BODY);
			}
		}
		//创建音效和特效
		Creator::CreateLowerSound(LowerSoundType::DiamondDrop);
		Creator::CreateLowerSound(LowerSoundType::Decelerate);
		auto effect1 = PVZ::CreateParticleSystem(proj.X - 60.0f, proj.Y + 100.0f, proj.Layer + 100, EffectType::ZOMBOIN_EXPLODED2);
		effect1.OverrideImage(PVZ::Image(Memory::ReadMemory<DWORD>(0x6A72F4)));
		effect1.OverrideScale(2.0f);
		auto effect2 = PVZ::CreateParticleSystem(proj.X - 60.0f, proj.Y + 100.0f, proj.Layer + 100, EffectType::ZOMBOIN_EXPLODED2);
		effect2.OverrideImage(PVZ::Image(Memory::ReadMemory<DWORD>(0x6A764C)));
		effect2.OverrideScale(2.0f);
		PVZ::CreateParticleSystem(proj.X, proj.Y, proj.Layer + 100, EffectType::ICE_SHROOM_EXPLODED);
	}
	if (proj.Motion == MotionType::Piercing && proj.GhostID!=0 && zombie.isValid())
	{
		MyProjectile ghost = PVZ::GetByID<MyProjectile>(proj.GhostID);
		ghost.SetPiercingID(ghost.PiercingCount, zombie.Id);
	}
	return ProjectileAbility::GetAbility(proj.Type)->onImpact(proj, zombie);
}


bool onProjectileSkipUpdateAndDraw(MyProjectile proj)
{
	if (proj.IsGhost)
		return false;
	return true;
}

bool onProjectileFindZombieTargetSkip(MyProjectile proj, MyZombie zombie)
{
	if (proj.Motion == MotionType::Piercing && proj.GhostID != 0)
	{
		MyProjectile ghost = PVZ::GetByID<MyProjectile>(proj.GhostID);
		for (int i = 0; i < ghost.PiercingCount; i++)
		{
			if (ghost.GetPiercingID(i) == zombie.Id)
			{
				return false;
			}
		}
	}
	return true;
}

bool onProjectileFindPlantTargetSkip(MyProjectile proj, MyPlant plant)
{
	if (proj.SpecialType == PST_LADDER_ZOMBIEPEA)
	{
		if (plant.OwnerID != 0)
			return false;

		MyBoard board = proj.GetBoard();
		auto griditems = board.GetAllGriditems<PVZ::Griditem>();
		for (auto& griditem : griditems)
		{
			if (griditem.Type == GriditemType::GriditemLadder && griditem.Row == plant.Row && griditem.Column == plant.Column)
				return false;
		}
	}
	return true;
}

bool onProjectileUpdatePiercingMotion(MyProjectile proj)
{
	if (proj.Motion == MotionType::Piercing)
	{
		return false;
	}
	return true;
}

bool onProjectileUpdateLeftMotion(MyProjectile proj)
{
	if (proj.X <= 10.0f && proj.Type != ProjectileType::ZombiePea)
	{
		proj.Motion = MotionType::Direct;
		/*
		* 尝试让火球反向失败，代码先放这吧
		if(proj.Type==ProjectileType::FirePea)
		{
			MyAttachment attachment = PVZ::GetByID<PVZ::Attachment>(proj.AttachmentID);
			if (attachment.isValid())
			{
				MyAttachEffect attacheffect{ attachment.GetBaseAddress()};
				attacheffect.GetOffset().ScaleRotateTransformMatrix(0, 0, 0, -1.0f, 1.0f);
			}
		}
		*/
	}
	return true;
}

bool onProjectileDivert(MyProjectile proj)
{
	return proj.Type == ProjectileType::ZombiePea && (proj.ExistedTime & 1);
}

void InitProjectileEvents()
{
	// 子弹初始化与销毁相关
	PVZEvent::ProjectileInitAfterEvent((int)onProjectileInitAfter);
	ProjectileRemoveEvent((int)onProjectileRemove);

	// 子弹总更新与绘制相关
	PVZEvent::ProjectileSkipUpdateAndDrawEvent((int)onProjectileSkipUpdateAndDraw);
	PVZEvent::ProjectileUpdateEvent((int)onProjectileUpdate);
	PVZEvent::ProjectileImageEvent((int)GetProjectileImage);
	PVZEvent::ProjectileImageSizeEvent((int)GetProjectileImageSize);
	PVZEvent::FireballInitColorEvent((int)onFireballInitColor);

	// 子弹创建相关
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::PlantAddProjDamageRangeFlagsEvent((int)onPlantAddProjDamageRangeFlags);

	// 子弹碰撞相关
	PVZEvent::ProjectileCheckExpireEvent((int)IsProjExpire);
	PVZEvent::ProjectileHitDiversionEvent((int)onProjectileDivert);
	PVZEvent::ProjectileFindZombieTargetSkipEvent((int)onProjectileFindZombieTargetSkip);
	PVZEvent::ProjectileFindPlantTargetSkipEvent((int)onProjectileFindPlantTargetSkip);

	// 子弹碰撞效果相关
	PVZEvent::ProjectileImpactEvent((int)onProjectileImpact);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);

	// 子弹运动相关
	PVZEvent::ProjectileUpdatePiercingMotionEvent((int)onProjectileUpdatePiercingMotion);
	PVZEvent::ProjectileSlideMotionEvent((int)onProjectileSlideMotion);
	PVZEvent::ProjectileUpdateLeftMotionEvent((int)onProjectileUpdateLeftMotion);

	//冰豌豆和冰瓜不附加原版减速
	PVZ::Memory::WriteMemory<byte>(0x46D2A1, 0);
}