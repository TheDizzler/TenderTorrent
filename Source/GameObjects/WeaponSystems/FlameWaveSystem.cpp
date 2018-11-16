#include "FlameWaveSystem.h"


FlameWaveSystem::FlameWaveSystem(Vector2 pos) : WeaponSystem(pos) {

	setWeaponStats(1, 0, .11);
}

FlameWaveSystem::~FlameWaveSystem() {
}

void FlameWaveSystem::setWeaponStats(int dmg, int nrgCst, double coolTime) {

	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].setDamage(dmg);
	}
	energyCost = nrgCst;
	coolDownTime = coolTime;
}


void FlameWaveSystem::fillBulletStore(Animation* bulletAsset) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].load(bulletAsset);
	}
}

bool FlameWaveSystem::ready() {
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

Bullet* FlameWaveSystem::fire() {
	Bullet* bullet = &bulletStore[nextBullet++];
	bullet->setPosition(weaponLocation);
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	if (nextBullet >= MAX_BULLETS)
		nextBullet = 0;

	return bullet;
}

void FlameWaveSystem::draw(SpriteBatch* batch) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bulletStore[i].isAlive)
			bulletStore[i].draw(batch);
	}
}

