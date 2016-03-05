#pragma once

#include "WeaponSystem.h"
#include "LaserBolt.h"


class LaserSystem : public WeaponSystem {
public:
	LaserSystem(Vector2 position);
	~LaserSystem();

	//virtual bool loadWeaponTexture(ID3D11Device* device, const wchar_t* textureFile);


private:


};