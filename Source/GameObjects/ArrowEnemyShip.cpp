#include "../pch.h"
#include "ArrowEnemyShip.h"

ArrowEnemyShip::ArrowEnemyShip() {
}

ArrowEnemyShip::ArrowEnemyShip(xml_node shipNode) {

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(
			unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));

	}

	midPos = Vector2(-100, Globals::getIntFrom(shipNode.child("midPoint")));
	endPos = Vector2(Globals::WINDOW_WIDTH /2, -100);

	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();

	startPos = Vector2(-100, Globals::WINDOW_HEIGHT + 100);
	position = startPos;
	health = maxHealth;
}

ArrowEnemyShip::~ArrowEnemyShip() {
}

void ArrowEnemyShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;

	if (percent < .50)
		position = Vector2::Lerp(startPos, midPos, percent * 2);
	else {
		position = Vector2::Lerp(midPos, endPos, (percent - .50) * 2);

		for (auto const& weapon : weaponSystems) {
			weapon->updatePosition(position);
			weapon->timeSinceFired += deltaTime;
			if (weapon->timeSinceFired >= weapon->fireDelay) {
				weapon->timeSinceFired = 0;
				weapon->fired = true;
				Bullet* bullet = weapon->launchBullet(player->getPosition());
				liveBullets.push_back(bullet);
			}
		}
	}
	EnemyShip::update(deltaTime);
}




void ArrowEnemyShip::setStart(int xPos) {

	startPos.x = xPos;
	position = startPos;
	midPos.x = startPos.x;

	for (auto const& weapon : weaponSystems)
		weapon->reset(position);
}