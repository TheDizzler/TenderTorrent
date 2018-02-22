#include "../../pch.h"
#include "SunBullet.h"
#include "../../globals.h"
#include "../../Engine/GameEngine.h"


SunBullet::SunBullet() : Bullet() {
	load(gfxAssets.getAnimation("Sun Bullet"));
	bulletSpeed = 350;
}

//SunBullet::SunBullet(const Vector2& position) : Bullet(position) {
//
//	damage = 20;
//	bulletSpeed = 350;
//}

SunBullet::~SunBullet() {
}


void SunBullet::update(double deltaTime) {

	/*float deltaSpeed = (float) deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*deltaSpeed;
	setPosition(newposition);*/


	//Bullet::update(deltaTime);



	double deltaSpeed = deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*(float) deltaSpeed;
	setPosition(newposition);
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
