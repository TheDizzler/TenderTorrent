#include "WeaponSystem.h"

WeaponSystem::WeaponSystem() {
}

WeaponSystem::WeaponSystem(Vector2 position) {

	weaponLocation = position;

	

}

WeaponSystem::~WeaponSystem() {
}

bool WeaponSystem::loadWeaponTexture(ID3D11Device * device, const wchar_t* textureFile) {

	baseBulletSprite.reset(new Sprite());
	if (!baseBulletSprite->load(device, textureFile)) {
		MessageBox(NULL, L"Failed to load bullet", L"ERROR", MB_OK);
		return false;
	}


	for (int i = 0; i <= 24; ++i) {
		Bullet* bullet = new Bullet(weaponStore);
		//bullet->setHitArea(baseBulletSprite->getHitArea());
		bullet->setDimensions(baseBulletSprite.get());
		bulletStore.push_back(bullet);

	}


	return true;
}


void WeaponSystem::setWeaponStats(int nrgCst, float coolTime) {

	energyCost = nrgCst;
	coolDownTime = coolTime;
}

/* Live bullets are updated in WaveManager.
	Only live bullets need to be updated. */
void WeaponSystem::update(double deltaTime, Vector2 positionUpdate) {

	weaponLocation = positionUpdate;
	timeSinceFired -= deltaTime;


	/*for (Bullet* bullet : bullets) {
		bullet->update(deltaTime);
	}*/
}

void WeaponSystem::draw(SpriteBatch * batch) {

	for (Bullet* bullet : bulletStore) {
		if (bullet->isAlive)
			bullet->draw(batch, baseBulletSprite.get());
	}
}



bool WeaponSystem::ready() {

	int size = bulletStore.size();
	if (nextBullet >= size || bulletStore[nextBullet]->isAlive == true) {
		//nextBullet = 0;
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
	bullet->position = weaponLocation;
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	return bullet;
}
