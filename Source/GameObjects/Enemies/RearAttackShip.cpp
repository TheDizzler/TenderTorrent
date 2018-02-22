#include "../../pch.h"
#include "RearAttackShip.h"
#include "../../globals.h"
#include "../../Engine/GameEngine.h"


RearAttackShip::RearAttackShip(xml_node mirrorNode) {

	xml_node shipNode = mirrorNode.parent().parent();

	xml_node startNode = mirrorNode.child("start");
	xml_node controlNode = mirrorNode.child("controlPoint");
	xml_node climaxNode = mirrorNode.child("climax");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(
		getFloatFrom(startNode.attribute("x")) + camera.viewportPosition.x,
		getFloatFrom(startNode.attribute("y")));
	controlPoint = Vector2(
		getFloatFrom(controlNode.attribute("x")) + camera.viewportPosition.x,
		getFloatFrom(controlNode.attribute("y")));
	climaxPos = Vector2(
		getFloatFrom(climaxNode.attribute("x")) + camera.viewportPosition.x,
		getFloatFrom(climaxNode.attribute("y")));;
	endPos = Vector2(
		getFloatFrom(endNode.attribute("x")) + camera.viewportPosition.x,
		getFloatFrom(endNode.attribute("y")));


	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(
			new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));

	}




	maxHealth = shipNode.child("health").text().as_int();


	//position = startPos;
	position = Globals::SHIP_STORE_POSITION;
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
	position = camera.screenToWorld(float(rt*rt)*startPos + 2 * float(rt*percent)*controlPoint
		+ float(percent*percent)*climaxPos);


	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		if (!weapon->fired && timeAlive >= TIME_TO_FIRE) {
			weapon->fired = true;
			Bullet* bullet = weapon->launchBullet(player->getPosition());
			liveBullets.push_back(bullet);
		}
	}

	if (position.y > camera.screenToWorld(Vector2(0, float(camera.viewportHeight + 120))).y) {
		reset();
	}
}


