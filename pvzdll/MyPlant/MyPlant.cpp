#include "MyPlant.hpp"
#include "implement/Index.hpp"

namespace PlantAbility
{
	PlantAbility::PlantPTR pt_factory[] =
	{
		new BasePlant(), new Barley(),	  new NoXPPlant(), new WallNut(),
		new BasePlant(), new SnowPea(),	  new BasePlant(), new Repeater(),

		new PuffShroom(), new SunShroom(), new FumeShroom(), new BasePlant(),
		new HypnoShroom(), new ScaredyShroom(), new IceShroom(), new NoXPPlant(),

		new DiamondShroom(), new Squash(), new Threepeater(), new BasePlant(),
		new NoXPPlant(), new NoXPPlant(), new Torchwood(), new TallNut(),

		new SeaShroom(), new Plantern(),  new Cactus(),    new Blover(),
		new SplitPea(),  new BasePlant(), new BasePlant(), new MagnetShroom(),

		new CabbagePult(), new BasePlant(), new BasePlant(), new NoXPPlant(),
		new Garlic(),	 new UmbrellaLeaf(), new Marigold(), new MelonPult(),

		new BasePlant(), new RestingBarley(), new GloomShroom(), new Cattail(),
		new BasePlant(), new GoldMagnet(), new SpikeRock(), new BasePlant(),

		new BasePlant(), new Explode_O_Nut(), new BasePlant(), new Endoflame(),
		new LeftRepeater(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new SpringEquinoxVine(), new DarkShroom(), new Mangosteen(), new ThymeWarp(),
	};
}

PlantAbility::PlantPTR PlantAbility::GetPrototype(SeedType::SeedType type)
{
	return PlantAbility::pt_factory[type];
}

MyPlant MyPlant::GetOwner()
{
	if (!this->OwnerID)
		return *this;
	else
		return GetByID(this->OwnerID);
}

void MyPlant::SetOwner(MyPlant plant)
{
	this->OwnerID = plant.Id;
}

void MyPlant::SetMaxHealth(int val)
{
	auto ori_val = this->MaxHp;
	this->MaxHp = val;
	this->Hp += val - ori_val;
}

bool MyPlant::IsToolPlant()
{
	auto type = this->Type;
	return type == SeedType::Iceshroom
		|| type == SeedType::TangleKelp
		|| type == SeedType::Torchwood
		|| type == SeedType::UmbrellaLeaf
		|| type == SeedType::CobCannon;
}

bool MyPlant::IsPrime()
{
	return !(this->FromBarley || this->OwnerID);
}

bool MyPlant::IsXPRecipient()
{
	return PlantAbility::GetPrototype(this->Type)->IsXPRecipient(*this);
}

bool MyPlant::CanUpgrade()
{
	auto level = this->Level;
	return level < this->MAX_LEVEL&& PlantAbility::PLANT_LEVEL_EXP[this->Type][level] != 0;
}

void MyPlant::Upgrade()
{
	auto anim = Creator::CreateReanimation(static_cast<AnimationType::AnimationType>(0x96), (float)this->ImageY, (float)this->ImageX, 0x61a80);
	anim.Play("anim_idle", 0, 2, 30.0);

	if (this->IsToolPlant())
		this->Level = MyPlant::MAX_LEVEL;
	else
		this->Level++;
	if (this->Level == MyPlant::MAX_LEVEL)
	{
		if (Creator::Rand(10))
			this->EnableEasterSkin();
		else
		{
			auto model = this->GetAnimationPart1();
			if (model.isValid())
			{
				model.AssignRenderGroupToPrefix(-1, "common");
				model.AssignRenderGroupToPrefix(0, "awake");
			}

			model = this->GetAnimationPart2();
			if (model.isValid())
			{
				model.AssignRenderGroupToPrefix(-1, "common");
				model.AssignRenderGroupToPrefix(0, "awake");
			}

			model = this->GetAnimationPart3();
			if (model.isValid())
			{
				model.AssignRenderGroupToPrefix(-1, "common");
				model.AssignRenderGroupToPrefix(0, "awake");
			}

			model = this->GetAnimationPart4();
			if (model.isValid())
			{
				model.AssignRenderGroupToPrefix(-1, "common");
				model.AssignRenderGroupToPrefix(0, "awake");
			}	
		}
	}

	PlantAbility::GetPrototype(this->Type)->onUpgrade(*this);

	this->Heal(this->MaxHp / 5);
}

void MyPlant::Heal(int val)
{
	this->Hp += val;
	if (this->Hp > this->MaxHp)
		this->Hp = this->MaxHp;
}

bool MyPlant::CheckUpgrade()
{
	if (this->IsToolPlant())
	{

	}
	else
		if (this->CanUpgrade() && this->Experience >= PlantAbility::PLANT_LEVEL_EXP[this->Type][this->Level])
		{
			this->Upgrade();
			return true;
		}
	return false;
}

void MyPlant::AddExperience(int val, bool kill_credit)
{
	this->Experience += val;
	PlantAbility::GetPrototype(this->Type)->onGainXP(*this, val, kill_credit);

	if (kill_credit)
		this->Light();

	if (!this->IsToolPlant())
		while (this->CheckUpgrade()) {};
}

void MyPlant::EnableEasterSkin()
{
	this->EasterSkin = true;

	auto model = this->GetAnimationPart1();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "\0");
		model.AssignRenderGroupToPrefix(0, "easter");
		model.AssignRenderGroupToPrefix(0, "easter2");
	}

	model = this->GetAnimationPart2();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "\0");
		model.AssignRenderGroupToPrefix(0, "easter");
	}

	model = this->GetAnimationPart3();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "\0");
		model.AssignRenderGroupToPrefix(0, "easter");
	}

	model = this->GetAnimationPart4();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "\0");
		model.AssignRenderGroupToPrefix(0, "easter");
	}

	model = this->GetAnimationPotatoGlow();
	if (model.isValid())
		model.AssignRenderGroupToPrefix(-1, "\0");
}

MyPlant MyPlant::GetByID(int id)
{
	if (id)
	{
		MyPlant tmp = MyPlant(ID_INDEX(id));
		if (tmp.Id != id)
			return MyPlant(INVALID_BASEADDRESS);
		else
			return tmp;
	}
	else
		return MyPlant(INVALID_BASEADDRESS);
}