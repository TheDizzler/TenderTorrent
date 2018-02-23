#include "../pch.h"
#include "WaveManager.h"
#include "../assets.h"
#include "FrogShipWave.h"
#include "StarEnemyShipWave.h"
#include "ArrowEnemyWave.h"
#include "BigShipAWave.h"


WaveManager::~WaveManager() {
}


bool WaveManager::initialize(GFXAssetManager* gfxAssets) {

	xml_document xml_enemies;
	xml_enemies.load_file(Assets::enemyShipManifestFile);

	xml_node root = xml_enemies.child("root");

	xml_node shipNode = root.child("ship");
	unique_ptr<Wave> wave = make_unique<FrogShipWave>();
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(move(wave));

	shipNode = shipNode.next_sibling();
	wave.reset(new StarEnemyShipWave());
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(move(wave));

	shipNode = shipNode.next_sibling();
	wave.reset(new ArrowEnemyWave());
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(move(wave));

	shipNode = shipNode.next_sibling();
	wave.reset(new BigShipAWave());
	if (!wave->initialize(gfxAssets, shipNode))
		return false;
	waves.push_back(move(wave));

	return true;
}

void WaveManager::reloadGraphicsAssets() {
	for (const auto& wave : waves)
		wave->reloadGraphicsAssets();
}

void WaveManager::clear() {

	for (const auto& wave : waves)
		wave->clear();

	waves.clear();
}

void WaveManager::clearEnemies() {
	for (const auto& wave : waves) {
		wave->clearEnemies();
		for (Bullet* bullet : wave->liveBullets)
			bullet->isAlive = false;
	}
}



size_t WaveManager::getLiveCount() {
	size_t count = 0;
	for (const auto& wave : waves) {
		count += wave->shipsLaunched;
	}
	return count;
}
int liveBullets;
int WaveManager::getBulletCount() {
	return liveBullets;
}


void WaveManager::checkHitDetection(Bullet* bullet) {

	for (const auto& wave : waves) {
		wave->checkHitDetection(bullet);
	}

}

void WaveManager::update(double deltaTime, PlayerShip* player) {

	//liveBullets = 0;
	for (const auto& wave : waves) {
		wave->update(deltaTime, player);

		for (Bullet* bullet : wave->liveBullets) {
			//++liveBullets;
			bullet->update(deltaTime);
			/*if (bullet->getHitArea()->collision(&player->getHitArea())) {
				bullet->isAlive = false;
				player->takeDamage(bullet->damage);
			}*/
			if (player->checkHitDetection(bullet->getHitArea())) {
				bullet->isAlive = false;
				player->takeDamage(bullet->damage);
			}
		}
	}
}



void WaveManager::draw(SpriteBatch* batch) {

	for (const auto& wave : waves)
		wave->draw(batch);

}

void WaveManager::finishedUpdate(double deltaTime) {
	for (const auto& wave : waves)
		wave->finishedUpdate(deltaTime);
}
