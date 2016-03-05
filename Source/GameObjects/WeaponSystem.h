#pragma once

#include <vector>

#include "Bullet.h"
#include "../globals.h"

class WeaponSystem {
public:

	WeaponSystem();
	WeaponSystem(Vector2 position);
	~WeaponSystem();

	virtual bool loadWeaponTexture(ID3D11Device* device, const wchar_t* textureFile);

	virtual void setWeaponStats(int energyCost, float coolDownTime);



	void update(double deltaTime, Vector2 positionUpdate);
	void draw(SpriteBatch* batch);

	bool ready();
	Bullet* fire();

	int energyCost = 2;

protected:
	std::unique_ptr<Sprite> baseBulletSprite;
	float coolDownTime = .1;
private:

	Vector2 weaponLocation;

	std::vector<Bullet*> bulletStore;
	int nextBullet = 0;

	float timeSinceFired = 0;


};