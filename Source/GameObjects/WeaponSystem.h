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

	virtual void loadBulletTexture(Animation* bulletAsset);

	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime);


	void update(double deltaTime, Vector2 positionUpdate);
	virtual void draw(SpriteBatch* batch);

	bool ready();
	virtual Bullet* fire();

	const Vector2& getPosition() const;

	/* Does nothing. */
	virtual void setTint(const XMFLOAT4 newTint);

	int energyCost = 2;

	/* Position from center where weapon is located. */
	Vector2 locationOffset;

protected:
	double coolDownTime = .1;

	Vector2 weaponLocation;

	int maxStoreSize = 24;
	virtual void fillBulletStore(Animation* bulletAsset);
	vector<Bullet*> bulletStore;

	int nextBullet = 0;
	double timeSinceFired = 0;


};