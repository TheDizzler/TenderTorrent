#pragma once

#include "WeaponSystem.h"
#include "SunBullet.h"

class BasicGun : public WeaponSystem {
public:
	BasicGun(Vector2 locationOffset);
	virtual ~BasicGun();

	virtual void fillBulletStore(Animation * bulletAsset) override;
	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime) override;

	virtual void draw(SpriteBatch* batch) override;


	virtual bool ready() override;
	virtual Bullet* fire() override;



private:
	static const short MAX_BULLETS = 20;
	SunBullet bulletStore[BasicGun::MAX_BULLETS];
};


