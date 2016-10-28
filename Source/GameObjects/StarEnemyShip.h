#include "../pch.h"
#pragma once

#include "EnemyShip.h"


class StarEnemyShip : public EnemyShip {
public:
	StarEnemyShip();
	StarEnemyShip(xml_node mirrorNode);
	//StarEnemyShip(bool rightSide);
	~StarEnemyShip();

	virtual void reset() override;

	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) override;

	//virtual EnemyBullet* launchBullet(Vector2 target);


private:

	//bool fired = false;
	double timeToTravel;

	Vector2 startPos;
	Vector2 endPos;

	//const Vector2 startPosLeftSide = Vector2(-50, 100);
	//const Vector2 startPosRightSide = Vector2(Globals::WINDOW_WIDTH + 50, 100);


};