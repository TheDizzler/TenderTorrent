#include "../../pch.h"
#include "Bullet.h"
#include "../../Engine/GameEngine.h"

Bullet::Bullet() : AnimatedSprite(weaponStore) {
	isAlive = false;
	load(gfxAssets.getAnimation("Cross Bullet"));
}


Bullet::~Bullet() {
}

void Bullet::setDamage(int newDamage) {
	damage = newDamage;
}

void Bullet::update(double deltaTime) {

	double deltaSpeed = deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*(float) deltaSpeed;
	setPosition(newposition - camera.getDelta());
	timeAlive += deltaTime;

	if (!camera.viewContains(position)) {
		isAlive = false;
	}

	if (!isAlive) {
		setPosition(weaponStore);
		currentFrameTime = 0;
		timeAlive = 0;
	}
	AnimatedSprite::update(deltaTime);
}


