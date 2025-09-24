#pragma once
#include "../pvzclass/Events/DLLEvent.h"

namespace PVZEvent
{
	/// @brief 大嘴花吞噬僵尸事件。\n
	///		取消该事件将不会结算啃咬伤害。若希望结算伤害，请使用其他事件。
	/// @param 依次为：触发事件的植物、该植物啃食的僵尸。
	/// @return 是否吞噬此僵尸。
	class ChomperDevourEvent : public BoolDLLEventTemplate<0x4614F9, 5, 0x4614E5, REG_ESI, REG_EDI>
	{
	public:
		ChomperDevourEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ChomperDevourEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ChomperDevourEvent() : ChomperDevourEvent("onChomperDevour") {};
	};

	/// @brief 大嘴花开始咀嚼事件。\n
	///		时机上落后于原版的设置。
	/// @param 触发事件的植物
	class ChomperChewStartEvent : public DLLEventTemplate<0x461555, 5, REG_EDI>
	{
	public:
		ChomperChewStartEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ChomperChewStartEvent(int address) : DLLEventTemplate() { Init(address); };
		ChomperChewStartEvent() : ChomperChewStartEvent("onChomperChewStart") {};
	};

	/// @brief 大嘴花完成咀嚼事件。\n
	///		时机上落后于原版的设置。
	/// @note 大嘴花吞噬失败也会触发这个事件。
	/// @param 触发事件的植物
	class ChomperChewFinishEvent : public DLLEventTemplate<0x4615BF, 7, REG_EDI>
	{
	public:
		ChomperChewFinishEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ChomperChewFinishEvent(int address) : DLLEventTemplate() { Init(address); };
		ChomperChewFinishEvent() : ChomperChewFinishEvent("onChomperChewFinish") {};
	};

	/// @brief 植物更新特殊外观事件
	/// @param 触发事件的植物
	/// @return 是否结算原版对部分植物的特殊外观设定。
	class PlantSpecialAnimateEvent : public BoolDLLEventTemplate<0x464E4E, 6, 0x464E7C, REG_EDI>
	{
	public:
		PlantSpecialAnimateEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantSpecialAnimateEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantSpecialAnimateEvent() : PlantSpecialAnimateEvent("onPlantSpecialAnimate") {};
	};

	/// @brief 获取子弹伤害标签事件
	/// @param 触发事件的子弹，子弹击中的僵尸
	/// @return 伤害标签，负值会被忽略。
	class ProjectileDmgFlagEvent : public IntDLLEventTemplate<0x46D230, 6, 0, 0, 0, REG_EAX, true, REG_EAX, REG_EDI>
	{
	public:
		ProjectileDmgFlagEvent(const char* str) : IntDLLEventTemplate() { Init(str); };
		ProjectileDmgFlagEvent(int address) : IntDLLEventTemplate() { Init(address); };
		ProjectileDmgFlagEvent() : ProjectileDmgFlagEvent("GetProjectileDmgFlag") {};
	};

