#include "RearAttackWave.h"

RearAttackWave::RearAttackWave() {


}

RearAttackWave::~RearAttackWave() {
}

bool RearAttackWave::initialize(ID3D11Device* device) {

	sharedShipSprite.reset(new Sprite());
	if (!sharedShipSprite->load(device, Assets::rearAttackShipFile)) {
		MessageBox(NULL, L"Failed to load enemy ship A", L"ERROR", MB_OK);
		return false;
	}

	sharedBulletSprite.reset(new Sprite());
	if (!sharedBulletSprite->load(device, Assets::enemyBulletA)) {
		MessageBox(NULL, L"Failed to load bullet yellow", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void RearAttackWave::launchNewWave() {

	timeSinceLastLaunch = 0;
	miniWavesLaunched = 0;

	launchNextMiniWave();


}

void RearAttackWave::update(double deltaTime, PlayerShip * player) {

	Wave::update(deltaTime, player);

	timeSinceLastMiniLaunch += deltaTime;
	if (miniWavesLaunched < miniWavesMax && timeSinceLastMiniLaunch >= timeBetweenMiniWaves) {

		launchNextMiniWave();

	}

}

#include <random>

bool RearAttackWave::checkForLaunch() {

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) 0, maxTimeBetweenLaunches);
	if (rand(rng) >= maxTimeBetweenLaunches - 2)
		return true;
	return false;
}

void RearAttackWave::launchNextMiniWave() {

	RearAttackShip* enemy = new RearAttackShip(false);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);
	enemy = new RearAttackShip(true);
	enemy->setDimensions(sharedShipSprite.get());
	enemyShips.push_back(enemy);

	++miniWavesLaunched;
	timeSinceLastMiniLaunch = 0;
}
