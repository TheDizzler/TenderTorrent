#pragma once

#include "Bullet.h"


class EnemyBullet : public Bullet {
public:
	EnemyBullet();
	EnemyBullet(Vector2& position);
	virtual ~EnemyBullet();

	virtual void update(double deltaTime);

private:


};