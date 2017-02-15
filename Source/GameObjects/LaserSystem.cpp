#include "LaserSystem.h"

LaserSystem::LaserSystem(Vector2 position) : WeaponSystem(position) {
	setWeaponStats(25, 5, 1);
}

LaserSystem::~LaserSystem() {
}


