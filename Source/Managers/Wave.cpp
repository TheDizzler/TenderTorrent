#include "Wave.h"

Wave::Wave() {
}

Wave::~Wave() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;
	shipStore.clear();

	// bullets are held in ship's store
	liveBullets.clear();
}

void Wave::clear() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;

	shipStore.clear();
	liveBullets.clear();
}


#include <algorithm>
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


	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive) {
			enemy->update(deltaTime, player, liveBullets);
			// check for collision with player
			if (player->getHitArea()->collision(enemy->getHitArea())) {
				int enemyHP = enemy->getHealth();
				int playerHP = player->getHealth();
				enemy->takeDamage(playerHP);
				player->takeDamage(enemyHP);
			}
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


#include <random>
bool Wave::checkForLaunch() {

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) timeSinceLastLaunch, maxTimeBetweenLaunches);
	if (rand(rng) >= maxTimeBetweenLaunches - 2)
		return true;
	return false;
}

