#pragma once

#include "EnemyShip.h"


class StarEnemyShip : public EnemyShip {
public:
	StarEnemyShip();
	StarEnemyShip(bool rightSide);
	~StarEnemyShip();

	virtual void update(double deltaTime, PlayerShip* player);

	//virtual EnemyBullet* launchBullet(Vector2 target);


private:

	bool rightSide;
	double onScreenTime = 0;

	const Vector2 startPosLeftSide = Vector2(-50, 100);
	const Vector2 startPosRightSide = Vector2(Globals::WINDOW_WIDTH + 50, 100);

};