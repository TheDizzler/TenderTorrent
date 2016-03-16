#include "Turret.h"

Turret::Turret(Vector2 locationOffset) : WeaponSystem(locationOffset) {

	maxStoreSize = 12;
	coolDownTime = .25;
}

Turret::~Turret() {
}

bool Turret::loadTurretTexture(ID3D11Device * device, const wchar_t * textureFile) {

	turretSprite.reset(new Sprite(weaponLocation));
	if (!turretSprite->load(device, textureFile)) {
		MessageBox(NULL, L"Failed to load Turret", L"ERROR", MB_OK);
		return false;
	}


	return true;
}


Bullet* Turret::fire() {

	SunBullet* bullet = static_cast<SunBullet*>(WeaponSystem::fire());
	bullet->direction = turretDirection;
	return bullet;
}

void Turret::fillBulletStore() {

	for (int i = 0; i < maxStoreSize; ++i) {
		Bullet* bullet = new SunBullet(weaponStore);
		bullet->setDimensions(baseBulletSprite.get());
		bulletStore.push_back(bullet);
	}
}


void Turret::update(double deltaTime, Vector2 positionUpdate, const Vector2& mousePosition) {

	WeaponSystem::update(deltaTime, positionUpdate);
	turretSprite->position = weaponLocation;

	int y = mousePosition.y - weaponLocation.y;
	int x = mousePosition.x - weaponLocation.x;
	turretDirection = Vector2(x, y);
	turretDirection.Normalize();
	turretSprite->rotation = atan2(y, x) + XM_PIDIV2;
}

void Turret::draw(SpriteBatch * batch) {

	turretSprite->draw(batch);
	WeaponSystem::draw(batch);
}
