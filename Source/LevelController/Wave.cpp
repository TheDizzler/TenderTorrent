#include "Wave.h"

Wave::Wave() {
}

Wave::~Wave() {
	for (EnemyShip* enemy : enemyShips)
		delete enemy;
}

#include <algorithm>
#include <random>

void Wave::update(double deltaTime, PlayerShip* player) {

	timeSinceLastLaunch += deltaTime;

	if (((int) timeSinceLastLaunch) % 3 == 2) {
		mt19937 rng;
		rng.seed(random_device{}());
		uniform_int_distribution<mt19937::result_type> rand((int) 0, maxTimeBetweenLaunches);
		if (rand(rng) == 14)
			launchNewWave();
	}
	enemyShips.erase(remove_if(enemyShips.begin(), enemyShips.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), enemyShips.end());
	bullets.erase(remove_if(bullets.begin(), bullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), bullets.end());


	for (EnemyShip* enemy : enemyShips) {
		enemy->update(deltaTime, player);
		if (enemy->readyToFire()) {
			Bullet* bullet = enemy->launchBullet(player->position);
			bullet->setDimensions(sharedBulletSprite.get());
			bullets.push_back(bullet);
		}
	}

}


void Wave::draw(SpriteBatch * batch) {

	for (EnemyShip* enemy : enemyShips) {
		enemy->draw(batch, sharedShipSprite.get());
	}

	for (Bullet* bullet : bullets)
		bullet->draw(batch, sharedBulletSprite.get());

}

