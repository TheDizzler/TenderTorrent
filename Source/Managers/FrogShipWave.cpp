#include "FrogShipWave.h"
#include "../Engine/GameEngine.h"



FrogShipWave::~FrogShipWave() {
}


bool FrogShipWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {

	xml_node waveDataNode = shipNode.child("waveData");
	maxTimeBetweenLaunches = waveDataNode.attribute("maxTimeBetweenWaves").as_uint();
	timeBetweenChecks = waveDataNode.attribute("timeBetweenChanceForWave").as_double();


	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find ship asset " << shipName;
		wss << " in FrogShipWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}

	MAX_SHIPS_IN_STORE = 6;
	// fill ship store
	xml_node mirrorsNode = shipNode.child("mirrors");
	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		for (xml_node mirrorNode : shipNode.child("mirrors").children("mirror")) {
			FrogShip* enemy = new FrogShip(mirrorNode);
			enemy->load(ship);
			shipStore.push_back(enemy);
		}
	}


	return true;
}


void FrogShipWave::launchNewWave() {

	timeSinceLastLaunch = 0;
	miniWavesLaunched = 0;

	launchNextMiniWave();
}


void FrogShipWave::update(double deltaTime, PlayerShip* player) {

	Wave::update(deltaTime, player);

	timeSinceLastMiniLaunch += deltaTime;
	if (miniWavesLaunched < miniWavesMax
		&& timeSinceLastMiniLaunch >= timeBetweenMiniWaves) {

		launchNextMiniWave();

	}
}


void FrogShipWave::launchNextMiniWave() {


	EnemyShip* next = shipStore[nextShipInStore++];
	while (next->isAlive) {
		if (nextShipInStore >= shipStore.size())
			nextShipInStore = 0;
		next = shipStore[nextShipInStore++];
	}
	next->launch();

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;

	++miniWavesLaunched;
	timeSinceLastMiniLaunch = 0;

	++shipsLaunched;
}
