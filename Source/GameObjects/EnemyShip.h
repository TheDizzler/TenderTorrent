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

	//bool loadBullet(ID3D11Device* device, const wchar_t* textureFile);


	virtual void update(double deltaTime, PlayerShip* player) = 0;
	virtual void update(double deltaTime);
	virtual void draw(SpriteBatch* batch, Sprite* baseShip);

	bool readyToFire();
	EnemyBullet* launchBullet(Vector2 target);

	
	

	//std::unique_ptr<Bullet> baseBulletSprite;

protected:
	
	int health = 10;

	bool fireReady;
	int speed = 200;

	Vector2 weaponLocation;
	
};