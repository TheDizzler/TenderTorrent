#pragma once

#include <vector>

#include "Bullet.h"
#include "../globals.h"

class WeaponSystem {
public:

	//WeaponSystem();
	WeaponSystem(Vector2 locationOffset);
	~WeaponSystem();

	virtual bool loadBulletTexture(ID3D11Device* device, const wchar_t* textureFile);

	virtual void setWeaponStats(int energyCost, float coolDownTime);



	void update(double deltaTime, Vector2 positionUpdate);
	virtual void draw(SpriteBatch* batch);

	bool ready();
	Bullet* fire();

	int energyCost = 2;

	/* Position from center where weapon is located. */
	Vector2 locationOffset;

protected:
	std::unique_ptr<Sprite> baseBulletSprite;
	float coolDownTime = .1;

	Vector2 weaponLocation;
private:

	

	std::vector<Bullet*> bulletStore;
	int nextBullet = 0;

	float timeSinceFired = 0;


};