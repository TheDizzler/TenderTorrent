#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() :Bullet(SimpleMath::Vector2(0, 0)) {
}

EnemyBullet::EnemyBullet(Vector2 & position) : Bullet(position) {

	bulletSpeed = 300;
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::update(double deltaTime) {

	position += direction*deltaTime*bulletSpeed;
}
