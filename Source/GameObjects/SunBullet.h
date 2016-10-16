#include "../pch.h"
#pragma once

#include "Bullet.h"


class SunBullet : public Bullet {
public:
	SunBullet();
	SunBullet(const Vector2& position);
	~SunBullet();


	virtual void update(double deltaTime);

private:


};