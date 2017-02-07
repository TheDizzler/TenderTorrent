#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/Sprite.h"
#include "PlayerShip.h"
#include "EnemyBullet.h"




class EnemyShip : public Sprite {
protected:
	struct EnemyWeaponSystem {
		EnemyWeaponSystem(xml_node weaponPointNode, xml_node weaponSystemsNode, bool mirrored = false);
		~EnemyWeaponSystem();

		void reset(const Vector2& shipPosition);

		Vector2 systemLocation;
		vector<EnemyBullet*> bulletStore;
		size_t nextBulletInStore = 0;
		size_t MAX_BULLETS_IN_STORE = 3;

		bool fired = false;
		double timeSinceFired = 0;
		float fireDelay = 0;
		//int timesFired = 0;

		void updatePosition(Vector2 shipPosition);
		/** Launches projectile at target. */
		EnemyBullet* launchBullet(Vector2 target);
		/** Luanches projectile at set angle. */
		EnemyBullet* launchBullet();

	private:
		/* Weapon system position relative to ship's origin. */
		Vector2 positionOffset;
		//Vector2 direction;
	};
public:
	/** For constructing base sprites. VESTIGIAL. */
	EnemyShip();
	/** For constructing actual enemies seen on stage. */
	EnemyShip(const Vector2& position);
	~EnemyShip();
	virtual void reset();

	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) = 0;
	virtual void update(double deltaTime);

	void takeDamage(int damageTaken);

	/** Points awarded for killing. */
	int points = 100;

	vector<unique_ptr<EnemyWeaponSystem>> weaponSystems;

protected:


	int maxHealth = 10;
	int health = 10;

	Vector2 startPos;

	double timeAlive = 0;

};