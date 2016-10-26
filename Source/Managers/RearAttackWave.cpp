#include "RearAttackWave.h"

RearAttackWave::RearAttackWave() {

	maxTimeBetweenLaunches = 15;
}

RearAttackWave::~RearAttackWave() {
}

#include "../Engine/GameEngine.h"
bool RearAttackWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {

	xml_node waveDataNode = shipNode.child("waveData");
	maxTimeBetweenLaunches = waveDataNode.attribute("maxTimeBetweenWaves").as_double();
	timeBetweenChecks = waveDataNode.attribute("timeBetweenChanceForWave").as_double();


	const char_t* shipName = shipNode.child("sprite").text().as_string();
	GraphicsAsset* ship = gfxAssets->getAsset(shipName);
	if (ship == NULL) {
		wostringstream wss;
		wss << "Unable to find ship asset " << shipName;
		wss << " in RearAttackWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}
	//sharedShipSprite.reset(new Sprite());
	//sharedShipSprite->load(ship);


	// fill ship store
	xml_node mirrorsNode = shipNode.child("mirrors");
	for (int i = 0; i < MAX_SHIPS_IN_STORE; ++i) {
		for (xml_node mirrorNode : shipNode.child("mirrors").children("mirror")) {
			RearAttackShip* enemy = new RearAttackShip(mirrorNode);
			//enemy->setDimensions(sharedShipSprite.get());
			enemy->load(ship);
			shipStore.push_back(enemy);
		}
	}


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
