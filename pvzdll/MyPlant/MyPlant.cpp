#include "MyPlant.hpp"
#include "implement/Index.hpp"

namespace PlantAbility
{
	PlantAbility::PlantPTR pt_factory[] =
	{
		new BasePlant(), new BasePlant(), new BasePlant(), new WallNut(),
		new BasePlant(), new SnowPea(),	  new BasePlant(), new Repeater(),

		new PuffShroom(), new SunShroom(), new FumeShroom(), new BasePlant(),
		new HypnoShroom(), new ScaredyShroom(), new IceShroom(), new BasePlant(),

		new DiamondShroom(), new Squash(), new Threepeater(), new BasePlant(),
		new BasePlant(), new BasePlant(), new Torchwood(), new TallNut(),

		new SeaShroom(), new Plantern(),  new Cactus(),    new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new MagnetShroom(),

		new CabbagePult(), new BasePlant(), new BasePlant(), new BasePlant(),
		new Garlic(),	 new UmbrellaLeaf(), new Marigold(), new MelonPult(),

		new BasePlant(), new BasePlant(), new GloomShroom(), new Cattail(),
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

bool MyPlant::IsToolPlant()
{
	auto type = this->Type;
	return type == SeedType::Iceshroom
		|| type == SeedType::TangleKelp
		|| type == SeedType::Torchwood
		|| type == SeedType::UmbrellaLeaf
		|| type == SeedType::CobCannon;
}

void MyPlant::Heal(int val)
{
	this->Hp += val;
	if (this->Hp > this->MaxHp)
		this->Hp = this->MaxHp;
}

void MyPlant::AddExperience(int val, bool kill_credit)
{
	if(kill_credit)
		this->Light();
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