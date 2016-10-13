#include "StarEnemyShipWave.h"

StarEnemyShipWave::StarEnemyShipWave() {
}

StarEnemyShipWave::~StarEnemyShipWave() {
}

bool StarEnemyShipWave::initialize(GFXAssetManager* gfxAssets) {
	
	sharedShipSprite.reset(new Sprite());
	sharedShipSprite->load(gfxAssets->getAsset("EnemyShip Star"));

	sharedBulletSprite.reset(new Sprite());
	sharedBulletSprite->load(gfxAssets->getAsset("Enemy Bullet A"));

	maxTimeBetweenLaunches = 25;
	
	return true;
}

void StarEnemyShipWave::launchNewWave() {

	timeSinceLastLaunch = 0;

	StarEnemyShip* enemy = new StarEnemyShip(false);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);
	enemy = new StarEnemyShip(true);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);
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
