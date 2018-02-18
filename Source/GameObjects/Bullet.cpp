#include "../pch.h"
#include "Bullet.h"
#include "../Engine/GameEngine.h"

Bullet::Bullet() :AnimatedSprite(weaponStore) {
	isAlive = false;
}

Bullet::Bullet(const Vector2 &position) : AnimatedSprite(position) {

	isAlive = false;
}

Bullet::~Bullet() {
}

void Bullet::setDamage(int newDamage) {
	damage = newDamage;
}

void Bullet::update(double deltaTime) {

	double deltaSpeed = deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*(float) deltaSpeed;
	setPosition(newposition);
	timeAlive += deltaTime;

	//if (position.y < 0) {
	if (!camera.viewContains(position)) {
		isAlive = false;
		position = weaponStore;
		timeAlive = 0;
	}

	AnimatedSprite::update(deltaTime);
}


