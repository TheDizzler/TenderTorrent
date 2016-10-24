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
		wss << "Unable to find asset " << shipName;
		wss << " in RearAttackWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}

	const char_t* bulletName = "Enemy Bullet A";
	GraphicsAsset* bullet = gfxAssets->getAsset(bulletName);
	if (bullet == NULL) {
		wostringstream wss;
		wss << "Unable to find asset " << bulletName;
		wss << " in RearAttackWave.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return true;
	}
	sharedShipSprite.reset(new Sprite());
	sharedShipSprite->load(ship);

	sharedBulletSprite.reset(new Sprite());
	sharedBulletSprite->load(bullet);


	// fill ship store
	for (int i = 0; i < 18; ++i) {
		for (xml_node mirrorNode = shipNode.child("mirrors").child("mirror");
			mirrorNode; mirrorNode = mirrorNode.next_sibling()) {
			RearAttackShip* enemy = new RearAttackShip(mirrorNode);
			enemy->setDimensions(sharedShipSprite.get());
			shipStore.push_back(enemy);
		}
		/*RearAttackShip* enemy = new RearAttackShip(false);
		enemy->setDimensions(sharedShipSprite.get());
		shipStore.push_back(enemy);
		enemy = new RearAttackShip(true);
		enemy->setDimensions(sharedShipSprite.get());
		shipStore.push_back(enemy);*/
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

	/*RearAttackShip* enemy = new RearAttackShip(false);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);
	enemy = new RearAttackShip(true);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);*/

	EnemyShip* next = shipStore[nextShipInStore++];
	//EnemyShip* next = shipStore.back();
	next->reset();
	//activeShips.push_back(next);
	//shipStore.pop_back();
	next = shipStore[nextShipInStore++];
	next->reset();
	//activeShips.push_back(next);

	if (nextShipInStore >= shipStore.size())
		nextShipInStore = 0;

	++miniWavesLaunched;
	timeSinceLastMiniLaunch = 0;
}
