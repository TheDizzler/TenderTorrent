#include "../pch.h"
#pragma once


static const Vector2 weaponStore = Vector2(-511, -511);

class Bullet : public Sprite {
public:
	/** For constructing base bullet sprites */
	Bullet();
	/** For constructing actual bullets seen on stage. */
	Bullet(const Vector2 &position);
	~Bullet();

	virtual void update(double deltaTime);
	//void draw(SpriteBatch* batch, Sprite* baseSprite);

	/** Normalized direction vector of projectile. */
	Vector2 direction;


	int damage = 1;
	int bulletSpeed = 400;
protected:

	float timeAlive = 0;

};