#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() :Bullet(SimpleMath::Vector2(0, 0)) {
}

EnemyBullet::EnemyBullet(Vector2 & position) : Bullet(position) {

	bulletSpeed = 300;
}

EnemyBullet::~EnemyBullet() {
}

#include "../globals.h"
void EnemyBullet::update(double deltaTime) {

	Vector2 newpos = position + direction*deltaTime*bulletSpeed;
	setPosition(newpos);
	if (position.x < 0 || position.x > Globals::WINDOW_WIDTH
		|| position.y < 0 || position.y > Globals::WINDOW_HEIGHT)

		isAlive = false;

	AnimatedSprite::update(deltaTime);
}
