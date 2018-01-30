#pragma once

#include "WeaponSystem.h"
//#include "LaserBolt.h"

/** !!!!!!!!!!!!!!
	TODO:
		Put different WeaponSystems in XML files for live editing.
	!!!!!!!!!!!!!!*/
class LaserSystem : public WeaponSystem {
public:
	LaserSystem(Vector2 position);
	virtual ~LaserSystem();

private:


};