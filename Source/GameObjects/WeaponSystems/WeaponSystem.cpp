#include "../../pch.h"
#include "WeaponSystem.h"

WeaponSystem::WeaponSystem(Vector2 offset) {

	locationOffset = offset;
	weaponLocation = Vector2(locationOffset.x, locationOffset.y);
}

WeaponSystem::~WeaponSystem() {

}


void WeaponSystem::loadBulletTexture(Animation* bulletAsset) {

	fillBulletStore(bulletAsset);
}



/* Live bullets are updated in WaveManager.
	Only live bullets need to be updated. */
void WeaponSystem::update(double deltaTime, Vector2 positionUpdate) {

	weaponLocation = Vector2(positionUpdate.x + locationOffset.x,
		positionUpdate.y + locationOffset.y);
	timeSinceFired -= deltaTime;

}


const Vector2& WeaponSystem::getPosition() const {
	return weaponLocation;
}

void WeaponSystem::setTint(const XMFLOAT4 colr) {

}
