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

		new SeaShroom(), new Plantern(),  new Cactus(),    new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new MagnetShroom(),

		new CabbagePult(), new BasePlant(), new BasePlant(), new NoXPPlant(),
		new Garlic(),	 new UmbrellaLeaf(), new Marigold(), new MelonPult(),

		new BasePlant(), new NoXPPlant(), new GloomShroom(), new Cattail(),
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
	return this->Id == this->GetOwner().Id;
}

bool MyPlant::IsXPRecipient()
{
	return PlantAbility::GetPrototype(this->Type)->IsXPRecipient(*this);
}

void MyPlant::Heal(int val)
{
	this->Hp += val;
	if (this->Hp > this->MaxHp)
		this->Hp = this->MaxHp;
}

void MyPlant::AddExperience(int val, bool kill_credit)
{
	this->Experience += val;

	if(kill_credit)
		this->Light();
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