#include "EnemyShip.h"

EnemyShip::EnemyShip() : Sprite(SimpleMath::Vector2(0, 0)) {

	isAlive = false;
}

EnemyShip::EnemyShip(const Vector2& position) : Sprite(position) {

	isAlive = false;
}

EnemyShip::~EnemyShip() {

	for (Bullet* bullet : bulletStore)
		delete bullet;

	bulletStore.clear();
}


void EnemyShip::update(double deltaTime) {
	Sprite::update(deltaTime);
}

//void EnemyShip::draw(SpriteBatch* batch, Sprite* baseShip) {
//
//	batch->Draw(baseShip->getTexture().Get(), position, &(baseShip->getRect()),
//		tint, rotation, baseShip->getOrigin(), scale, SpriteEffects_None, layerDepth);
//}

bool EnemyShip::readyToFire() {

	if (fireReady) {
		fireReady = false;
		return true;
	}

	return false;
}

EnemyBullet* EnemyShip::launchBullet(Vector2 target) {

	EnemyBullet* bullet = bulletStore[nextBulletInStore++];
	bullet->setPosition(weaponLocation);
	Vector2 direction = target - weaponLocation;
	direction.Normalize();
	bullet->direction = direction;
	bullet->isAlive = true;

	if (nextBulletInStore >= bulletStore.size())
		nextBulletInStore = 0;
	return bullet;
}

void EnemyShip::takeDamage(int damageTaken) {

	health -= damageTaken;

	isAlive = health > 0;
}


