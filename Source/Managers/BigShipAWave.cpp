#include "../pch.h"
#include "BigShipAWave.h"
#include "../Engine/GameEngine.h"
#include "../GameObjects/Enemies/BigShipA.h"


BigShipAWave::~BigShipAWave() {
}

bool BigShipAWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {

	xml_node waveDataNode = shipNode.child("waveData");
	maxTimeBetweenLaunches = waveDataNode.attribute("maxTimeBetweenWaves").as_uint();
	timeBetweenChecks = waveDataNode.attribute("timeBetweenChanceForWave").as_double();

	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find asset " << shipName;
		wss << " in BigShipAWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return false;
	}



	MAX_SHIPS_IN_STORE = 2;

	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		BigShipA* enemy = new BigShipA(shipNode);
		enemy->load(ship);
		shipStore.push_back(enemy);
	}

	return true;
}

void BigShipAWave::launchNewWave() {

	timeSinceLastLaunch = 0;
	

	EnemyShip* next = shipStore[nextShipInStore++];
	next->launch();

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;
}

void BigShipAWave::update(double deltaTime, PlayerShip* player) {

	Wave::update(deltaTime, player);
}
