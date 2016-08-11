#pragma once

#include <dinput.h>

#include "../Engine/MouseController.h"
#include "../Engine/KeyboardController.h"
#include "LaserSystem.h"
#include "../globals.h"
#include "Turret.h"


static Vector2 startPosition(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT + 175);

using namespace std;

class PlayerShip : public Sprite {

public:

	PlayerShip(const Vector2& position);
	~PlayerShip();

	void clear();

	bool loadBullet(ID3D11Device* device);
	/** Update to perform before action starts.
		Return true when ship has moved into position. */
	bool startUpdate(double deltaTime, MouseController* mouse);
	void update(double deltaTime, const KeyboardController* keys, MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

	void takeDamage(int damageTaken);

	int startMaxEnergy = 520;
	int maxEnergy = startMaxEnergy;
	int energy = maxEnergy;

	vector<Bullet*> liveBullets;
protected:


	int rechargeAmount = 25;
	float rechargeTickCount = 1.5f;
	double timeSinceRecharge = 0.0;

	vector<WeaponSystem*> weaponSlots;
	// need these for now for loading bullet textures
	unique_ptr<WeaponSystem> leftWeaponSlot;
	unique_ptr<WeaponSystem> rightWeaponSlot;
	unique_ptr<WeaponSystem> centerWeaponSlot;

	unique_ptr<Turret> leftTurret;
	unique_ptr<Turret> rightTurret;

	bool firing = false;


	float speed = 200.0f;
	float firingSpeed = 150;


private:
	bool lastStateVKLButtonDown;
};