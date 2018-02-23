#pragma once

#include "../WeaponSystems/Bullet.h"


class EnemyBullet : public Bullet {
public:
	EnemyBullet();
	virtual ~EnemyBullet();

	virtual void update(double deltaTime);

private:


};