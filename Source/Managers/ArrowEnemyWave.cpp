#include "ArrowEnemyWave.h"

ArrowEnemyWave::ArrowEnemyWave() {
}


ArrowEnemyWave::~ArrowEnemyWave() {
}

int shipWidth;
#include "../Engine/GameEngine.h"
bool ArrowEnemyWave::initialize(GFXAssetManager * gfxAssets, xml_node shipNode) {

	xml_node waveDataNode = shipNode.child("waveData");
	maxTimeBetweenLaunches = waveDataNode.attribute("maxTimeBetweenWaves").as_double();
	timeBetweenChecks = waveDataNode.attribute("timeBetweenChanceForWave").as_double();

	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find asset " << shipName;
		wss << " for ArrowEnemyWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}

	shipWidth = ship->getWidth() * 2;

	MAX_SHIPS_IN_STORE = 9;

	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		ArrowEnemyShip* enemy = new ArrowEnemyShip(shipNode);
		enemy->load(ship);
		shipStore.push_back(enemy);
	}

	return true;
}

void ArrowEnemyWave::update(double deltaTime, PlayerShip* player) {

	Wave::update(deltaTime, player);

	timeSinceLastMiniLaunch += deltaTime;
	if (miniWavesLaunched < miniWavesMax
		&& timeSinceLastMiniLaunch >= timeBetweenMiniWaves) {

		launchNextMiniWave();

	}
}

#include <random>
int startX;
void ArrowEnemyWave::launchNewWave() {

	timeSinceLastLaunch = 0;
	miniWavesLaunched = 0;

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand(shipWidth, Globals::WINDOW_WIDTH - shipWidth);
	startX = rand(rng);

	launchNextMiniWave();
}

void ArrowEnemyWave::launchNextMiniWave() {

	ArrowEnemyShip* next = (ArrowEnemyShip*) shipStore[nextShipInStore++];
	next->reset();
	next->setStart(startX);

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;

	++miniWavesLaunched;
	timeSinceLastMiniLaunch = 0;
}
