#pragma once

#include "Bullet.h"


class SunBullet : public Bullet {
public:
	SunBullet();
	SunBullet(const Vector2& position);
	virtual ~SunBullet();


	virtual void update(double deltaTime);

private:


};