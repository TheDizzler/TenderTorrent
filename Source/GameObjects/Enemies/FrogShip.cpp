#include "../../pch.h"
#include "FrogShip.h"
#include "../../globals.h"
#include "../../Engine/GameEngine.h"


FrogShip::FrogShip(xml_node mirrorNode) {

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
	health = maxHealth;

	position = Globals::SHIP_STORE_POSITION;


}


FrogShip::~FrogShip() {
}

void FrogShip::reset() {
	EnemyShip::reset();
	setRotation(0);
}


const double rotationSpeed = 4;

const double TIME_TO_CLIMAX = 3.0;
const double TIME_TO_FIRE = 1.5;
void FrogShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {
	
	GameObject::updateDebug(deltaTime);
	timeAlive += deltaTime;

	double percent = timeAlive / TIME_TO_CLIMAX;
	percent = 1 - cos(percent*XM_PIDIV2);
	double rt = 1 - percent;
	position = camera.screenToWorld(float(rt*rt)*startPos + 2 * float(rt*percent)*controlPoint
		+ float(percent*percent)*climaxPos);

	if (percent > .25) {
		// start aiming at player
		Vector2 dirToPlayer = player->getCenter() - position;
		dirToPlayer.Normalize();

		float angleToPlayer = atan2(dirToPlayer.y, dirToPlayer.x) + XM_PIDIV2;
		dirToPlayer = Vector2(cos(angleToPlayer), sin(angleToPlayer));
		//dirToPlayer += Vector2(cos(XM_PIDIV2), sin(XM_PIDIV2));
		Vector2 currentRot(cos(rotation), sin(rotation));

		currentRot += (dirToPlayer - currentRot) * (float) deltaTime * rotationSpeed;
		setRotation(atan2(currentRot.y, currentRot.x));


		if (timeAlive >= TIME_TO_FIRE && closeEnough(dirToPlayer, currentRot)) {
			for (auto const& weapon : weaponSystems) {
				weapon->timeSinceFired += deltaTime;
				weapon->updatePosition(position);
				if (weapon->ready()) {
					weapon->fired = true;
					Bullet* bullet = weapon->launchBullet(player->getCenter());
					liveBullets.push_back(bullet);
				}
			}
		}
	}

	if (position.y > camera.screenToWorld(Vector2(0, float(camera.viewportHeight + 120))).y) {
		reset();
	}
}



bool FrogShip::closeEnough(const Vector2& toPlayer, const Vector2& currentRot) const {

	if (toPlayer.x > currentRot.x + .15 || toPlayer.x < currentRot.x - .15)
		return false;

	if (toPlayer.y > currentRot.y + .15 || toPlayer.y < currentRot.y - .15)
		return false;

	return true;
}


