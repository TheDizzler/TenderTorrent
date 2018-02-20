#include "../pch.h"
#include "Turret.h"
#include "SunBullet.h"


Turret::Turret(Vector2 locationOffset) : WeaponSystem(locationOffset) {

	maxStoreSize = 12;
	coolDownTime = .25;
}

Turret::~Turret() {
}

void Turret::loadTurretTexture(GraphicsAsset* turretAsset) {

	turretSprite.load(turretAsset);
	turretSprite.setPosition(weaponLocation);
}


Bullet* Turret::fire() {

	SunBullet* bullet = static_cast<SunBullet*>(WeaponSystem::fire());
	bullet->direction = turretDirection;
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
	WeaponSystem::draw(batch);
}

void Turret::setTint(const XMFLOAT4 newTint) {
	turretSprite.setTint(newTint);
}

void Turret::setScale(const Vector2& newScale) {
	turretSprite.setScale(newScale);
}
