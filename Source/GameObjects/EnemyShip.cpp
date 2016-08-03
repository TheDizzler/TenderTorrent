#include "EnemyShip.h"

EnemyShip::EnemyShip() :Sprite(SimpleMath::Vector2(0, 0)) {

}

EnemyShip::EnemyShip(const Vector2 & position) : Sprite(position) {


}

EnemyShip::~EnemyShip() {
}


void EnemyShip::update(double deltaTime) {

	Sprite::update(deltaTime);
}

void EnemyShip::draw(SpriteBatch* batch, Sprite* baseShip) {

	batch->Draw(baseShip->texture.Get(), position, &(baseShip->sourceRect), tint, rotation, baseShip->origin, scale, SpriteEffects_None, layerDepth);

}

bool EnemyShip::readyToFire() {

	if (fireReady) {
		fireReady = false;
		return true;
	}

	return false;
}

EnemyBullet * EnemyShip::launchBullet(Vector2 target) {

	EnemyBullet* bullet = new EnemyBullet(weaponLocation);
	Vector2 direction = target - weaponLocation;
	direction.Normalize();
	bullet->direction = direction;
	bullet->isAlive = true;

	return bullet;
}

void EnemyShip::takeDamage(int damageTaken) {

	health -= damageTaken;

	isAlive = health > 0;
}


