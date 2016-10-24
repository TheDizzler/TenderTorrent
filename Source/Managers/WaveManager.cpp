#include "WaveManager.h"

WaveManager::WaveManager() {
}

WaveManager::~WaveManager() {

	for (Wave* wave : waves)
		delete wave;
}

#include "../assets.h"
bool WaveManager::initialize(GFXAssetManager* gfxAssets) {

	xml_document xml_enemies;
	xml_enemies.load_file(Assets::enemyShipManifestFile);

	xml_node root = xml_enemies.child("root");
	xml_node shipNode = root.child("ship");

	Wave* wave = new RearAttackWave();
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(wave);

	wave = new StarEnemyShipWave();
	if (!wave->initialize(gfxAssets, shipNode.next_sibling()))
		return false;
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
