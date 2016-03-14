#pragma once

#include "WeaponSystem.h"


class Turret : public WeaponSystem {
public:
	Turret(Vector2 locationOffset);
	~Turret();

	bool loadTurretTexture(ID3D11Device* device, const wchar_t* textureFile);

	void update(double deltaTime, Vector2 positionUpdate, const Vector2& mousePosition);
	virtual void draw(SpriteBatch* batch);

private:

	std::unique_ptr<Sprite> turretSprite;
};