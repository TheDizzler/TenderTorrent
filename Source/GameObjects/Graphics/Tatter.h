#include "../../pch.h"
#pragma once

class Tatter : public Sprite {
public:
	Tatter(const Vector2& position);
	~Tatter();

	void update(double deltaTime);

	//void setDirection();
private:

	Vector2 direction;
	int speed;
	double timeAlive = 0;
	double timeSinceLastFlashSwitch = 0;
};