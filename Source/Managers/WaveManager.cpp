#include "WaveManager.h"

WaveManager::WaveManager() {
}

WaveManager::~WaveManager() {
}

bool WaveManager::initialize(ID3D11Device* device) {

	Wave* wave = new RearAttackWave();
	if (!wave->initialize(device))
		return false;

	wave->launchNewWave();
	waves.push_back(wave);

	/*sharedEnemyShipA.reset(new Sprite());
	if (!sharedEnemyShipA->load(device, L"assets/enemy ship A.dds")) {
		MessageBox(NULL, L"Failed to load enemy ship A", L"ERROR", MB_OK);
		return false;
	}

	yellowBullet.reset(new Sprite());
	if (!yellowBullet->load(device, L"assets/bullet yellow.dds")) {
		MessageBox(NULL, L"Failed to load bullet yellow", L"ERROR", MB_OK);
		return false;
	}*/
	/*baseEnemyShipB.reset(new EnemyShip());
	if (!baseEnemyShipB->load(device, L"assets/enemy ship B.dds")) {
	MessageBox(NULL, L"Failed to load enemy ship B", L"ERROR", MB_OK);
	return false;
	}*/


	/*RearAttackShip* enemy = new RearAttackShip(false);
	enemyShips.push_back(enemy);
	enemy = new RearAttackShip(true);
	enemyShips.push_back(enemy);*/

	return true;
}



void WaveManager::update(double deltaTime, PlayerShip* player) {

	for (Bullet* bullet : player->liveBullets) {
		bullet->update(deltaTime);
		for (Wave* wave : waves) {
			for (EnemyShip* enemy : wave->enemyShips) {
				if (bullet->getHitArea()->collision(enemy->getHitArea())) {
					enemy->isAlive = false;
					bullet->isAlive = false;
				}
			}
		}
	}


	for (Wave* wave : waves) {
		wave->update(deltaTime, player);

		for (Bullet* bullet : wave->bullets) {
			bullet->update(deltaTime);
			if (bullet->getHitArea()->collision(player->getHitArea())) {

				bullet->isAlive = false;
				player->takeDamage(bullet->damage);
			}
		}
	}


}



void WaveManager::draw(SpriteBatch * batch) {


	for (Wave* wave : waves)
		wave->draw(batch);
			/*for (EnemyShip* enemy : enemyShips) {
				enemy->draw(batch, baseEnemyShipA.get());
			}*/

	/*for (Bullet* bullet : bullets)
		bullet->draw(batch, baseEnemyShipA->baseBulletSprite.get());*/
}
