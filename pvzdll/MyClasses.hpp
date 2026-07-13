#pragma once
#include <vector>
#include "MyBoard.hpp"
#include "MyPlant/MyPlant.hpp"
#include "MyProjectile/MyProjectile.hpp"
#include "MyZombie/MyZombie.hpp"

typedef PVZEvent::PlantDamageZombieEvent::PZDamageInfo<MyPlant, MyZombie> PZDamageEvent;

// 需要在不同Class的Events.cpp中相互调用的函数，在此处声明。

/// @brief 植物对僵尸造成伤害前的事件触发函数，在此修改伤害。
void onPlantDamageZombie(PZDamageEvent* info);
int onPlantTakeDamage(MyPlant plant, PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage);
/// @todo 将此函数改为 MyProjectile 的成员函数
void onFireballInitColor(MyProjectile proj, PVZ::Animation anim);

void DestroyPString(Draw::PString string);

MyPlant NoiselessCreatePlant(SeedType::SeedType type, int row, int col, SeedType::SeedType ImitaterType = SeedType::None);
/// @attention 创建子弹后，注意设置DamageAbility
MyProjectile MyCreateProjectile(ProjectileType::ProjectileType type, int row, int layer, int x, int y);

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

	/// @brief 植物对僵尸造成伤害
	void ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags = PVZ::DAMAGEF_NONE);
	/// @brief 僵尸对植物造成伤害
	bool ApplyZPDamage(MyZombie zombie, MyPlant plant, int damage);

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


namespace Debug
{
	/// @brief 浮动数字类信息容器模板
	template<typename T,std::size_t N>
	class DebugPopDigitVector
	{
	public:
		void Add(const T& newdata)
		{
			std::vector<T>& data = this->Data;
			//管理Data的大小，不定期清理死亡的info
			if (data.size() >= this->DataRefreshSize && this->DataRefreshCount <= 0)
			{
				this->Refresh();
				this->DataRefreshCount = this->DataRefreshSize + 1;
			}

			data.push_back(newdata);

			//刷新周期减少
			if (this->DataRefreshCount > 0)
				this->DataRefreshCount--;
		}
		auto begin() { return Data.begin(); }
		auto end() { return Data.end(); }
		virtual void Refresh() = 0;
		virtual ~DebugPopDigitVector() = default;
	protected:
		std::vector<T> Data;
	private:
		/// @brief 刷新周期，为0时新增数据才刷新
		std::size_t DataRefreshCount = 0;
		/// @brief 容器刷新大小
		std::size_t DataRefreshSize = N;
	};


	struct PZDamageDigit
	{
		int ExistedTime;
		bool isAlive;
		int X;
		int Y;
		int Damage;
	};

	class PZDamageDigitVector : public DebugPopDigitVector<PZDamageDigit, 1024>
	{
	public:
		virtual void Refresh()
		{
			auto& data = this->Data;
			data.erase(
				std::remove_if(data.begin(), data.end(),
					[](const PZDamageDigit& digit) {
						return !digit.isAlive;   // 删除 isAlive == false 的元素
					}),
				data.end()
						);
		}
	};

	struct PlantGetExpDigit
	{
		int ExistedTime;
		bool isAlive;
		int X;
		int Y;
		int Exp;
	};

	class PlantGetExpDigitVector : public DebugPopDigitVector<PlantGetExpDigit, 256>
	{
	public:
		virtual void Refresh()
		{
			auto& data = this->Data;
			data.erase(
				std::remove_if(data.begin(), data.end(),
					[](const PlantGetExpDigit& digit) {
						return !digit.isAlive;   // 删除 isAlive == false 的元素
					}),
				data.end()
						);
		}
	};

	class DebugInfo
	{
	public:
		PZDamageDigitVector m_PZDamageDigitVector;
		PlantGetExpDigitVector m_PlantGetExpDigitVector;
	};

	extern DebugInfo * MyDebugInfo;
}

namespace MyRand
{
	// 静态种子，只占用 4 字节
	inline static unsigned int s_seed = 123456789u;

	/// @brief 快速随机，范围为[inf,sup)
	/// @param sup 上限
	/// @param inf 下限
	inline int FastRand(int inf,int sup) {
		// 经典 ANSI C 标准 LCG 常量
		s_seed = s_seed * 1103515245u + 12345u;
		// 取模得到 [0,sup-inf)，再+inf得到[inf,sup)
		return static_cast<int>(s_seed % (sup - inf)) + inf;
	}
}