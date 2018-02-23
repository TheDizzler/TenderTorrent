#pragma once

#include "Mount.h"
#include "SunBullet.h"

class Turret : public Mount {
public:
	Turret(Vector2 locationOffset);
	virtual ~Turret();

	virtual void loadTurretTexture(GraphicsAsset* turretAsset);
	virtual void setWeaponStats(int damage, int energyCost, double coolDownTime) override;

	virtual bool ready() override;
	virtual Bullet* fire() override;

	void setTurretRotation(const Vector2& targetDirection);
	virtual void update(double deltaTime, Vector2 positionUpdate) override;
	virtual void draw(SpriteBatch* batch) override;

	virtual void setTint(const XMFLOAT4 newTint) override;
	virtual void setScale(const Vector2& newScale) override;


private:
	static const short MAX_BULLETS = 2;
	SunBullet bulletStore[Turret::MAX_BULLETS];

	Vector2 turretDirection;
	Sprite turretSprite;

	virtual void fillBulletStore(Animation* bulletAsset) override;
};