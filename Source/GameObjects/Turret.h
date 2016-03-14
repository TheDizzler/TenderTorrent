#pragma once

#include "WeaponSystem.h"
#include "SunBullet.h"

class Turret : public WeaponSystem {
public:
	Turret(Vector2 locationOffset);
	~Turret();

	bool loadTurretTexture(ID3D11Device* device, const wchar_t* textureFile);

	virtual Bullet* fire();

	void update(double deltaTime, Vector2 positionUpdate, const Vector2& mousePosition);
	virtual void draw(SpriteBatch* batch);


protected:
	virtual void fillBulletStore();

private:

	std::unique_ptr<Sprite> turretSprite;
};