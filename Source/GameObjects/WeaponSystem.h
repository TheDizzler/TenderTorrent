#include "../pch.h"
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
	~WeaponSystem();

	virtual void loadBulletTexture(/*GraphicsAsset**/ shared_ptr<Animation> bulletAsset);

	virtual void setWeaponStats(int damage, int energyCost, float coolDownTime);


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
	//unique_ptr<Sprite> baseBulletSprite;
	float coolDownTime = .1;

	Vector2 weaponLocation;

	int maxStoreSize = 24;
	virtual void fillBulletStore(shared_ptr<Animation> bulletAsset);
	vector<Bullet*> bulletStore;

	int nextBullet = 0;
	float timeSinceFired = 0;


};