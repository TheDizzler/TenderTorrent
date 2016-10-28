#include "../pch.h"
#pragma once

#include "EnemyShip.h"


/** A ship that appears from the bottom of the screen and leaps in
	parabolic curve. Always comes in pairs. */
class RearAttackShip : public EnemyShip {
public:

	RearAttackShip();
	RearAttackShip(xml_node shipNode);
	~RearAttackShip();


	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) override;



private:

	

	Vector2 controlPoint;
	Vector2 climaxPos;
	Vector2 endPos;

	


};