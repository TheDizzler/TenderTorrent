#pragma once

#include <dinput.h>

#include "../GameObjects/GUIObjects/MouseController.h"
#include "LaserSystem.h"
#include "../globals.h"

static Vector2 startPosition(Globals::WINDOW_WIDTH/2, Globals::WINDOW_HEIGHT - 75);


class PlayerShip : public Sprite {

public:

	PlayerShip(const Vector2& position);
	~PlayerShip();

	bool loadBullet(ID3D11Device* device);
	void update(double deltaTime, const BYTE keyboardState[256], MouseController* mouse);

	virtual void draw(SpriteBatch* batch);

	
	int maxEnergy = 520;
	int energy = 520;

	std::vector<Bullet*> liveBullets;
protected:

	
	int rechargeAmount = 50;
	float rechargeTickCount = 1.5f;
	double timeSinceRecharge = 0.0;

	std::vector<WeaponSystem*> weaponSlots;
	// need these for now for loading bullet textures
	WeaponSystem* leftWeaponSlot;
	WeaponSystem* rightWeaponSlot;
	WeaponSystem* centerWeaponSlot;



	bool firing = false;


	float speed = 200.0f;
	float firingSpeed = 150;

};