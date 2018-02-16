#include "../pch.h"
#include "SunBullet.h"
#include "../globals.h"

SunBullet::SunBullet() : Bullet() {

}

SunBullet::SunBullet(const Vector2& position) : Bullet(position) {

	damage = 20;
	bulletSpeed = 350;
}

SunBullet::~SunBullet() {
}


void SunBullet::update(double deltaTime) {

	float deltaSpeed = (float) deltaTime*bulletSpeed;
	Vector2 newposition = position + direction*deltaSpeed;
	setPosition(newposition);
	/*position.x += direction.x*deltaSpeed;
	position.y += direction.y*deltaSpeed;*/

	if (position.y < 0 || position.y > Globals::WINDOW_HEIGHT
		|| position.x < 0 || position.x > Globals::WINDOW_WIDTH) {
		isAlive = false;
		position = weaponStore;
		timeAlive = 0;
	}

	AnimatedSprite::update(deltaTime);
}
