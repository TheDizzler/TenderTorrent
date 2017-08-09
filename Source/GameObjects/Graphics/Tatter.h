#pragma once
#include "../../DXTKGui/BaseGraphics/Sprite.h"

class Tatter : public Sprite {
public:
	Tatter(const Vector2& position);
	virtual ~Tatter();

	void update(double deltaTime);
private:

	Vector2 direction;
	int speed;
	double timeAlive = 0;
	double timeSinceLastFlashSwitch = 0;
};