#include "../../pch.h"
#include "LaserSystem.h"
#include "../../Managers/GameManager.h"

LaserSystem::LaserSystem(Vector2 position) : WeaponSystem(position) {

	setWeaponStats(10, 0, .21);
}

LaserSystem::~LaserSystem() {
}


void LaserSystem::fillBulletStore(Animation* bulletAsset) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].load(bulletAsset);
	}
}

void LaserSystem::setWeaponStats(int damage, int nrgCst, double coolTime) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].setDamage(damage);
	}
	energyCost = nrgCst;
	coolDownTime = coolTime;
}


void LaserSystem::draw(SpriteBatch* batch) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bulletStore[i].isAlive)
			bulletStore[i].draw(batch);
	}
}


bool LaserSystem::ready() {
	if (timeSinceFired > 0)
		return false;
	if (bulletStore[nextBullet].isAlive == false)
		return true;

	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bulletStore[i].isAlive == false) {
			nextBullet = i;
			return true;
		}
	}
	return false;
}

Bullet* LaserSystem::fire() {

	Bullet* bullet = &bulletStore[nextBullet++];
	bullet->setPosition(weaponLocation);
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	if (nextBullet >= MAX_BULLETS)
		nextBullet = 0;

	return bullet;
}





