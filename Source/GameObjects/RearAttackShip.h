#include "../pch.h"
#pragma once

#include "EnemyShip.h"


/** A ship that appears from the bottom of the screen and leaps in
	parabolic curve. Always comes in pairs. */
class RearAttackShip : public EnemyShip {
public:

	RearAttackShip();
	RearAttackShip(xml_node shipNode);
	RearAttackShip(bool rightSide);
	~RearAttackShip();

	virtual void reset() override;

	virtual void update(double deltaTime, PlayerShip* player);



private:

	bool fired = false;
	const Vector2 startPosLeftSide = Vector2(120, Globals::WINDOW_HEIGHT + 50);
	const Vector2 startPosRightSide = Vector2(Globals::WINDOW_WIDTH - 120, Globals::WINDOW_HEIGHT + 50);

	Vector2 controlPoint;

	Vector2 startPos;
	Vector2 climaxPos;
	Vector2 endPos;

	double timeAlive = 0;


};