#include "../pch.h"
#include "StarEnemyShipWave.h"
#include "../Engine/GameEngine.h"

StarEnemyShipWave::~StarEnemyShipWave() {
}


bool StarEnemyShipWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {

	xml_node waveDataNode = shipNode.child("waveData");
	maxTimeBetweenLaunches = waveDataNode.attribute("maxTimeBetweenWaves").as_uint();
	timeBetweenChecks = waveDataNode.attribute("timeBetweenChanceForWave").as_double();

	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find asset " << shipName;
		wss << " in StarEnemyShipWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return false;
	}

	MAX_SHIPS_IN_STORE = 4;

	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		for (xml_node mirrorNode : shipNode.child("mirrors").children("mirror")) {
			StarEnemyShip* enemy = new StarEnemyShip(mirrorNode);
			enemy->load(ship);
			shipStore.push_back(enemy);
		}
	}
	
	return true;
}

void StarEnemyShipWave::launchNewWave() {

	timeSinceLastLaunch = 0;

	EnemyShip* next = shipStore[nextShipInStore++];
	while (next->isAlive) {
		if (nextShipInStore >= shipStore.size())
			nextShipInStore = 0;
		next = shipStore[nextShipInStore++];
	}
	next->launch();

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;
	next = shipStore[nextShipInStore++];
	while (next->isAlive) {
		if (nextShipInStore >= shipStore.size())
			nextShipInStore = 0;
		next = shipStore[nextShipInStore++];
	}
	next->launch();

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;

	shipsLaunched += 2;
}
