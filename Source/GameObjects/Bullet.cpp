#include "Bullet.h"

Bullet::Bullet() :AnimatedSprite(weaponStore) {
	isAlive = false;
}

Bullet::Bullet(const Vector2 &position) : AnimatedSprite(position) {

	/*hitArea.reset(new HitArea(position, Vector2(getWidth(), getHeight())));*/
	isAlive = false;
}

Bullet::~Bullet() {
}

void Bullet::update(double deltaTime) {

	float deltaSpeed = deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*deltaSpeed;
	setPosition(newposition);
	timeAlive += deltaTime;

	if (position.y < 0) {
		isAlive = false;
		position = weaponStore;
		timeAlive = 0;
	}

	AnimatedSprite::update(deltaTime);
}


