#include "../pch.h"
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


void WeaponSystem::fillBulletStore(Animation* bulletAsset) {

	for (int i = 0; i < maxStoreSize; ++i) {
		Bullet* bullet = new Bullet(weaponStore);
		bullet->load(bulletAsset);
		bulletStore.push_back(bullet);
	}
}


void WeaponSystem::setWeaponStats(int damage, int nrgCst, double coolTime) {

	for (Bullet* bullet : bulletStore)
		bullet->setDamage(damage);

	energyCost = nrgCst;
	coolDownTime = coolTime;
}

/* Live bullets are updated in WaveManager.
	Only live bullets need to be updated. */
void WeaponSystem::update(double deltaTime, Vector2 positionUpdate) {

	weaponLocation = Vector2(positionUpdate.x + locationOffset.x,
		positionUpdate.y + locationOffset.y);
	timeSinceFired -= deltaTime;

}

void WeaponSystem::draw(SpriteBatch* batch) {

	for (Bullet* bullet : bulletStore) {
		if (bullet->isAlive)
			bullet->draw(batch);
	}
}

bool WeaponSystem::ready() {

	size_t size = bulletStore.size();
	if (nextBullet >= size || bulletStore[nextBullet]->isAlive == true) {
		for (int i = 0; i < size; ++i) {
			if (bulletStore[i]->isAlive == false) {
				nextBullet = i;
				return timeSinceFired <= 0;
			}
		}
		return false;
	}
	return timeSinceFired <= 0;
}


Bullet* WeaponSystem::fire() {

	Bullet* bullet = bulletStore[nextBullet++];
	bullet->setPosition(weaponLocation);
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	return bullet;
}

const Vector2& WeaponSystem::getPosition() const {
	return weaponLocation;
}

void WeaponSystem::setTint(const XMFLOAT4 colr) {
}
