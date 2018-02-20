#pragma once

#include "../PlayerShip.h"
#include "EnemyBullet.h"




class EnemyShip : public GameObject {
protected:
	class EnemyWeaponSystem {
	public:
		EnemyWeaponSystem(xml_node weaponPointNode, xml_node weaponSystemsNode, bool mirrored = false);
		virtual ~EnemyWeaponSystem();

		void reset(const Vector2& shipPosition);

		Vector2 systemLocation;
		vector<unique_ptr<EnemyBullet>> bulletStore;
		size_t nextBulletInStore = 0;
		size_t MAX_BULLETS_IN_STORE = 3;

		bool fired = false;
		double timeSinceFired = 0;
		float fireDelay = 0;
		//int timesFired = 0;

		virtual void updatePosition(const Vector2& shipPosition);
		/** Launches projectile at target. */
		EnemyBullet* launchBullet(const Vector2& target);
		/** Luanches projectile at set angle. */
		EnemyBullet* launchBullet();

	protected:
		/* Weapon system position relative to ship's origin. */
		Vector2 positionOffset;
	};
public:
	EnemyShip();
	virtual ~EnemyShip();

	virtual void setExplosion(Animation* explosion);

	virtual void reset();
	virtual void launch();
	


	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) = 0;
	virtual void explodeUpdate(double deltaTime);

	virtual void drawExplosion(SpriteBatch* batch);

	int getHealth();
	void takeDamage(int damageTaken);

	/** Points awarded for killing. */
	int points = 100;

	vector<unique_ptr<EnemyWeaponSystem>> weaponSystems;

protected:

	unique_ptr<AnimatedSprite> explosion;

	int maxHealth = 10;
	int health = 10;

	/* Start position in SCREEN coordinates, NOT world. */
	Vector2 startPos;

	double timeAlive = 0;

};