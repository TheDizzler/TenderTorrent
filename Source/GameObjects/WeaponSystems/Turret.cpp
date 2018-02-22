#include "../../pch.h"
#include "Turret.h"
#include "SunBullet.h"
#include "../../Managers/GameManager.h"


Turret::Turret(Vector2 locationOffset) : WeaponSystem(locationOffset) {

	//maxStoreSize = 12;
	//coolDownTime = .25;

	loadTurretTexture(gfxAssets.getAsset("PlayerShip Turret"));
	setWeaponStats(5, 5, .25);
}

Turret::~Turret() {
}

void Turret::loadTurretTexture(GraphicsAsset* turretAsset) {

	turretSprite.load(turretAsset);
	turretSprite.setPosition(weaponLocation);
}


void Turret::fillBulletStore(Animation* bulletAsset) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].load(bulletAsset);
	}
}

void Turret::setWeaponStats(int damage, int nrgCst, double coolTime) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bulletStore[i].setDamage(damage);
	}
	energyCost = nrgCst;
	coolDownTime = coolTime;
}

bool Turret::ready() {
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


Bullet* Turret::fire() {

	Bullet* bullet = &bulletStore[nextBullet++];
	bullet->setPosition(weaponLocation);
	bullet->isAlive = true;
	timeSinceFired = coolDownTime;

	bullet->direction = turretDirection;

	if (nextBullet >= MAX_BULLETS)
		nextBullet = 0;
	return bullet;
}


void Turret::setTurretRotation(const Vector2& targetDirection) {
	turretSprite.setRotation(atan2(targetDirection.y, targetDirection.x) + XM_PIDIV2);
	turretDirection = Vector2(targetDirection.x, targetDirection.y);
	turretDirection.Normalize();
}


void Turret::update(double deltaTime, Vector2 positionUpdate) {

	WeaponSystem::update(deltaTime, positionUpdate);
	turretSprite.setPosition(weaponLocation);
}

void Turret::draw(SpriteBatch* batch) {

	turretSprite.draw(batch);
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bulletStore[i].isAlive)
			bulletStore[i].draw(batch);
	}
}

void Turret::setTint(const XMFLOAT4 newTint) {
	turretSprite.setTint(newTint);
}

void Turret::setScale(const Vector2& newScale) {
	turretSprite.setScale(newScale);
}

