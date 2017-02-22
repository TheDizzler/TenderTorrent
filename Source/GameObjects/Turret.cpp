#include "Turret.h"

#include "SunBullet.h"


Turret::Turret(Vector2 locationOffset) : WeaponSystem(locationOffset) {

	maxStoreSize = 12;
	coolDownTime = .25;
}

Turret::~Turret() {
}

void Turret::loadTurretTexture(GraphicsAsset* turretAsset) {

	turretSprite.reset(new Sprite(weaponLocation));
	turretSprite->load(turretAsset);
}


Bullet* Turret::fire() {

	SunBullet* bullet = static_cast<SunBullet*>(WeaponSystem::fire());
	bullet->direction = turretDirection;
	return bullet;
}

//void Turret::fillBulletStore(/*GraphicsAsset**/ shared_ptr<Animation> bulletAsset) {
//
//	for (int i = 0; i < maxStoreSize; ++i) {
//		Bullet* bullet = new SunBullet(weaponStore);
//		bullet->load(bulletAsset);
//		bulletStore.push_back(bullet);
//	}
//}


void Turret::setTurretRotation(const Vector2 & targetDirection) {
	turretSprite->setRotation(atan2(targetDirection.y, targetDirection.x) + XM_PIDIV2);
	turretDirection = Vector2(targetDirection.x, targetDirection.y);
	turretDirection.Normalize();
}

void Turret::update(double deltaTime, Vector2 positionUpdate) {

	WeaponSystem::update(deltaTime, positionUpdate);
	turretSprite->setPosition(weaponLocation);

	/*int y = mousePosition.y - weaponLocation.y;
	int x = mousePosition.x - weaponLocation.x;
	turretDirection = Vector2(x, y);
	turretDirection.Normalize();
	turretSprite->setRotation(atan2(targetDirection.y, targetDirection.x) + XM_PIDIV2);*/
}

void Turret::draw(SpriteBatch * batch) {

	turretSprite->draw(batch);
	WeaponSystem::draw(batch);
}

void Turret::setTint(const XMFLOAT4 newTint) {
	turretSprite->setTint(newTint);
}

void Turret::setScale(const Vector2& newScale) {
	turretSprite->setScale(newScale);
}
