#include "MyPlant.hpp"
#include "implement/Index.hpp"

namespace PlantAbility
{
	PlantAbility::PlantPTR pt_factory[] =
	{
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new PuffShroom(), new BasePlant(), new FumeShroom(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new SeaShroom(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new GoldMagnet(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),

		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
		new BasePlant(), new BasePlant(), new BasePlant(), new BasePlant(),
	};
}

PlantAbility::PlantPTR PlantAbility::GetPrototype(SeedType::SeedType type)
{
	return PlantAbility::pt_factory[type];
}

void MyPlant::Heal(int val)
{
	this->Hp += val;
	if (this->Hp > this->MaxHp)
		this->Hp = this->MaxHp;
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