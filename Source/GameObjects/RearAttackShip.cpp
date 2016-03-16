#include "RearAttackShip.h"

RearAttackShip::RearAttackShip() {
}

RearAttackShip::RearAttackShip(bool isRght) : EnemyShip(position) {

	rotation = XM_PI;

	rightSide = isRght;
	if (rightSide)
		position = startPosRightSide;
	else
		position = startPosLeftSide;

	weaponLocation = position;
}

RearAttackShip::~RearAttackShip() {
}

void RearAttackShip::update(double deltaTime, PlayerShip* player) {

	timeAlive += deltaTime / 2;

	if (rightSide) {
		position.x -= sin(timeAlive) / 14;
		position.y -= cos(-timeAlive) / 5;
	} else {
		position.x += sin(timeAlive) / 14;
		position.y -= cos(-timeAlive) / 5;
	}

	if (position.y > Globals::WINDOW_HEIGHT) {
		onScreen = fireReady = false; // gonna keep them alive so they keep coming back
		onScreenTime = 0;
		timesFired = 0;
		returned = false;
		if (timesOnScreen >= 4)
			isAlive = false;
	} else {
		onScreen = true;
		if (!returned) {
			returned = true;
			++timesOnScreen;
		}
	}

	if (onScreen) {
		onScreenTime += deltaTime;
		if (onScreenTime > 1.75 && timesFired <= 3) {

			timeSinceFired += deltaTime;

			if (timeSinceFired >= .5) {
				timeSinceFired = 0;
				++timesFired;
				fireReady = true;
			} else
				fireReady = false;
		} else
			fireReady = false;
	}

	weaponLocation = position;

	EnemyShip::update(deltaTime);
}



