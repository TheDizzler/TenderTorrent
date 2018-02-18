#include "../pch.h"
#include "StarEnemyShip.h"
#include "../Engine/GameEngine.h"
#include "../globals.h"


StarEnemyShip::StarEnemyShip() {
}


using namespace Globals;
StarEnemyShip::StarEnemyShip(xml_node mirrorNode) {

	xml_node shipNode = mirrorNode.parent().parent();

	xml_node startNode = mirrorNode.child("start");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(
		getFloatFrom(startNode.attribute("x")), getFloatFrom(startNode.attribute("y")));
	endPos = Vector2(
		getFloatFrom(endNode.attribute("x")), getFloatFrom(endNode.attribute("y")));

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	bool mirrored = strcmp(mirrorNode.attribute("name").as_string(), "right side") == 0;
	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode, mirrored)));

	}


	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();


	position = Globals::SHIP_STORE_POSITION;
	health = maxHealth;
}


StarEnemyShip::~StarEnemyShip() {
}



void StarEnemyShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;

	position = camera.screenToWorld(Vector2::Lerp(startPos, endPos, (float)percent));


	isAlive = percent < 1;

	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		if (percent >= .75 && !weapon->fired) {
			weapon->fired = true;
			Bullet* bullet = weapon->launchBullet();
			liveBullets.push_back(bullet);
		}
	}
}