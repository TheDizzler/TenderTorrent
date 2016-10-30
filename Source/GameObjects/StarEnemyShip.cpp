#include "StarEnemyShip.h"

StarEnemyShip::StarEnemyShip() {
}

#include "../Engine/GameEngine.h"
#include "../globals.h"
using namespace Globals;
StarEnemyShip::StarEnemyShip(xml_node mirrorNode) {

	xml_node shipNode = mirrorNode.parent().parent();

	xml_node startNode = mirrorNode.child("start");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(getIntFrom(startNode.attribute("x")), getIntFrom(startNode.attribute("y")));
	endPos = Vector2(getIntFrom(endNode.attribute("x")), getIntFrom(endNode.attribute("y")));

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	bool mirrored = strcmp(mirrorNode.attribute("name").as_string(), "right side") == 0;
	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode, mirrored)));

	}


	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();


	position = startPos;
	health = maxHealth;
}


StarEnemyShip::~StarEnemyShip() {
}


void StarEnemyShip::reset() {

	position = startPos;
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;

	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		weapon->fired = false;
	}
}


void StarEnemyShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;

	position = Vector2::Lerp(startPos, endPos, percent);


	isAlive = percent < 1;

	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		if (percent >= .75 && !weapon->fired) {
			weapon->fired = true;
			Bullet* bullet = weapon->launchBullet();
			liveBullets.push_back(bullet);
		}
	}
	EnemyShip::update(deltaTime);
}