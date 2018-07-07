#include "../../pch.h"
#include "BasicGun.h"
#include "../../Managers/GameManager.h"

BasicGun::BasicGun(Vector2 locOffset) : WeaponSystem(locOffset) {

	setWeaponStats(1, 0, .1);

}

BasicGun::~BasicGun() {
}


void BasicGun::fillBulletStore(Animation* bulletAsset) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].load(bulletAsset);
	}
}


void BasicGun::setWeaponStats(int damage, int nrgCst, double coolTime) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].setDamage(damage);
	}
	energyCost = nrgCst;
	coolDownTime = coolTime;
}


void BasicGun::draw(SpriteBatch* batch) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bulletStore[i].isAlive)
			bulletStore[i].draw(batch);
	}
}

bool BasicGun::ready() {
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

Bullet* BasicGun::fire() {

	Bullet* bullet = &bulletStore[nextBullet++];
	bullet->setPosition(weaponLocation);
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	if (nextBullet >= MAX_BULLETS)
		nextBullet = 0;
	return bullet;
}


