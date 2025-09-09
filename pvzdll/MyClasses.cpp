#include "MyClasses.hpp"

void PVZ::ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags)
{
	PZDamageEvent event{ zombie, plant, flags, damage, PVZEvent::PLANTDAMAGETYPE_CUSTOM};
	onPlantDamageZombie(&event);
	event.zombie.Hit(event.damage, event.flags);
}
