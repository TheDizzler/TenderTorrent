#include "../pch.h"
#include "RearAttackShip.h"
#include "../globals.h"

using namespace Globals;

RearAttackShip::RearAttackShip(xml_node mirrorNode) {

	xml_node shipNode = mirrorNode.parent().parent();

	xml_node startNode = mirrorNode.child("start");
	xml_node controlNode = mirrorNode.child("controlPoint");
	xml_node climaxNode = mirrorNode.child("climax");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(getIntFrom(startNode.attribute("x")), getIntFrom(startNode.attribute("y")));
	controlPoint = Vector2(getIntFrom(controlNode.attribute("x")), getIntFrom(controlNode.attribute("y")));
	climaxPos = Vector2(getIntFrom(climaxNode.attribute("x")), getIntFrom(climaxNode.attribute("y")));;
	endPos = Vector2(getIntFrom(endNode.attribute("x")), getIntFrom(endNode.attribute("y")));


	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));

	}




	maxHealth = shipNode.child("health").text().as_int();


	position = startPos;
	health = maxHealth;

	rotation = XM_PI;
}


RearAttackShip::~RearAttackShip() {
}




const double TIME_TO_CLIMAX = 3.0;
const double TIME_TO_FIRE = 2.5;
void RearAttackShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;

	double percent = timeAlive / TIME_TO_CLIMAX;
	percent = 1 - cos(percent*XM_PIDIV2);
	double rt = 1 - percent;
	position = rt*rt*startPos + 2 * rt*percent*controlPoint + percent*percent*climaxPos;


	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		if (!weapon->fired && timeAlive >= TIME_TO_FIRE) {
			weapon->fired = true;
			Bullet* bullet = weapon->launchBullet(player->getPosition());
			liveBullets.push_back(bullet);
		}
	}



	EnemyShip::update(deltaTime);

	if (position.y > Globals::WINDOW_HEIGHT + 120) {
		isAlive = false;
	}
}


