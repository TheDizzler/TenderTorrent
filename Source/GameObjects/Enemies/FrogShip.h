#pragma once

#include "EnemyShip.h"


/** A ship that appears from the bottom of the screen and leaps in
	parabolic curve. Always comes in pairs. */
class FrogShip : public EnemyShip {
public:

	FrogShip(xml_node shipNode);
	virtual ~FrogShip();

	virtual void reset() override;

	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) override;



private:

	bool closeEnough(const Vector2& toPlayer, const Vector2& currentRot) const;

	Vector2 controlPoint;
	Vector2 climaxPos;
	Vector2 endPos;

	


};