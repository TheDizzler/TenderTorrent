#pragma once

#include "EnemyShip.h"


/** A ship that appears from the bottom of the screen and leaps in
	parabolic curve. Always comes in pairs. */
class RearAttackShip : public EnemyShip {
public:

	RearAttackShip();
	RearAttackShip(bool rightSide);
	~RearAttackShip();

	virtual void update(double deltaTime, PlayerShip* player);



private:

	bool rightSide;
	bool onScreen = false;
	double onScreenTime = 0;
	int timesOnScreen = 0;
	bool returned = false;

	const Vector2 startPosLeftSide = Vector2(120, Globals::WINDOW_HEIGHT + 50);
	const Vector2 startPosRightSide = Vector2(Globals::WINDOW_WIDTH - 120, Globals::WINDOW_HEIGHT + 50);

	double timeAlive = 0;

	

	//void checkReturned();
};