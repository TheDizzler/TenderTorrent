#pragma once

#include "..\Graphics\AnimatedSprite.h"

/** The hiding place where all young bullet sprites live
	when not performing their sacred duty. */
static const Vector2 weaponStore = Vector2(-511, -511);

class Bullet : public AnimatedSprite {
public:
	/** For constructing base bullet sprites */
	Bullet();
	virtual ~Bullet();

	void setDamage(int newDamage);

	virtual void update(double deltaTime);

	/** Normalized direction vector of projectile. */
	Vector2 direction = Vector2(0, -1);


	int damage = 5;
	int bulletSpeed = 400;
protected:

	double timeAlive = 0;
};