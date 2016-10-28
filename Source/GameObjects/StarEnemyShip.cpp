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
	startPos = Vector2(getInt(startNode.attribute("x")), getInt(startNode.attribute("y")));
	endPos = Vector2(getInt(endNode.attribute("x")), getInt(endNode.attribute("y")));

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	//xml_node weaponNode = weaponPointsNode.child("weapon");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));

	}
	/*xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponNode = weaponPointsNode.child("weapon");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	const char_t* weaponTypeName = weaponNode.attribute("type").as_string();
	xml_node weaponTypeNode = weaponSystemsNode.find_child_by_attribute("weaponType", "name", weaponTypeName);
	int damage = weaponTypeNode.child("damage").text().as_int();
	int bulletSpeed = weaponTypeNode.child("bulletSpeed").text().as_int();
	const char_t* bulletName = weaponTypeNode.child("sprite").text().as_string();
	GraphicsAsset* bulletAsset = GameManager::gfxAssets->getAsset(bulletName);
	if (bulletAsset == NULL) {
		wostringstream wss;
		wss << "Unable to find weapon asset " << bulletName;
		wss << " in StarEnemyShip.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return;
	}
	for (int i = 0; i < MAX_BULLETS_IN_STORE; ++i) {
		EnemyBullet* bullet = new EnemyBullet();
		bullet->damage = damage;
		bullet->bulletSpeed = bulletSpeed;
		bullet->load(bulletAsset);
		bulletStore.push_back(bullet);
	}*/


	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();


	position = startPos;
	//weaponLocation = position;
	health = maxHealth;
}


StarEnemyShip::~StarEnemyShip() {
}


void StarEnemyShip::reset() {
	position = startPos;
	//weaponLocation = position;
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;
	//fired = false;
	//fireReady = false;
	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		weapon->fired = false;
	}
}


void StarEnemyShip::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;

	position = Vector2::Lerp(startPos, endPos, percent);

	/*if (percent >= .75 && !fired) {
		fireReady = true;
		fired = true;
	}*/

	isAlive = percent < 1;

	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
		if (percent >= .75 && !weapon->fired) {
			//fireReady = true;
			weapon->fired = true;
			Bullet* bullet = weapon->launchBullet(player->getPosition());
			liveBullets.push_back(bullet);
		}
	}
	EnemyShip::update(deltaTime);
}