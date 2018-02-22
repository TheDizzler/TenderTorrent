#include "../pch.h"
#include "Wave.h"
#include <algorithm>
#include <random>
#include "../Engine/GameEngine.h"


Wave::~Wave() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;
	shipStore.clear();

	// bullets are held in ship's store
	liveBullets.clear();
}

void Wave::reloadGraphicsAssets() {
	for (EnemyShip* enemy : shipStore)
		enemy->reloadGraphicsAsset(&guiFactory);
}

void Wave::clear() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;

	shipStore.clear();
	liveBullets.clear();

	shipsLaunched = 0;
}

void Wave::clearEnemies() {
	for (EnemyShip* enemy : shipStore)
		if (enemy->isAlive)
			enemy->takeDamage(10000);
}

void Wave::checkHitDetection(Bullet* bullet) {
	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive) {
			if (bullet->getHitArea()->collision(enemy->getHitArea())) {
				enemy->takeDamage(bullet->damage);
				bullet->isAlive = false;
			}
			if (!enemy->isAlive)
				--shipsLaunched;
		}
	}
}


void Wave::update(double deltaTime, PlayerShip* player) {

	timeSinceLastLaunch += deltaTime;
	timeUntilNextCheck -= deltaTime;

	if (timeUntilNextCheck <= 0) {
		timeUntilNextCheck = timeBetweenChecks;
		if (checkForLaunch())
			launchNewWave();
	}

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Bullet* bullet) { return !bullet->isAlive; }), liveBullets.end());

	Vector2 cameraD = camera.getDelta();
	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive) {
			enemy->moveBy(cameraD);
			enemy->update(deltaTime, player, liveBullets);
			// check for collision with player
			if (player->getHitArea().collision(enemy->getHitArea())) {
				int enemyHP = enemy->getHealth();
				int playerHP = player->getHealth();
				enemy->takeDamage(playerHP);
				player->takeDamage(enemyHP);
			}
			if (!enemy->isAlive)
				--shipsLaunched;
		} else if (enemy->isExploding) {
			enemy->explodeUpdate(deltaTime);
		}
	}

}


void Wave::draw(SpriteBatch * batch) {

	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive)
			enemy->draw(batch);
		else if (enemy->isExploding)
			enemy->drawExplosion(batch);
	}

	for (Bullet* bullet : liveBullets)
		bullet->draw(batch);

}

void Wave::finishedUpdate(double deltaTime) {

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Bullet* bullet) { return !bullet->isAlive; }), liveBullets.end());


	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive)
			enemy->explodeUpdate(deltaTime);
	}
}


bool Wave::checkForLaunch() {

	if (shipsLaunched >= MAX_SHIPS_IN_STORE)
		return false;

	if (timeSinceLastLaunch >= maxTimeBetweenLaunches)
		return true;
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) timeSinceLastLaunch, maxTimeBetweenLaunches);
	if (rand(rng) >= maxTimeBetweenLaunches - 2)
		return true;
	return false;
}

