#pragma once

#include "../Managers/GFXAssetManager.h"

#include "WeaponSystems/LaserSystem.h"
#include "WeaponSystems/Turret.h"
#include "Graphics/GameObject.h"



/* In screenspace. */
//static const Vector2 PLAYER_START_POSITION(
//	(float)camera.viewportCenter.x ,
//	(float)camera.viewportHeight + 175);


class PlayerShip : public GameObject {
public:
	PlayerShip();
	virtual ~PlayerShip();

	void loadShip(xml_node shipNode);

	void reset();

	virtual void setPosition(const Vector2& newPosition) override;

	/** Update to perform before action starts.
		Return true when ship has moved into position. */
	bool startUpdate(double deltaTime);
	void update(double deltaTime);
	virtual void draw(SpriteBatch* batch) override;

	void finishedUpdate(double deltaTime);

	void deathUpdate(double deltaTime);
	void deathDraw(SpriteBatch* batch);

	bool checkHitDetection(const HitArea& object);

	int getHealth();
	void takeDamage(int damageTaken);

	int startMaxEnergy = 100;
	int maxEnergy = startMaxEnergy;
	int energy = maxEnergy;

	vector<Bullet*> liveBullets;
protected:


	int rechargeAmount = 25;
	float rechargeTickCount = 1.5f;
	double timeSinceRecharge = 0.0;

	
	vector<unique_ptr<SubHitArea>> subHitAreas;
	vector<unique_ptr<WeaponSystem>> weaponSlots;
	vector<unique_ptr<Mount>> mounts;


	bool firing = false;

	/* Normal speed of ship */
	float speed = 225.0f;
	/* Speed of ship while shooting */
	float firingSpeed = 150;

	/* direction travelling while killed, for eye candy. */
	Vector2 deathVector;
	double totalDeathTime = 0;
	double timeSinceLastExplosion = 0;

	vector<unique_ptr<AnimatedSprite> > explosions;

private:
	bool lastStateVKLButtonDown;
};