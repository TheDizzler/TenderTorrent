#include "RearAttackWave.h"

RearAttackWave::RearAttackWave() {

	
}

RearAttackWave::~RearAttackWave() {
}

bool RearAttackWave::initialize(ID3D11Device* device) {

	/*sharedShipSprite = shipSprite;
	sharedBulletSprite = bulletSprite;*/

	sharedShipSprite.reset(new Sprite());
	if (!sharedShipSprite->load(device, L"assets/enemy ship A.dds")) {
		MessageBox(NULL, L"Failed to load enemy ship A", L"ERROR", MB_OK);
		return false;
	}

	sharedBulletSprite.reset(new Sprite());
	if (!sharedBulletSprite->load(device, L"assets/bullet yellow.dds")) {
		MessageBox(NULL, L"Failed to load bullet yellow", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void RearAttackWave::launchNewWave() {

	timeSinceLastLaunch = 0;


	RearAttackShip* enemy = new RearAttackShip(false);
	enemy->setHitArea(sharedShipSprite->getHitArea());
	enemyShips.push_back(enemy);
	enemy = new RearAttackShip(true);
	enemy->setHitArea(sharedShipSprite->getHitArea());
	enemyShips.push_back(enemy);

	
}
