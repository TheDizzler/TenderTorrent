#include "RearAttackShip.h"

#include "../Engine/GameEngine.h"
#include "../globals.h"
using namespace Globals;
RearAttackShip::RearAttackShip(xml_node mirrorNode) {

	xml_node shipNode = mirrorNode.parent().parent();

	xml_node startNode = mirrorNode.child("start");
	xml_node controlNode = mirrorNode.child("controlPoint");
	xml_node climaxNode = mirrorNode.child("climax");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(getInt(startNode.attribute("x")), getInt(startNode.attribute("y")));
	controlPoint = Vector2(getInt(controlNode.attribute("x")), getInt(controlNode.attribute("y")));
	climaxPos = Vector2(getInt(climaxNode.attribute("x")), getInt(climaxNode.attribute("y")));;
	endPos = Vector2(getInt(endNode.attribute("x")), getInt(endNode.attribute("y")));


	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponNode = weaponPointsNode.child("weapon");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");

	/*for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {*/
	const char_t* weaponTypeName = weaponNode.attribute("type").as_string();
	xml_node weaponTypeNode = weaponSystemsNode.find_child_by_attribute("weaponType", "name", weaponTypeName);
	int damage = weaponTypeNode.child("damage").text().as_int();
	int bulletSpeed = weaponTypeNode.child("bulletSpeed").text().as_int();
	const char_t* bulletName = weaponTypeNode.child("sprite").text().as_string();
	GraphicsAsset* bulletAsset = GameManager::gfxAssets->getAsset(bulletName);
	if (bulletAsset == NULL) {
		wostringstream wss;
		wss << "Unable to find weapon asset " << bulletName;
		wss << " in RearAttackShip.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return;
	}
	for (int i = 0; i < MAX_BULLETS_IN_STORE; ++i) {
		EnemyBullet* bullet = new EnemyBullet();
		bullet->damage = damage;
		bullet->bulletSpeed = bulletSpeed;
		bullet->load(bulletAsset);
		bulletStore.push_back(bullet);
	}
	//}




	maxHealth = shipNode.child("health").text().as_int();


	position = startPos;
	weaponLocation = position;
	health = maxHealth;

	rotation = XM_PI;
}



RearAttackShip::RearAttackShip() {
}


RearAttackShip::~RearAttackShip() {
}

void RearAttackShip::reset() {
	position = startPos;
	weaponLocation = position;
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;
	fired = false;
}


double timeToClimax = 3.0;
double timeToFire = 2.5;
void RearAttackShip::update(double deltaTime, PlayerShip* player) {

	timeAlive += deltaTime;

	double percent = timeAlive / timeToClimax;
	//percent = sin(percent*XM_PIDIV2);
	//position = Vector2::Lerp(startPos, climaxPos, percent);
	percent = 1 - cos(percent*XM_PIDIV2);
	double rt = 1 - percent;
	position = rt*rt*startPos + 2 * rt*percent*controlPoint + percent*percent*climaxPos;



	//} else if (fired) {
	//	double percent = (timeAlive - timeToClimax) / timeToEnd;
	//	percent = 1 - cos(percent*XM_PIDIV2);
	//	//percent *= percent;
	//	position = Vector2::Lerp(climaxPos, endPos, percent);

	if (position.y > Globals::WINDOW_HEIGHT + 120) {
		isAlive = false;
	}

	if (!fired && timeAlive >= timeToFire) {
		fireReady = true;
		fired = true;
	}

	weaponLocation = position;

	EnemyShip::update(deltaTime);
}


