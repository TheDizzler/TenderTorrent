#include "../pch.h"
#pragma once

#include "WeaponSystem.h"

class Turret : public WeaponSystem {
public:
	Turret(Vector2 locationOffset);
	~Turret();

	virtual void loadTurretTexture(GraphicsAsset* turretAsset);

	virtual Bullet* fire();

	void setTurretRotation(const Vector2& targetDirection);
	void update(double deltaTime, Vector2 positionUpdate);
	virtual void draw(SpriteBatch* batch);

	virtual void setTint(const XMFLOAT4 newTint) override;
	virtual void setScale(const Vector2& newScale);


private:

	Vector2 turretDirection;
	unique_ptr<Sprite> turretSprite;
};