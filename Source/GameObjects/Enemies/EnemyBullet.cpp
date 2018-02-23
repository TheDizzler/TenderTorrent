#include "../../pch.h"
#include "EnemyBullet.h"
#include "../../Engine/GameEngine.h"

EnemyBullet::EnemyBullet() : Bullet() {
	bulletSpeed = 300;
}

EnemyBullet::~EnemyBullet() {
}


void EnemyBullet::update(double deltaTime) {

	Bullet::update(deltaTime);
}
