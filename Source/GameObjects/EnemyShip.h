#pragma once

#include "Graphics/Sprite.h"
#include "PlayerShip.h"
#include "EnemyBullet.h"


class EnemyShip : public Sprite {

public:
	/** For constructing base sprites */
	EnemyShip();
	/** For constructing actual enemies seen on stage. */
	EnemyShip(const Vector2& position);
	~EnemyShip();

	virtual void update(double deltaTime, PlayerShip* player) = 0;
	virtual void update(double deltaTime);
	virtual void draw(SpriteBatch* batch, Sprite* baseShip);

	bool readyToFire();
	virtual EnemyBullet* launchBullet(Vector2 target);

	void takeDamage(int damageTaken);

	/** Points awarded for killing. */
	int points = 100;

protected:
	
	int health = 10;

	bool fireReady;
	int speed = 200;

	Vector2 weaponLocation;

	double timeSinceFired = 0;
	int timesFired = 0;
	
};