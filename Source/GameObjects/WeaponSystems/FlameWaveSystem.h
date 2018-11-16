#pragma once
#include "WeaponSystem.h"
#include "FlameWave.h"

class FlameWaveSystem : public WeaponSystem {
public:
	FlameWaveSystem(Vector2 position);
	virtual ~FlameWaveSystem();


	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime) override;
	virtual void fillBulletStore(Animation * bulletAsset) override;

	virtual bool ready() override;
	virtual Bullet* fire() override;

	virtual void draw(SpriteBatch * batch) override;




private:
	static const short MAX_BULLETS = 10;
	FlameWave bulletStore[FlameWaveSystem::MAX_BULLETS];

};