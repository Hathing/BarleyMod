#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Pea : public BaseProjectile
	{
	public:
		inline static const int damage[6] = { 20, 20, 20, 20, 20, 20 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			int dmg = 20;
			if (proj.SpecialType == PST_SCATTER_PEA)
				dmg /= 2;
			return dmg;
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialType == PST_SCATTER_PEA)
				return original_val / 2.0f;
			return original_val;
		}
		bool onImpact(MyProjectile proj, MyZombie zombie)
		{
			//爆裂子弹的效果
			if (proj.SpecialType == PST_CRACK_PEA)
			{
				int crack_num = 2 + proj.SourceLevel / 2;//这个数是循环用的，实际爆裂子弹数量 = 1 + crack_num * 2
				int x = proj.X, y = proj.Y;
				constexpr float v = 5.0f;
				//水平的子弹
				MyProjectile newproj{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
				newproj.DeriveProperty(proj);
				newproj.MakePiercing(3, v, 0.0f);
				newproj.SpecialType = PST_SCATTER_PEA;
				//斜向的子弹
				for (int i = crack_num; i > 0; i--)
				{
					float rad = 1.14f * i / crack_num;
					float cos = std::cosf(rad);
					float sin = std::sinf(rad);

					MyProjectile newproj_{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
					newproj_.DeriveProperty(proj);
					//newproj.Row = min(proj.Row + 1, (proj.GetBoard().SixRoute ? 6 : 5));
					newproj_.MakePiercing(3, v * cos, v * sin);
					newproj_.SpecialType = PST_SCATTER_PEA;

					MyProjectile newproj__{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
					newproj__.DeriveProperty(proj);
					//newproj__.Row = max(0, proj.Row - 1);
					newproj__.MakePiercing(3, v * cos, v * sin * -1);
					newproj__.SpecialType = PST_SCATTER_PEA;
				}
			}
			return true;
		}
	};
}
