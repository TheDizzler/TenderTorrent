#pragma once

#include "EnemyShip.h"


class StarEnemyShip : public EnemyShip {
public:
	StarEnemyShip();
	StarEnemyShip(xml_node mirrorNode);
	virtual ~StarEnemyShip();

	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) override;


private:

	double timeToTravel;

	Vector2 startPos;
	Vector2 endPos;

};