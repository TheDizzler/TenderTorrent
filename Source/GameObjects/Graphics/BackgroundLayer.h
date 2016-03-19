#pragma once

//#include "Sprite.h"
#include "../PlayerShip.h"

class BackgroundLayer : public Sprite {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void takeDamage(int damageTaken);

	void setHitArea(const Vector2& position, const Vector2& size);
	virtual void setPosition(const Vector2& position) override;
private:

	int health = 20;

};