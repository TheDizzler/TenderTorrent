#include "RearAttackWave.h"

RearAttackWave::RearAttackWave() {

	maxTimeBetweenLaunches = 15;
}

RearAttackWave::~RearAttackWave() {
}

#include "../Engine/GameEngine.h"
bool RearAttackWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {

	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find ship asset " << shipName;
		wss << " in RearAttackWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}
	sharedShipSprite.reset(new Sprite());
	sharedShipSprite->load(ship);

	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");
	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponNode = weaponPointsNode.child("weapon");
	/*for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {*/

	const char_t* weaponTypeName = weaponNode.attribute("type").as_string();
	xml_node weaponTypeNode = weaponSystemsNode.find_child_by_attribute("weaponType", "name", weaponTypeName);
	//}

	const char_t* bulletName = weaponTypeNode.child("sprite").text().as_string();
	GraphicsAsset* bullet = gfxAssets->getAsset(bulletName);
	if (bullet == NULL) {
		wostringstream wss;
		wss << "Unable to find weapon asset " << bulletName;
		wss << " in RearAttackWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}


	sharedBulletSprite.reset(new Sprite());
	sharedBulletSprite->load(bullet);


	// fill ship store
	xml_node mirrorsNode = shipNode.child("mirrors");
	//if (!mirrorsNode) {
	//	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {


	//	}
	//} else {
	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		for (xml_node mirrorNode : shipNode.child("mirrors").children("mirror")) {
		//= shipNode.child("mirrors").child("mirror");
			//mirrorNode; mirrorNode = mirrorNode.next_sibling()) {
			RearAttackShip* enemy = new RearAttackShip(mirrorNode);
			enemy->setDimensions(sharedShipSprite.get());
			shipStore.push_back(enemy);
		}
	}
//}

	return true;
}

void RearAttackWave::launchNewWave() {

	timeSinceLastLaunch = 0;
	miniWavesLaunched = 0;

	launchNextMiniWave();
}

void RearAttackWave::update(double deltaTime, PlayerShip* player) {

	Wave::update(deltaTime, player);

	timeSinceLastMiniLaunch += deltaTime;
	if (miniWavesLaunched < miniWavesMax
		&& timeSinceLastMiniLaunch >= timeBetweenMiniWaves) {

		launchNextMiniWave();

	}
}

#include <random>
bool RearAttackWave::checkForLaunch() {

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) timeSinceLastLaunch, maxTimeBetweenLaunches);
	if (rand(rng) >= maxTimeBetweenLaunches - 2)
		return true;
	return false;
}

void RearAttackWave::launchNextMiniWave() {


	EnemyShip* next = shipStore[nextShipInStore++];
	next->reset();
	next = shipStore[nextShipInStore++];
	next->reset();

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;

	++miniWavesLaunched;
	timeSinceLastMiniLaunch = 0;
}
