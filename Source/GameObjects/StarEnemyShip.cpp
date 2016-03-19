#include "StarEnemyShip.h"

StarEnemyShip::StarEnemyShip() {
}

StarEnemyShip::StarEnemyShip(bool isRght) {

	rightSide = isRght;
	if (rightSide)
		position = startPosRightSide;
	else
		position = startPosLeftSide;

	weaponLocation = position;

	health = 10;
}

StarEnemyShip::~StarEnemyShip() {
}

void StarEnemyShip::update(double deltaTime, PlayerShip * player) {

	if (rightSide)
		position.x -= speed*deltaTime;
	else
		position.x += speed*deltaTime;

	weaponLocation = position;
	EnemyShip::update(deltaTime);
}
