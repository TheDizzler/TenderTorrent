#include "../../pch.h"
#include "Tatter.h"
#include <random>

Tatter::Tatter(const Vector2& pos) : Sprite(pos) {

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_real_distribution<> rand(0, XM_2PI);

	direction = Vector2((float) cos(rand(rng)), (float) sin(rand(rng)));
	direction.Normalize();
	speed = 50;
}

Tatter::~Tatter() {
}

const double TIME_TO_START_FLASHING = 1.5;
const double TIME_BETWEEN_FLASH_SWITCH = .25;
void Tatter::update(double deltaTime) {

	timeAlive += deltaTime;
	if (timeAlive >= TIME_TO_START_FLASHING) {
		timeSinceLastFlashSwitch += deltaTime;
		if (timeSinceLastFlashSwitch >= TIME_BETWEEN_FLASH_SWITCH) {
			timeSinceLastFlashSwitch = 0;
			if (getAlpha() == 0)
				setAlpha(1);
			else
				setAlpha(0);
		}
	}
	moveBy(direction*float(deltaTime*speed));
}
