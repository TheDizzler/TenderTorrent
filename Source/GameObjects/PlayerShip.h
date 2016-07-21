#pragma once

#include <dinput.h>

#include "../GameObjects/GUIObjects/MouseController.h"
#include "LaserSystem.h"
#include "../globals.h"
#include "Turret.h"


static Vector2 startPosition(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT + 175);


class PlayerShip : public Sprite {

public:

	PlayerShip(const Vector2& position);
	~PlayerShip();

	void clear();

	bool loadBullet(ID3D11Device* device);
	/** Update to perform before action starts.
		Return true when ship has moved into position. */
	bool startUpdate(double deltaTime, MouseController* mouse);
	void update(double deltaTime, const BYTE keyboardState[256], MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

	void takeDamage(int damageTaken);

	int startMaxEnergy = 520;
	int maxEnergy = startMaxEnergy;
	int energy = maxEnergy;

	std::vector<Bullet*> liveBullets;
protected:


	int rechargeAmount = 25;
	float rechargeTickCount = 1.5f;
	double timeSinceRecharge = 0.0;

	std::vector<WeaponSystem*> weaponSlots;
	// need these for now for loading bullet textures
	WeaponSystem* leftWeaponSlot;
	WeaponSystem* rightWeaponSlot;
	WeaponSystem* centerWeaponSlot;

	std::unique_ptr<Turret> leftTurret;
	std::unique_ptr<Turret> rightTurret;

	bool firing = false;


	float speed = 200.0f;
	float firingSpeed = 150;


private:
	bool lastStateVKLButtonDown;
};