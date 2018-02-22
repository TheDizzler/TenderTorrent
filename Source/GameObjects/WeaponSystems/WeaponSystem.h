#pragma once

#include "Bullet.h"

/*
!!!!!!!!!!!!!!
	TODO:
		Put different WeaponSystems in XML files for live editing.
!!!!!!!!!!!!!!
*/
class WeaponSystem {
public:
	WeaponSystem(Vector2 locationOffset);
	virtual ~WeaponSystem();

	void loadBulletTexture(Animation* bulletAsset);
	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime) = 0;


	virtual bool ready() = 0;
	virtual Bullet* fire() = 0;

	virtual void update(double deltaTime, Vector2 positionUpdate);
	virtual void draw(SpriteBatch* batch) = 0;


	const Vector2& getPosition() const;

	int energyCost = 2;
	/* Position from center where weapon is located. */
	Vector2 locationOffset;

	/* Does nothing. */
	virtual void setTint(const XMFLOAT4 newTint);
protected:
	double coolDownTime = .1;

	Vector2 weaponLocation;

	
	virtual void fillBulletStore(Animation* bulletAsset) = 0;
	//int maxStoreSize = 24;
	//vector<Bullet*> bulletStore;


	size_t nextBullet = 0;
	/* Counts down from coolDownTime. When it reaches 0, weapon may fire. */
	double timeSinceFired = 0;
};