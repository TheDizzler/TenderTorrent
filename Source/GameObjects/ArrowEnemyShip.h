#include "../pch.h"
#pragma once

#include "EnemyShip.h"

/** A ship that comes in threes and has a random start location at the bottom of the screen.
		First flies straight then drifts in a new direction after firing.*/
class ArrowEnemyShip : public EnemyShip {
public:
	ArrowEnemyShip();
	ArrowEnemyShip(xml_node shipNode);
	~ArrowEnemyShip();

	virtual void update(double deltaTime, PlayerShip * player, vector<Bullet*>& liveBullets) override;

	void setStart(int xPos);
	virtual void reset() override;
private:

	double timeToTravel;
	Vector2 midPos;
	Vector2 endPos;
};