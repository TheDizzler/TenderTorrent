#include "../../pch.h"
#include "ArrowEnemyShip.h"
#include "../../Engine/GameEngine.h"


ArrowEnemyShip::ArrowEnemyShip(xml_node shipNode) {

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(
			unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));

	}

	midPos = Vector2(-100.0f, Globals::getFloatFrom(shipNode.child("midPoint")));
	endPos = Vector2((float) Globals::WINDOW_WIDTH / 2, -100.0);

	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();

	startPos = Vector2((float) -100, float(Globals::WINDOW_HEIGHT + 100));
	position = Globals::SHIP_STORE_POSITION;
	for (auto const& weapon : weaponSystems)
		weapon->reset(position);
	health = maxHealth;
}

ArrowEnemyShip::~ArrowEnemyShip() {
}

void ArrowEnemyShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;

	if (percent < .50)
		position = camera.screenToWorld(Vector2::Lerp(startPos, midPos, float(percent * 2)));
	else {
		position = camera.screenToWorld(Vector2::Lerp(midPos, endPos, float(percent - .50) * 2));

		if (!camera.viewContains(position)) {
			isAlive = false;
			reset();
		}

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
}




void ArrowEnemyShip::setStart(int xPos) {

	startPos.x = (float) xPos;
	position = camera.screenToWorld(startPos);
	midPos.x = startPos.x;

	for (auto const& weapon : weaponSystems)
		weapon->reset(position);
}