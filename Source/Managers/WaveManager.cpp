#include "../pch.h"
#include "WaveManager.h"
#include "../assets.h"

WaveManager::WaveManager() {
}

WaveManager::~WaveManager() {

	for (Wave* wave : waves)
		delete wave;
}


bool WaveManager::initialize(GFXAssetManager* gfxAssets) {

	xml_document xml_enemies;
	xml_enemies.load_file(Assets::enemyShipManifestFile);

	xml_node root = xml_enemies.child("root");
	xml_node shipNode = root.child("ship");

	Wave* wave = new RearAttackWave();
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(wave);

	shipNode = shipNode.next_sibling();
	wave = new StarEnemyShipWave();
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(wave);

	shipNode = shipNode.next_sibling();
	wave = new ArrowEnemyWave();
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(wave);


	return true;
}

void WaveManager::reloadGraphicsAssets() {
	for (Wave* wave : waves)
		wave->reloadGraphicsAssets();
}

void WaveManager::clear() {

	for each (Wave* wave in waves)
		wave->clear();

	waves.clear();
}

void WaveManager::clearEnemies() {
	for each (Wave* wave in waves) {
		wave->clearEnemies();
		for (Bullet* bullet : wave->liveBullets)
			bullet->isAlive = false;
	}
}



void WaveManager::update(double deltaTime, PlayerShip* player) {

	for (Wave* wave : waves) {
		wave->update(deltaTime, player);

		for (Bullet* bullet : wave->liveBullets) {
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

void WaveManager::finishedUpdate(double deltaTime) {
	for (Wave* wave : waves)
		wave->finishedUpdate(deltaTime);
}
