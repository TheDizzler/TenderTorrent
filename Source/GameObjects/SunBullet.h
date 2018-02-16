#pragma once

#include "Bullet.h"


class SunBullet : public Bullet {
public:
	/** For constructing base bullet sprites */
	SunBullet();
	SunBullet(const Vector2& position);
	virtual ~SunBullet();


	virtual void update(double deltaTime);

private:


};