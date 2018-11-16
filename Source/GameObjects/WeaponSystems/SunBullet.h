#pragma once

#include "Bullet.h"


class SunBullet : public Bullet {
public:
	/** For constructing base bullet sprites */
	SunBullet();
	virtual ~SunBullet();


	virtual void update(double deltaTime) override;

private:


};