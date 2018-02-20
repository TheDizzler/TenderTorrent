#pragma once

#include "EnemyShip.h"

/** A ship that comes in threes and has a random start location at the bottom of the screen.
		First flies straight then drifts in a new direction after firing.*/
class ArrowEnemyShip : public EnemyShip {
public:
	ArrowEnemyShip(xml_node shipNode);
	virtual ~ArrowEnemyShip();

	virtual void update(double deltaTime, PlayerShip * player, vector<Bullet*>& liveBullets) override;

	void setStart(int xPos);
private:

	double timeToTravel;
	/* Position in SCREEN coordinates, NOT world. */
	Vector2 midPos;
	/* Position in SCREEN coordinates, NOT world. */
	Vector2 endPos;
};