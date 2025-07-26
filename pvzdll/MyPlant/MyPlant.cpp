#include "MyPlant.hpp"

/*
namespace PlantPrototype
{
	PlantPrototype::PrototypePTR pt_factory[] =
	{
		new BasePrototype()
	};
}

PlantPrototype::PrototypePTR PlantPrototype::GetPrototype(SeedType::SeedType type)
{
	return PlantPrototype::pt_factory[type];
}*/

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