	/// @brief 植物获取伤害范围标签事件
	/// @param 植物、PlantWeapon
	/// @return 伤害标签，负值会回到原版更新，非零值会跳过原版直接返回
	class PlantGetDamageRangeFlagsEvent : public DLLEventTemplate<0x45EB10, 6, MEM_ESP_ADD(0x24), REG_EAX>
	{
	public:
		PlantGetDamageRangeFlagsEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantGetDamageRangeFlagsEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantGetDamageRangeFlagsEvent() : PlantGetDamageRangeFlagsEvent("onPlantGetDamageRangeFlags") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.cmp_reg_imm(REG_EAX, 0).jl_rel(8);
			builder.mov_mem_esp_add_imm8_reg(0x1C - ((REG_EAX & 7) << 2), REG_EAX).popad();
			builder.retn(0x4);
		}
	};

	/// @brief 辣椒僵尸爆炸事件
	/// @param 触发事件的僵尸
	/// @return 是否执行烧毁植物的部分。
	class JalapenoZombieBurnEvent : public BoolDLLEventTemplate<0x5276BB, 7, 0x52773E, REG_EDI>
	{
	public:
		JalapenoZombieBurnEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		JalapenoZombieBurnEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		JalapenoZombieBurnEvent() : JalapenoZombieBurnEvent("onJalapenoZombieBurn") {};
	};

	/// @brief 冰车僵尸更新冰道事件
	/// @param 触发事件的僵尸
	/// @return 是否更新冰道。
	class ZomboniUpdateIceEvent : public BoolDLLEventTemplate<0x52A865, 6, 0x52A8D6, REG_ESI>
	{
	public:
		ZomboniUpdateIceEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZomboniUpdateIceEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZomboniUpdateIceEvent() : ZomboniUpdateIceEvent("onZomboniUpdateIce") {};
	};

	/// @brief 小丑僵尸正常关卡爆炸时事件
	/// @param 僵尸、爆炸的X、爆炸的Y
	/// @return False则不经过原版爆炸。原版有个意义不明且看不出效果的炸僵尸？不知道有没有影响
	class ClownZombiePopEvent : public BoolDLLEventTemplate<0x526C67, 5, 0x526C84, REG_EBX, REG_EDI, REG_ESI>
	{
	public:
		ClownZombiePopEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ClownZombiePopEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ClownZombiePopEvent() : ClownZombiePopEvent("onClownZombiePop") {};
	};
	/// @brief 魅惑小丑僵尸爆炸时事件
	/// @param 僵尸、爆炸的X、爆炸的Y
	/// @return False则不经过原版爆炸。
	class HypnotizedClownZombiePopEvent : public BoolDLLEventTemplate<0x526C4E, 6, 0x526C84, REG_EBX, REG_EDI, REG_ESI>
	{
	public:
		HypnotizedClownZombiePopEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		HypnotizedClownZombiePopEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		HypnotizedClownZombiePopEvent() : HypnotizedClownZombiePopEvent("onHypnotizedClownZombiePop") {};
	};

	/// @brief 僵尸发射子弹事件（复合事件）
	/// @param 触发事件的僵尸，僵尸发射的子弹
	class ZombieAddProjectileEvent
	{
	private:
		class Gatling : public DLLEventTemplate<0x527A98, 7, REG_ESI, MEM_ESP_ADD(0x28)>
		{
		public:
			Gatling(const char* str) : DLLEventTemplate() { Init(str); };
			Gatling(int address) : DLLEventTemplate() { Init(address); };
		};
		class PeaHead : public DLLEventTemplate<0x5275B6, 6, REG_EAX, REG_EDI>
		{
		public:
			PeaHead(const char* str) : DLLEventTemplate() { Init(str); };
			PeaHead(int address) : DLLEventTemplate() { Init(address); };
		};
		class Catapult1 : public DLLEvent
		{
		public:
			Catapult1()
			{
				hookAddress = 0x5257D9;
				rawlen = 6;
				PVZ::Memory::AllAccess(0x525884);
				BYTE code[] = { MOV_PTR_ADDR_EUX(REG_ESI, 0x525884)};
				start(STRING(code));
			};
		};
		class Catapult2 : public DLLEvent
		{
		public:
			Catapult2(int address)
			{
				hookAddress = 0x52587B;
				rawlen = 6;
				BYTE code[] =
				{
					PUSH_ESI,
					PUSH_PTR(0x525884),
					INVOKE(address),
					ADD_ESP(8)
				};
				start(STRING(code));
			};
			Catapult2(const char* str) : Catapult2(PVZ::Memory::GetProcAddress(str)) { };
		};
		Gatling* gatling;
		PeaHead* pea;
		Catapult1* catapult1;
		Catapult2* catapult2;
	public:
		ZombieAddProjectileEvent(int address)
		{
			gatling = new Gatling(address);
			pea = new PeaHead(address);
			catapult1 = new Catapult1();
			catapult2 = new Catapult2(address);
		}
		ZombieAddProjectileEvent(const char* str) : ZombieAddProjectileEvent(PVZ::Memory::GetProcAddress(str)) {};
		ZombieAddProjectileEvent() : ZombieAddProjectileEvent("onZombieAddProjectile") {};
		void end()
		{
			gatling->end();
			pea->end();
			catapult1->end();
			catapult2->end();
		}
	};

	/// @brief 子弹击中僵尸事件
	/// @param 依次为：触发事件的子弹，子弹击中的僵尸
	class ProjectileImpactEvent : public DLLEventTemplate<0x46E000, 5, REG_EAX, REG_ECX>
	{
	public:
		ProjectileImpactEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ProjectileImpactEvent(int address) : DLLEventTemplate() { Init(address); };
		ProjectileImpactEvent() : ProjectileImpactEvent("onProjectileImpact") {};
	};

	/// @brief 子弹击中僵尸事件
	/// @param 依次为：触发事件的子弹，发射子弹的植物
	class PlantAddProjDamageRangeFlagsEvent : public DLLEventTemplate<0x4672CA, 6, REG_EBP, REG_ECX>
	{
	public:
		PlantAddProjDamageRangeFlagsEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantAddProjDamageRangeFlagsEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantAddProjDamageRangeFlagsEvent() : PlantAddProjDamageRangeFlagsEvent("onPlantAddProjDamageRangeFlags") {};
	};

	/// @brief 子弹总更新事件，发生在计时器与图层更新后、子弹运动前
	/// @param 子弹ID
	class ProjectileUpdateEvent : public DLLEventTemplate<0x46E4FE, 6, REG_ESI>
	{
	public:
		ProjectileUpdateEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ProjectileUpdateEvent(int address) : DLLEventTemplate() { Init(address); };
		ProjectileUpdateEvent() : ProjectileUpdateEvent("onProjectileUpdate") {};
	};

	/// @brief 三线边路运动方式的子弹的更新事件
	/// @param 子弹ID
	/// @return False则跳过原版运动方式更新
	class ProjectileSlideMotionEvent : public BoolDLLEventTemplate<0x46DBF6, 6, 0x46DC1F, REG_EBX>
	{
	public:
		ProjectileSlideMotionEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ProjectileSlideMotionEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ProjectileSlideMotionEvent() : ProjectileSlideMotionEvent("onProjectileSlideMotion") {};
	};
	/// @brief 三线成功索敌并准备开火的事件
	/// @param 植物
	/// @return True则会重置+90为35！
	class ThreepeaterLaunchEvent : public BoolDLLEventTemplate<0x45F452, 10, 0x45F45C, REG_EDI>
	{
	public:
		ThreepeaterLaunchEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ThreepeaterLaunchEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ThreepeaterLaunchEvent() : ThreepeaterLaunchEvent("onThreepeaterLaunch") {};
	};

	/// @brief 大嘴花判定是否秒杀僵尸的事件
	/// @param 依次为：触发事件的植物，植物攻击的僵尸
	class ChomperInstantJudgeEvent : public IntDLLEventTemplate<0x461444, 6, 0, 0, 0, REG_EDX, false, REG_ESI, REG_EDI>
	{
	public:
		ChomperInstantJudgeEvent(const char* str) : IntDLLEventTemplate() { Init(str); };
		ChomperInstantJudgeEvent(int address) : IntDLLEventTemplate() { Init(address); };
		ChomperInstantJudgeEvent() : ChomperInstantJudgeEvent("onChomperInstantJudge") {};
	};

	/// @brief 磁力菇修改吸收半径的事件
	/// @param 植物，当前遍历的僵尸
	/// @return 负数则使用原版半径（270，啃咬320）,否则使用返回值作为半径
	class MagnetShroomAttractRadiusEvent : public DLLEventTemplate<0x4620F4, 9, REG_EBX, REG_EDI>
	{
	public:
		MagnetShroomAttractRadiusEvent(const char* str) : DLLEventTemplate() { Init(str); };
		MagnetShroomAttractRadiusEvent(int address) : DLLEventTemplate() { Init(address); };
		MagnetShroomAttractRadiusEvent() : MagnetShroomAttractRadiusEvent("onMagnetShroomAttractRadius") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.cmp_reg_imm(REG_EAX, 0).jl_rel(11);
			builder.mov_mem_esp_add_imm8_reg(0x1C, REG_EAX).popad().push_imm32(0x462104).ret();
		}
	};

	/// @brief 磁力菇控制吸引的物体运动的事件
	/// @param 植物
	/// @return True则使用原版运动
	class MagnetShroomMoveItemEvent : public BoolDLLEventTemplate<0x461DB6, 6, 0x461E1F, REG_EBX>
	{
	public:
		MagnetShroomMoveItemEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		MagnetShroomMoveItemEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		MagnetShroomMoveItemEvent() : MagnetShroomMoveItemEvent("onMagnetShroomMoveItem") {};
	};
	/// @brief 磁力菇吸引物体后瞬间的事件，可以修改铁器的属性，也可以用于吸取时加生命值等
	/// @param 植物，目标僵尸
	class MagnetShroomAttractItemEvent : public DLLEventTemplate<0x4621B3, 5, REG_EBX, REG_EDI>
	{
	public:
		MagnetShroomAttractItemEvent(const char* str) : DLLEventTemplate() { Init(str); };
		MagnetShroomAttractItemEvent(int address) : DLLEventTemplate() { Init(address); };
		MagnetShroomAttractItemEvent() : MagnetShroomAttractItemEvent("onMagnetShroomAttractItem") {};
	};
	/// @brief 磁力菇清空物体恢复Idle的事件
	/// @note 发生在播放动画后，重置+D8前
	/// @param 植物
	class MagnetShroomClearItemEvent : public DLLEventTemplate<0x461E6E, 5, REG_EAX>
	{
	public:
		MagnetShroomClearItemEvent(const char* str) : DLLEventTemplate() { Init(str); };
		MagnetShroomClearItemEvent(int address) : DLLEventTemplate() { Init(address); };
		MagnetShroomClearItemEvent() : MagnetShroomClearItemEvent("onMagnetShroomClearItem") {};
	};

	/// @brief 魅惑菇被啃的事件
	/// @param 触发事件的魅惑菇，啃食魅惑菇的僵尸
	/// @return True则魅惑菇正常死亡
	class HypnoShroomEatenEvent : public DLLEventTemplate<0x52B9D2, 5, REG_EDI, REG_ESI>
	{
	public:
		HypnoShroomEatenEvent(const char* str) : DLLEventTemplate() { Init(str); };
		HypnoShroomEatenEvent(int address) : DLLEventTemplate() { Init(address); };
		HypnoShroomEatenEvent() : HypnoShroomEatenEvent("onHypnoShroomEaten") {};
		void InitExtra(AsmBuilder& builder)
		{
			builder.test_al_al().jnz_rel(10).popad().add_reg_imm(REG_ESP, 4).push_imm32(0x52B9D7).ret().add_reg_imm(REG_ESP, 4);
		}
	};

	/// @brief 原版缠绕海草+54=0时，自身死亡并击杀目标僵尸的事件
	/// @param 触发事件的海草
	/// @return False则跳过原版更新
	class TangleKelpKillZombieEvent : public BoolDLLEventTemplate<0x4602D5, 6, 0x460316, REG_EDI>
	{
	public:
		TangleKelpKillZombieEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		TangleKelpKillZombieEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		TangleKelpKillZombieEvent() : TangleKelpKillZombieEvent("onTangleKelpKillZombie") {};
	};

	/// @brief 原版缠绕海草抓住僵尸时的更新事件
	/// @note 此事件发生在判断水草的+54=50、20、0之后！
	/// @param 触发事件的海草
	class TangleKelpUpdateGrabbingEvent : public DLLEventTemplate<0x460316, 5, REG_EDI>
	{
	public:
		TangleKelpUpdateGrabbingEvent(const char* str) : DLLEventTemplate() { Init(str); };
		TangleKelpUpdateGrabbingEvent(int address) : DLLEventTemplate() { Init(address); };
		TangleKelpUpdateGrabbingEvent() : TangleKelpUpdateGrabbingEvent("onTangleKelpUpdateGrabbing") {};
		void InitExtra(AsmBuilder& builder)
		{
			//因为注入点在0x460316，为了防止意外访问，需要修改原版的一处跳转
			PVZ::Memory::WriteMemory<int>(0x0046008B, 0x000000FF);
		}
	};
	/// @brief 原版缠绕海草寻找到僵尸并获取目标后的事件
	/// @param 触发事件的海草、寻找到的僵尸
	class TangleKelpTargetAfterEvent : public DLLEventTemplate<0x460188, 6, REG_ESI, REG_EDI>
	{
	public:
		TangleKelpTargetAfterEvent(const char* str) : DLLEventTemplate() { Init(str); };
		TangleKelpTargetAfterEvent(int address) : DLLEventTemplate() { Init(address); };
		TangleKelpTargetAfterEvent() : TangleKelpTargetAfterEvent("onTangleKelpTargetAfter") {};
	};

	/// @brief 胆小菇下蹲瞬间事件
	/// @param 胆小菇
	/// @return False则不会蹲下
	class ScardyShroomScaredEvent : public BoolDLLEventTemplate<0x46051B, 6, 0x4605EE, REG_EDI>
	{
	public:
		ScardyShroomScaredEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ScardyShroomScaredEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ScardyShroomScaredEvent() : ScardyShroomScaredEvent("onScardyShroomScared") {};
	};

	/// @brief 胆小菇起立瞬间事件
	/// @param 胆小菇
	/// @return False则不会起立
	class ScardyShroomGrowEvent : public BoolDLLEventTemplate<0x46057F, 6, 0x4605EE, REG_EDI>
	{
	public:
		ScardyShroomGrowEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ScardyShroomGrowEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ScardyShroomGrowEvent() : ScardyShroomGrowEvent("onScardyShroomGrow") {};
	};

	class ZombieDropHelmByDamageEvent : public DLLEventTemplate<0x531070, 5, REG_EBP>
	{
	public:
		ZombieDropHelmByDamageEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieDropHelmByDamageEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieDropHelmByDamageEvent() : ZombieDropHelmByDamageEvent("onZombieDropHelmByDamage") {};
	};

	/// @brief 物品自动收集事件，注入功能：存在1秒后自动设置自己为被收集状态
	/// @param 物品的ID
	class CoinAutoCollectEvent : public DLLEventTemplate<0x43158B, 6, REG_EBX>
	{
	public:
		CoinAutoCollectEvent(const char* str) : DLLEventTemplate() { Init(str); };
		CoinAutoCollectEvent(int address) : DLLEventTemplate() { Init(address); };
		CoinAutoCollectEvent() : CoinAutoCollectEvent("onCoinAutoCollect") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.test_al_al().jz_rel(7).popad().push_imm32(0x431599).ret().popad().push_imm32(0x431591).ret();
		}
	};

	/// @brief 游戏内按键的事件
	/// @param Board，Key
	class TypingEvent : public DLLEventTemplate<0x41B1D0, 6, REG_ECX, REG_EDX>
	{
	public:
		TypingEvent(const char* str) : DLLEventTemplate() { Init(str); };
		TypingEvent(int address) : DLLEventTemplate() { Init(address); };
		TypingEvent() : TypingEvent("onTyping") {};
	};

	/// @brief 辣椒烧僵尸事件
	/// @param 触发事件的植物，植物烧的僵尸
	class JalapenoBurnEvent : public DLLEventTemplate<0x466528, 5, REG_ESI, REG_EDI>
	{
	public:
		JalapenoBurnEvent(const char* str) : DLLEventTemplate() { Init(str); };
		JalapenoBurnEvent(int address) : DLLEventTemplate() { Init(address); };
		JalapenoBurnEvent() : JalapenoBurnEvent("onJalapenoBurn") {};
	};

	/// @brief 小推车割僵尸事件
	/// @param 触发事件的小推车，小推车割的僵尸
	class LawnmowerMowZombieEvent : public DLLEventTemplate<0x458540, 6, MEM_ESP_ADD(0x20), REG_EAX>
	{
	public:
		LawnmowerMowZombieEvent(const char* str) : DLLEventTemplate() { Init(str); };
		LawnmowerMowZombieEvent(int address) : DLLEventTemplate() { Init(address); };
		LawnmowerMowZombieEvent() : LawnmowerMowZombieEvent("onLawnmowerMowZombie") {};
	};

	/// @brief 植物开始眨眼事件
	/// @param 触发事件的植物
	/// @return 是否眨眼
	class PlantStartBlinkEvent : public BoolDLLEventTemplate<0x464341, 5, 0x464383, REG_ESI>
	{
	public:
		PlantStartBlinkEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantStartBlinkEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantStartBlinkEvent() : PlantStartBlinkEvent("onPlantStartBlink") {};
	};

	/// @brief 植物开始攻击动作事件
	/// @param 触发事件的植物，植物选择的目标僵尸，植物是否使用副武器
	class NormalPlantAttackStartEvent : public DLLEventTemplate<0x45EF3F, 6, MEM_ESP_ADD(0x3C), MEM_ESP_ADD(0x34), REG_ESI>
	{
	public:
		NormalPlantAttackStartEvent(const char* str) : DLLEventTemplate() { Init(str); };
		NormalPlantAttackStartEvent(int address) : DLLEventTemplate() { Init(address); };
		NormalPlantAttackStartEvent() : NormalPlantAttackStartEvent("onNormalPlantAttackStart") {};
	};

	/// @brief 修改植物的颜色
	/// @param 植物（ESI）、动画ID（EBX）
	/// @return True则无颜色修改，与原版相同
	class PlantUpdateColorEvent : public BoolDLLEventTemplate<0x463714, 6, 0x463720, REG_EBX, REG_ESI>
	{
	public:
		PlantUpdateColorEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantUpdateColorEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantUpdateColorEvent() : PlantUpdateColorEvent("onPlantUpdateColor") {};
	};

	/// @brief 射手植物更新射击事件
	/// @param 触发事件的植物
	/// @return 是否进行更新
	/// @retval 跳过本次更新
	class PlantUpdateShooterEvent : public BoolDLLEventTemplate<0x45F8A3, 6, 0x45F97C, REG_EAX>
	{
	public:
		PlantUpdateShooterEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantUpdateShooterEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantUpdateShooterEvent() : PlantUpdateShooterEvent("onPlantUpdateShooter") {};
	};
	/// @brief 投手类植物跳过被多投标记的僵尸事件
	/// @param 植物ID(ECX)，僵尸ID(ESI)
	/// @return False则跳过该僵尸
	class PlantPultSkipEvent : public BoolDLLEventTemplate<0x4677E9, 6, 0x467881, REG_ESI, REG_ECX>
	{
	public:
		PlantPultSkipEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantPultSkipEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantPultSkipEvent() : PlantPultSkipEvent("onPlantPultSkip") {};
	};
	/// @brief 植物更新Shooting事件，发生在更新+90计时前
	/// @param 植物ID
	/// @return False则跳过原版更新，注意更新计时、重置豌豆头部动画等也会被跳过
	class PlantUpdateShootingEvent : public BoolDLLEventTemplate<0x464889, 6, 0x464D9F, REG_EDI>
	{
	public:
		PlantUpdateShootingEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantUpdateShootingEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantUpdateShootingEvent() : PlantUpdateShootingEvent("onPlantUpdateShooting") {};
	};
	/// @brief 植物开火事件。
	/// @note 此事件与 PlantShootEvent 注入位置相同，区别在于此事件可以选择跳过原版开火
	/// @param 触发事件的植物、开火目标僵尸、Weapon类型
	/// @return False则跳过原版开火
	class PlantFireEvent : public BoolDLLEventTemplate<0x466E0D, 6, 0x466E7A, REG_EBX, MEM_ESP_ADD(0x50), REG_EBP>
	{
	public:
		PlantFireEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantFireEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantFireEvent() : PlantFireEvent("onPlantFire") {};
	};
	/// @brief 植物开火生成子弹前的事件。
	/// @param 触发事件的植物、子弹类型、子弹初始化坐标X、Y
	/// @return False则跳过子弹生成
	/// @deprecated 请使用 PVZEvent::PlantFireEvent
	class PlantAddProjectileBeforeEvent : public BoolDLLEventTemplate<0x4672A5, 5, 0x467319, REG_ESI,REG_EDI,REG_EAX,REG_EBP>
	{
	public:
		PlantAddProjectileBeforeEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantAddProjectileBeforeEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantAddProjectileBeforeEvent() : PlantAddProjectileBeforeEvent("onPlantAddProjectileBefore") {};
	};
	/// @brief 四个投手的多投事件，冰瓜的概率判定也在此处
	/// @param 植物ID（EDI） 植物副武器（ESI）
	class PlantPultMultipleEvent : public DLLEventTemplate<0x464C1C, 5, REG_ESI, REG_EDI>
	{
	public:
		PlantPultMultipleEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantPultMultipleEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantPultMultipleEvent() : PlantPultMultipleEvent("onPlantPultMultiple") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.popad().push_imm32(0x464C34).ret();
		}
	};
	/// @brief 一次性植物在+50>0时更新的事件
	/// @note 正常植物在+50>0时也会经过这里，并且原版中也会调用DoSpecial事件，但是无事发生
	/// @note 不调用DoSpecial并不妨碍灰烬植物消失！灰烬消失取决于状态是否为2（灰烬生效），当状态=2或者自身被碾压，且+4C倒计时为0时,自身死亡
	/// @note 寒冰菇会一直更新自己的状态为2，其余灰烬植物似乎没有这种情况，设置寒冰菇tick ability返回false可以解决该问题
	/// @param 触发事件的植物
	/// @return False则不DoSpecial
	class SingleUsePlantUpdateEvent : public BoolDLLEventTemplate<0x463402, 6, 0x463410, REG_EDI>
	{
	public:
		SingleUsePlantUpdateEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		SingleUsePlantUpdateEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		SingleUsePlantUpdateEvent() : SingleUsePlantUpdateEvent("onSingleUsePlantUpdate") {};
	};

	/// @brief 总绘制事件，位置在绘制金钱框函数里
	/// @param Graphics*(REG_EDI)和Board*(REG_EDX)
	class BoardDrawImageEvent : public DLLEventTemplate<0x41A2B9, 6, REG_EDX, REG_EDI>
	{
	public:
		BoardDrawImageEvent(const char* str) : DLLEventTemplate() { Init(str); };
		BoardDrawImageEvent(int address) : DLLEventTemplate() { Init(address); };
		BoardDrawImageEvent() : BoardDrawImageEvent("onBoardDrawImage") {};
	};
	/// @brief 加载以普僵为基础模型的动画时最开头的事件
	/// @param 僵尸
	class LoadPlainZombieReanimBeforeEvent : public DLLEventTemplate<0x524370, 5, REG_EDI>
	{
	public:
		LoadPlainZombieReanimBeforeEvent(const char* str) : DLLEventTemplate() { Init(str); };
		LoadPlainZombieReanimBeforeEvent(int address) : DLLEventTemplate() { Init(address); };
		LoadPlainZombieReanimBeforeEvent() : LoadPlainZombieReanimBeforeEvent("onLoadPlainZombieReanimBefore") {};
	};
	/// @brief 创建动画图集事件
	/// @param 触发事件的动画类型，触发事件的动画定义
	/// @return 是否生成动画图集
	class CreateAtlasEvent : public BoolDLLEventTemplate<0x471A96, 7, 0x471AFC, REG_ESI, REG_EAX>
	{
	public:
		CreateAtlasEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		CreateAtlasEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		CreateAtlasEvent() : CreateAtlasEvent("onCreateAtlas") {};
	};

	/// @brief 僵尸碾压植物事件。
	/// @attention 注意碾压≠植物死亡，而是指植物压成饼这一事件。
	/// @note 僵王踩踏和砸车暂时不触发此事件。
	/// @note 原版中的细节：巨人在碾压地刺王时，调用45EC00，并且这里会触发僵尸伤害植物事件；倭瓜头只调用squish()；车类只调用squish，植物的范围伤害函数中，地刺王命中会调用45EC00，地刺会调用Die()，两者均造成1800点伤害；冰火球我没看
	/// @param 触发事件的僵尸，被碾压的行数，被碾压的列数，僵尸攻击类型（车类、冰火球=1,巨人、倭瓜头=0，原版中前者对地刺、地刺王无效，后者只对地刺王无效），被碾压的植物
	/// @return 是否继续结算碾压。若取消，碾压不会发生。
	class ZombieSquishPlantEvent : public BoolDLLEventTemplate<0x52E971, 9, 0x52E980, REG_ECX, MEM_ESP_ADD(0x40),
		MEM_ESP_ADD(0x3C), MEM_ESP_ADD(0x44), REG_EDI>
	{
	public:
		ZombieSquishPlantEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieSquishPlantEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieSquishPlantEvent() : ZombieSquishPlantEvent("onZombieSquishPlant") {};
	};
	/// @brief 僵尸移动速度/瞬时位移修改事件，不涉及动画速度，因此修改后会有“滑行”的观感
	/// @param 僵尸，已经计算好的瞬时位移（浮点数）
	/// @return 修改后的瞬时位移
	class ZombieUpdateWalkingSpeedEvent : public DLLEventTemplate<0x52AB18,5, MEM_ESP_ADD(0x28), REG_ESI>
	{
	public:
		ZombieUpdateWalkingSpeedEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieUpdateWalkingSpeedEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieUpdateWalkingSpeedEvent() : ZombieUpdateWalkingSpeedEvent("onZombieUpdateWalkingSpeed") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.fstp_m32_esp_imm8(0x28);
		}
	};
	/// @brief 僵尸出水的额外判断
	/// @note 不影响原版中僵尸+BD=1且自身不在水格中的前置条件
	/// @param 僵尸
	/// @return False则不出水
	class ZombieWalkOutOfWaterEvent : public BoolDLLEventTemplate<0x52F996, 7, 0x52F9AA, REG_EBP>
	{
	public:
		ZombieWalkOutOfWaterEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieWalkOutOfWaterEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieWalkOutOfWaterEvent() : ZombieWalkOutOfWaterEvent("onZombieWalkOutOfWater") {};
	};
	/// @brief 僵尸入水的额外判断
	/// @param 僵尸
	/// @return False则直接入水，True则正常进行原版判断（如果+BD=0且自身不在水格中则入水）
	class ZombieWalkIntoWaterEvent : public BoolDLLEventTemplate<0x52F942, 6, 0x52F950, REG_EBP>
	{
	public:
		ZombieWalkIntoWaterEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieWalkIntoWaterEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieWalkIntoWaterEvent() : ZombieWalkIntoWaterEvent("onZombieWalkIntoWater") {};
	};

	/// @brief 僵尸更新坠落事件
	/// @param 僵尸
	/// @return False则跳过原版位置更新
	class ZombieUpdateFallingEvent : public BoolDLLEventTemplate<0x529775, 6, 0x5297A1, REG_ESI>
	{
	public:
		ZombieUpdateFallingEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieUpdateFallingEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieUpdateFallingEvent() : ZombieUpdateFallingEvent("onZombieUpdateFalling") {};
	};

	/// @brief 僵尸落地时事件
	/// @param 僵尸
	class ZombieFallOnGroundEvent : public DLLEventTemplate<0x5297DA, 7, REG_ESI>
	{
	public:
		ZombieFallOnGroundEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieFallOnGroundEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieFallOnGroundEvent() : ZombieFallOnGroundEvent("onZombieFallOnGround") {};
	};

	/// @brief 判断僵尸是否反向事件
	/// @param 僵尸
	/// @return 负数则调用原版函数，0则返回false，1则返回true
	class ZombieIsWalkingBackwardsEvent : public DLLEventTemplate<0x52BEE0, 7, REG_ECX>
	{
	public:
		ZombieIsWalkingBackwardsEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieIsWalkingBackwardsEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieIsWalkingBackwardsEvent() : ZombieIsWalkingBackwardsEvent("onZombieIsWalkingBackwards") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.cmp_reg_imm(REG_EAX,0).jl_rel(6);
			builder.mov_mem_esp_add_imm8_reg(0x1C, REG_EAX).popad().ret();
		}
	};
	/// @brief 僵尸索敌间隔的重写事件
	/// @note 在原版中，使用僵尸的+60 % 4或8（无减速/有减速） = 0 作为触发索敌的条件，该事件用于替代这一判断
	/// @param 僵尸
	/// @return 负数则调用原版判断，0则不索敌，正数则索敌
	class ZombieFindTargetIntervalEvent : public DLLEventTemplate<0x52F640, 7, REG_EDI>
	{
	public:
		ZombieFindTargetIntervalEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieFindTargetIntervalEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieFindTargetIntervalEvent() : ZombieFindTargetIntervalEvent("onZombieFindTargetInterval") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.cmp_reg_imm(REG_EAX, 0).jl_rel(23);
			builder.cmp_reg_imm(REG_EAX,0).jne_rel(7).popad().push_imm32(0x52F6BC).ret();
			builder.popad().push_imm32(0x52F65D).ret();
		}
	};
	/// @brief 僵尸能否将植物作为目标的额外判断，优先级高于原版
	/// @param 僵尸，植物，攻击方式(0(啃食/锤砸) | 1(车类碾压) | 2(跳跃) | 3(搭梯))
	/// @return False则直接不攻击植物，True则正常原版判断
	/// @note 该事件暂时被PVZClass的ZombieTargetPlant替代
	/// @deprecated
	class ZombieCanTargetPlantEvent : public BoolDLLEventTemplate<0x52E500, 6, 0x52E578, MEM_ESP_ADD(0x4C), REG_ESI, REG_EBP>
	{
	public:
		ZombieCanTargetPlantEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieCanTargetPlantEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieCanTargetPlantEvent() : ZombieCanTargetPlantEvent("onZombieCanTargetPlant") {};
	};
	/// @brief 僵尸在切换状态时，调用原版PickRandomSpeed获取并重置移速的事件
	/// @param 僵尸
	/// @return False则直接返回，True则正常原版
	class ZombiePickRandomSpeedEvent : public BoolDLLEventTemplate<0x524A70, 7, 0x524A8C, REG_EAX>
	{
	public:
		ZombiePickRandomSpeedEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombiePickRandomSpeedEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombiePickRandomSpeedEvent() : ZombiePickRandomSpeedEvent("onZombiePickRandomSpeed") {};
	};
	/// @brief 僵尸施加动画速度事件，主要是处理减速相关
	/// @param 僵尸、动画、原速率
	/// @return 修改后的动画速率（浮点数）
	class ZombieApplyAnimSpeedEvent : public DLLEventTemplate<0x52F01F, 7, MEM_ESP_ADD(0x2C), REG_ESI, REG_EAX>
	{
	public:
		ZombieApplyAnimSpeedEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieApplyAnimSpeedEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieApplyAnimSpeedEvent() : ZombieApplyAnimSpeedEvent("onZombieApplyAnimSpeed") {};
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.fstp_m32_esp_imm8(0x2C);
		}
	};
	/// @brief 僵尸修改绘制颜色事件
	/// @attention 注意不是所有状态的僵尸都会经过这个位置，这个位置适用于新增绘制
	/// @param 僵尸、动画，原R,G,B,A（均为int，0~255）
	/// @return True则使用原版颜色，False则跳过原版颜色。
	class ZombieUpdateColorEvent : public BoolDLLEventTemplate<0x52D3F6, 7, 0x52D429, MEM_ESP_ADD(0x4C), REG_EDX, REG_ECX, REG_EAX, REG_EBX, REG_ESI>
	{
	public:
		ZombieUpdateColorEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieUpdateColorEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieUpdateColorEvent() : ZombieUpdateColorEvent("onZombieUpdateColor") {};
	};
	/// @brief 子弹初始化完成事件
	/// @note 此时 ImageX 和 ImageY 均未初始化
	/// @param 触发事件的子弹
	class ProjectileInitAfterEvent : public DLLEventTemplate<0x46CA78, 5, REG_EBP>
	{
	public:
		ProjectileInitAfterEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ProjectileInitAfterEvent(int address) : DLLEventTemplate() { Init(address); };
		ProjectileInitAfterEvent() : ProjectileInitAfterEvent("onProjectileInitAfter") {};
	};
	/// @brief 火球初始化动画时根据SpecialType设置颜色的事件
	/// @note 由于直接获取火球动画的方式过于复杂，就直接在过火初始化动画时就设置颜色了。
	/// @param 触发事件的子弹 动画
	class FireballInitColorEvent : public DLLEventTemplate<0x46ED91, 6, REG_EBP,REG_EDI>
	{
	public:
		FireballInitColorEvent(const char* str) : DLLEventTemplate() { Init(str); };
		FireballInitColorEvent(int address) : DLLEventTemplate() { Init(address); };
		FireballInitColorEvent() : FireballInitColorEvent("onFireballInitColor") {};
	};

	/// @brief 检查子弹是否应该过期事件
	/// @note 构造后，原版的判定条件会失效
	/// @param 触发事件的子弹
	/// @return 子弹是否过期
	class ProjectileCheckExpireEvent : public DiversionEventTemplate<0x46CE91, 6, 0x46D047, 0x46CE9D, REG_EBP>
	{
	public:
		ProjectileCheckExpireEvent(const char* str) : DiversionEventTemplate() { Init(str); };
		ProjectileCheckExpireEvent(int address) : DiversionEventTemplate() { Init(address); };
		ProjectileCheckExpireEvent() : ProjectileCheckExpireEvent("onProjectileCheckExpire") {};
	};

	/// @brief 植物索敌优先级重载函数
	/// @note 返回值越高，优先级越高
	/// @param 触发事件的植物，植物当前索敌的僵尸，原始优先级
	/// @return 重载后的优先级
	class PlantFindTargetZombiePriorityEvent : public DLLEventTemplate<0x46786C, 5, REG_EAX, REG_ESI, MEM_ESP_ADD(0x78)>
	{
	public:
		PlantFindTargetZombiePriorityEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantFindTargetZombiePriorityEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantFindTargetZombiePriorityEvent() : PlantFindTargetZombiePriorityEvent("GetPlantFindTargetZombiePriority") {};
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.mov_mem_esp_add_imm8_reg(0x1C, REG_EAX);
		}
	};

	namespace KernelPult
	{
		/// @brief 玉米投手判定是否投掷黄油事件
		/// @note 若启用该事件，原版的判定条件将被废弃
		/// @param 触发事件的植物
		/// @return 是否投掷黄油
		class JudgeButterEvent : public DiversionEventTemplate<0x45F1E5, 5, 0x45F1EE, 0x45F22D, REG_ESI>
		{
		public:
			JudgeButterEvent(const char* str) : DiversionEventTemplate() { Init(str); };
			JudgeButterEvent(int address) : DiversionEventTemplate() { Init(address); };
			JudgeButterEvent() : JudgeButterEvent("onKernelPultJudgeButter") {};
		};
	}


	/// @brief 投篮车的索敌跳过事件，用于投篮车多投
	/// @param 触发事件的僵尸、当前遍历的植物
	/// @return False则跳过该植物
	class CatapultTargetSkipEvent : public BoolDLLEventTemplate<0x5258D3, 6, 0x5259A9, REG_EAX, REG_ECX>
	{
	public:
		CatapultTargetSkipEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		CatapultTargetSkipEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		CatapultTargetSkipEvent() : BoolDLLEventTemplate() { Init("onCatapultTargetSkip"); };
	};
	/// @brief 投篮车的索敌并开火事件
	/// @param 触发事件的僵尸
	/// @return False则跳过原版开火
	class CatapultZombieFireEvent : public DLLEventTemplate<0x525A71, 6, REG_EDI>
	{
	public:
		CatapultZombieFireEvent(const char* str) : DLLEventTemplate() { Init(str); };
		CatapultZombieFireEvent(int address) : DLLEventTemplate() { Init(address); };
		CatapultZombieFireEvent() : DLLEventTemplate() { Init("onCatapultZombieFire"); };
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.test_al_al().jnz_rel(7).popad().push_imm32(0x525A7E).ret().popad().push_reg(REG_EDI).invoke(0x525890).push_imm32(0x525A77).ret();
		}
	};

	/// @brief Zombie 行为动作的更新。
	/// @note 时机上先于原版的更新。
	/// @note 该事件与 ZombieUpdateActionEvent 不同，只影响僵尸自身的技能。
	/// @param 更新的 Zombie
	/// @return 是否更新原版行为动作
	/// @retval false 完全跳过原版的任何行为动作，这会导致原生技能失效。
	class ZombieUpdateAbilityEvent : public DLLEventTemplate<0x52B174, 6, REG_ESI>
	{
	public:
		ZombieUpdateAbilityEvent() : DLLEventTemplate() { Init("onZombieUpdateAbility"); };
		ZombieUpdateAbilityEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieUpdateAbilityEvent(int address) : DLLEventTemplate() { Init(address); };
	protected:
		void InitExtra(AsmBuilder& builder)
		{
			static constexpr byte after[] =
			{
				TEST_AL_AL,
				JNZ(7),

				POPAD,
				PUSHDWORD(0x52B279),
				RET,

				POPAD,
				CMP_PTR_EUX_ADD_V_V(REG_ESI, 0x24, 3),
				JNZ(6),

				PUSHDWORD(0x52B17A),
				RET,
				PUSHDWORD(0x52B279),
				RET,
			};
			builder.add_bytes(STRING(after));
		}
	};

	/// @brief 重载僵尸绘制位置事件
	/// @note 此时已经完成了除 BodyY 和 ClipRect 的所有量的设定
	/// @param 触发事件的僵尸，僵尸的 ZombieDrawPos
	/// @return 是否执行原版对 BodyY 和 ClipRect 的设定
	class ZombieOverrideDrawPosEvent : BoolDLLEventTemplate<0x52DBAC, 6, 0x52DFD8, REG_ESI, REG_ECX>
	{
	public:
		ZombieOverrideDrawPosEvent() : BoolDLLEventTemplate() { Init("OverrideZombieDrawPos"); };
		ZombieOverrideDrawPosEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieOverrideDrawPosEvent(int address) : BoolDLLEventTemplate() { Init(address); };
  };

	/// @brief 植物因生命值小于 0 被移除事件。
	/// @note 照搬的PVZCLASS里的事件，在这基础上增加了一个死亡类型
	/// @param 触发事件的植物。
	/// @return 该植物是否被移除。
	class PlantDyingFromLowHealthEvent : public DLLEventTemplate<0x463EDD, 6, CONST_VAL(DYING_HP_BELOW_ZERO), REG_EBX>
	{
	public:
		PlantDyingFromLowHealthEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantDyingFromLowHealthEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantDyingFromLowHealthEvent() : DLLEventTemplate() { Init("onPlantDyingFromLowHealth"); };
		void InitExtra(AsmBuilder& builder)
		{
			BYTE code[] =
			{
				TEST_AL_AL,
				JE(14),

				PUSH_EBX,
				INVOKE(0x4679B0),

				POPAD,
				MOV_ECX(0x463EE3),
				JMP_REG32(REG_ECX)
			};
			builder.add_bytes(STRING(code));
		}
	};
	/// @brief 植物因被啃死而被移除事件。
	/// @param 触发事件的植物。
	/// @return 该植物是否被移除。
	class PlantDyingFromEatenEvent : public BoolDLLEventTemplate<0x52FD4E, 6, 0x52FD5A, CONST_VAL(DYING_EATEN), REG_ESI>
	{
	public:
		PlantDyingFromEatenEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantDyingFromEatenEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantDyingFromEatenEvent() : BoolDLLEventTemplate() { Init("onPlantDyingFromEaten"); };
	};
	/// @brief 植物因压扁或者自身是灰烬而+4C=0时被移除事件。
	/// @param 触发事件的植物。
	/// @return 该植物是否被移除。
	class PlantDyingFromDisappearingEvent : public DLLEventTemplate<0x463191, 6, CONST_VAL(DYING_DISAPPEARING), REG_EDI>
	{
	public:
		PlantDyingFromDisappearingEvent(const char* str) : DLLEventTemplate() { Init(str); };
		PlantDyingFromDisappearingEvent(int address) : DLLEventTemplate() { Init(address); };
		PlantDyingFromDisappearingEvent() : DLLEventTemplate() { Init("onPlantDyingFromDisappearing"); };
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.test_al_al().jnz_rel(7).popad().push_imm32(0x46319B).ret().popad().push_reg(REG_EDI).invoke(0x4679B0).pop(REG_EDI).pop(REG_ESI).pop(REG_ECX).ret();
		}
	};
	/// @brief 钢地刺被碾压扣血至死的事件
	/// @attention 由于地刺王死亡时同时还会触发HP<0的亡语事件，要注意亡语重复的问题。
	/// @param 触发事件的植物。
	/// @return 该植物是否被移除。
	class SpikerockDyingFromSmashedEvent : public DLLEventTemplate<0x45ECEC, 6, CONST_VAL(DYING_SPIKEROCK_SMASHED), REG_ESI>
	{
	public:
		SpikerockDyingFromSmashedEvent(const char* str) : DLLEventTemplate() { Init(str); };
		SpikerockDyingFromSmashedEvent(int address) : DLLEventTemplate() { Init(address); };
		SpikerockDyingFromSmashedEvent() : DLLEventTemplate() { Init("onSpikerockDyingFromSmashed"); };
	protected:
		virtual void InitExtra(AsmBuilder& builder)
		{
			builder.test_al_al().jnz_rel(7).popad().push_imm32(0x45ECF2).ret().popad().push_reg(REG_ESI).invoke(0x4679B0).pop(REG_EDI).pop(REG_EBX).pop(REG_ECX).ret();
		}
	};
	/// @brief 植物因被辣椒僵尸烧死而被移除事件。
	/// @param 触发事件的植物。
	/// @return 该植物是否被移除。
	class PlantDyingFromJalapenoHeadEvent : public BoolDLLEventTemplate<0x527722, 6, 0x52772E, CONST_VAL(DYING_JALAPENOHEAD_EXPLODED), REG_EAX>
	{
	public:
		PlantDyingFromJalapenoHeadEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		PlantDyingFromJalapenoHeadEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		PlantDyingFromJalapenoHeadEvent() : BoolDLLEventTemplate() { Init("onPlantDyingFromJalapenoHead"); };
	};

	/// @brief 植物在游戏中因各种原因而被移除的事件。
	/// @param 触发事件的植物、死亡原因类型PlantDyingType。
	/// @return 该植物是否被移除。
	class PlantDyingEvent
	{
	private:
		PlantDyingFromLowHealthEvent* part1;
		PlantDyingFromEatenEvent* part2;
		PlantDyingFromDisappearingEvent* part3;
		//亡语重复问题，暂时禁用该事件
		//SpikerockDyingFromSmashedEvent* part4;
		PlantDyingFromJalapenoHeadEvent* part5;
	public:
		PlantDyingEvent()
		{
			PlantDyingEvent(PVZ::Memory::GetProcAddress("onPlantDying"));
		}
		PlantDyingEvent(int address)
		{
			part1 = new PlantDyingFromLowHealthEvent(address);
			part2 = new PlantDyingFromEatenEvent(address);
			part3 = new PlantDyingFromDisappearingEvent(address);
			//part4 = new SpikerockDyingFromSmashedEvent(address);
			part5 = new PlantDyingFromJalapenoHeadEvent(address);
		}
		void end()
		{
			part1->end();
			part2->end();
			part3->end();
			//part4->end();
			part5->end();
		}
	};

	/// @brief 车类僵尸碾压结算事件。
	/// @param 触发事件的僵尸
	/// @return 是否结算原版碾压过程。
	class ZombieCheckSquishEvent : public BoolDLLEventTemplate<0x52EDB0, 6, 0x52EEEC, MEM_ESP_ADD(0x20)>
	{
	public:
		ZombieCheckSquishEvent(const char* str) : BoolDLLEventTemplate() { Init(str); };
		ZombieCheckSquishEvent(int address) : BoolDLLEventTemplate() { Init(address); };
		ZombieCheckSquishEvent() : BoolDLLEventTemplate() { Init("onZombieCheckSquish"); };
	};

	/// @brief 子弹击中判定分支事件。
	/// @warning 该事件与 ProjectileCollisionEvent 冲突，
	/// @param 触发事件的子弹
	/// @retval true 结算对植物的索敌和击中效果。
	/// @retval false 结算对僵尸的索敌和击中效果。
	class ProjectileHitDiversionEvent : public DiversionEventTemplate<0x46CFC9, 6, 0x46CFCF, 0x46D058, REG_EBP>
	{
	public:
		ProjectileHitDiversionEvent(const char* str) : DiversionEventTemplate() { Init(str); };
		ProjectileHitDiversionEvent(int address) : DiversionEventTemplate() { Init(address); };
		ProjectileHitDiversionEvent() : DiversionEventTemplate() { Init("onProjectileHitDiversion"); };
	};

	/// @brief 机枪僵尸判定是否发射子弹事件事件。
	/// @note 不会影响射击动作，只影响子弹生成。
	/// @param 触发事件的僵尸
	/// @retval true 是否发射子弹
	class GatlingZombieJudgeShootEvent : public DiversionEventTemplate<0x5277D9, 5, 0x52783D, 0x5277ED, REG_EDI>
	{
	public:
		GatlingZombieJudgeShootEvent(const char* str) : DiversionEventTemplate() { Init(str); };
		GatlingZombieJudgeShootEvent(int address) : DiversionEventTemplate() { Init(address); };
		GatlingZombieJudgeShootEvent() : DiversionEventTemplate() { Init("onGatlingZombieJudgeShoot"); };
	};

	/// @brief 巨人扔小鬼事件
	/// @param 触发事件的僵尸，僵尸投掷的小鬼
	class GargantaurThrowAfterEvent : public DLLEventTemplate<0x527148, 7, REG_ESI, MEM_ESP_ADD(0x38)>
	{
	public:
		GargantaurThrowAfterEvent(const char* str) : DLLEventTemplate() { Init(str); };
		GargantaurThrowAfterEvent(int address) : DLLEventTemplate() { Init(address); };
		GargantaurThrowAfterEvent() : DLLEventTemplate() { Init("onGargantaurThrowAfter"); };
	};
};