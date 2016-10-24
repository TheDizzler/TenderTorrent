#include "StarEnemyShipWave.h"

StarEnemyShipWave::StarEnemyShipWave() {
}

StarEnemyShipWave::~StarEnemyShipWave() {
}

#include "../Engine/GameEngine.h"
bool StarEnemyShipWave::initialize(GFXAssetManager* gfxAssets, xml_node shipNode) {
	
	const char_t* shipName = "EnemyShip Star";
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

	maxTimeBetweenLaunches = 25;
	
	return true;
}

void StarEnemyShipWave::launchNewWave() {

	timeSinceLastLaunch = 0;

	/*StarEnemyShip* enemy = new StarEnemyShip(false);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);
	enemy = new StarEnemyShip(true);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);*/
}

#include <random>

bool StarEnemyShipWave::checkForLaunch() {
	
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) 0, maxTimeBetweenLaunches);
	if (rand(rng) >= maxTimeBetweenLaunches - 1)
		return true;
	return false;
}
