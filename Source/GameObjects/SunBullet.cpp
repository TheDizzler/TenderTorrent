#include "SunBullet.h"

SunBullet::SunBullet() : Bullet() {

}

SunBullet::SunBullet(const Vector2 & position) : Bullet(position) {

	damage = 20;
	bulletSpeed = 350;
}

SunBullet::~SunBullet() {
}

void SunBullet::update(double deltaTime) {

	float deltaSpeed = deltaTime*bulletSpeed;
	position.x += direction.x*deltaSpeed;
	position.y += direction.y*deltaSpeed;

	if (position.y < 0 || position.y > Globals::WINDOW_HEIGHT
		|| position.x < 0 || position.x > Globals::WINDOW_WIDTH) {
		isAlive = false;
		position = weaponStore;
		timeAlive = 0;
	}

	Sprite::update(deltaTime);
}
