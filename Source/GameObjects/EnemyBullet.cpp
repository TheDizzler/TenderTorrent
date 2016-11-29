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

	position += direction*deltaTime*bulletSpeed;

	if (position.x < 0 || position.x > Globals::WINDOW_WIDTH
		|| position.y < 0 || position.y > Globals::WINDOW_HEIGHT)

		isAlive = false;


	//hitArea->position = Vector2(position.x - width / 2, position.y - height / 2);
	AnimatedSprite::update(deltaTime);
}
