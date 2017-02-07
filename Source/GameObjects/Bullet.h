#include "../pch.h"
#pragma once

#include "Graphics\AnimatedSprite.h"
static const Vector2 weaponStore = Vector2(-511, -511);

class Bullet : public AnimatedSprite {
public:
	/** For constructing base bullet sprites */
	Bullet();
	/** For constructing actual bullets seen on stage. */
	Bullet(const Vector2 &position);
	~Bullet();

	virtual void update(double deltaTime);

	/** Normalized direction vector of projectile. */
	Vector2 direction = Vector2(0, -1);


	int damage = 1;
	int bulletSpeed = 400;
protected:

	float timeAlive = 0;

};