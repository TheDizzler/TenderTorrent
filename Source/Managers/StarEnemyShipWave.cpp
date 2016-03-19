#include "StarEnemyShipWave.h"

StarEnemyShipWave::StarEnemyShipWave() {
}

StarEnemyShipWave::~StarEnemyShipWave() {
}

bool StarEnemyShipWave::initialize(ID3D11Device * device) {
	
	sharedShipSprite.reset(new Sprite());
	if (!sharedShipSprite->load(device, Assets::starEnemyShip)) {
		MessageBox(NULL, L"Failed to load starEnemyShip", L"ERROR", MB_OK);
		return false;
	}

	sharedBulletSprite.reset(new Sprite());
	if (!sharedBulletSprite->load(device, Assets::enemyBulletA)) {
		MessageBox(NULL, L"Failed to load bullet yellow", L"ERROR", MB_OK);
		return false;
	}


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
