#include "RearAttackShip.h"

RearAttackShip::RearAttackShip(xml_node shipNode) {


}



RearAttackShip::RearAttackShip() {
}

#include "../globals.h"
RearAttackShip::RearAttackShip(bool isRght) : EnemyShip(position) {

	rotation = XM_PI;

	//rightSide = isRght;
	if (isRght) {
		startPos = startPosRightSide;
		climaxPos = Vector2(Globals::WINDOW_WIDTH - 125, 150);
		controlPoint = Vector2(Globals::WINDOW_WIDTH, 0);
	} else {
		startPos = startPosLeftSide;
		climaxPos = Vector2(125, 150);
		controlPoint = Vector2(0, 0);
	}
	endPos = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT + 120);
	maxHealth = 6;


	position = startPos;
	weaponLocation = position;
	health = maxHealth;
}

RearAttackShip::~RearAttackShip() {
}

void RearAttackShip::reset() {
	position = startPos;
	weaponLocation = position;
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;
	fired = false;
}


double timeToClimax = 3.0;
double timeToFire = 2.5;
void RearAttackShip::update(double deltaTime, PlayerShip* player) {

	timeAlive += deltaTime;

	//if (timeAlive <= timeToClimax) {
	double percent = timeAlive / timeToClimax;
	//percent = sin(percent*XM_PIDIV2);
	//position = Vector2::Lerp(startPos, climaxPos, percent);
	percent = 1 - cos(percent*XM_PIDIV2);
	double rt = 1 - percent;
	position = rt*rt*startPos + 2 * rt*percent*controlPoint + percent*percent*climaxPos;



	//} else if (fired) {
	//	double percent = (timeAlive - timeToClimax) / timeToEnd;
	//	percent = 1 - cos(percent*XM_PIDIV2);
	//	//percent *= percent;
	//	position = Vector2::Lerp(climaxPos, endPos, percent);

	if (position.y > Globals::WINDOW_HEIGHT + 120) {
		isAlive = false;
	}

	if (!fired && timeAlive >= timeToFire) {
		fireReady = true;
		fired = true;
	}

	weaponLocation = position;

	EnemyShip::update(deltaTime);
}


