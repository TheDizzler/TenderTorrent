#pragma once

#include "WeaponSystem.h"
#include "LaserBolt.h"

/** !!!!!!!!!!!!!!
	TODO:
		Put different WeaponSystems in XML files for live editing.
	!!!!!!!!!!!!!!*/
class LaserSystem : public WeaponSystem {
public:
	LaserSystem(Vector2 position);
	virtual ~LaserSystem();

	virtual void fillBulletStore(Animation* bulletAsset) override;
	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime) override;


	virtual void draw(SpriteBatch * batch) override;


	virtual bool ready() override;
	virtual Bullet* fire() override;
private:
	static const short MAX_BULLETS = 10;
	LaserBolt bulletStore[LaserSystem::MAX_BULLETS];

};