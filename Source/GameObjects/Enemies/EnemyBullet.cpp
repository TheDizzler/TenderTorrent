#include "../../pch.h"
#include "EnemyBullet.h"
#include "../../Engine/GameEngine.h"

EnemyBullet::EnemyBullet() : Bullet() {
	bulletSpeed = 300;
}

//EnemyBullet::EnemyBullet(Vector2& position) : Bullet() {
//
//	bulletSpeed = 300;
//}

EnemyBullet::~EnemyBullet() {
}


void EnemyBullet::update(double deltaTime) {

	/*Vector2 newpos = position + direction * FLOAT(deltaTime * bulletSpeed);
	setPosition(newpos);

	if (!camera.viewContains(position)) {
		isAlive = false;
	}*/
	Bullet::update(deltaTime);
}
