#include "WaveManager.h"

WaveManager::WaveManager() {
}

WaveManager::~WaveManager() {

	for (Wave* wave : waves)
		delete wave;
}

bool WaveManager::initialize(ID3D11Device* device) {

	Wave* wave = new RearAttackWave();
	if (!wave->initialize(device))
		return false;
	waves.push_back(wave);

	wave = new StarEnemyShipWave();
	if (!wave->initialize(device))
		return false;
	wave->launchNewWave();
	waves.push_back(wave);


	return true;
}

void WaveManager::clear() {

	for each (Wave* wave in waves)
		wave->clear();

	waves.clear();
}



void WaveManager::update(double deltaTime, PlayerShip* player) {

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

}
