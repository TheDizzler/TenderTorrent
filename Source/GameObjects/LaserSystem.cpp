#include "template.h"
#include "LaserSystem.h"

LaserSystem::LaserSystem(Vector2 position) : WeaponSystem(position) {

	setWeaponStats(5, 1);
}

LaserSystem::~LaserSystem() {
}


//bool LaserSystem::loadWeaponTexture(ID3D11Device * device, const wchar_t* textureFile) {
//
//	baseBulletSprite.reset(new Sprite());
//	if (!baseBulletSprite->load(device, textureFile)) {
//		MessageBox(NULL, L"Failed to load bullet", L"ERROR", MB_OK);
//		return false;
//	}
//
//	return true;
//}


