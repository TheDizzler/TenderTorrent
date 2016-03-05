#include "Wave.h"

Wave::Wave() {
}

Wave::~Wave() {
	for (EnemyShip* enemy : enemyShips)
		delete enemy;
}

#include <algorithm>

void Wave::update(double deltaTime, PlayerShip* player) {

	enemyShips.erase(remove_if(enemyShips.begin(), enemyShips.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), enemyShips.end());
	bullets.erase(remove_if(bullets.begin(), bullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), bullets.end());


	for (EnemyShip* enemy : enemyShips) {
		enemy->update(deltaTime, player);
		if (enemy->readyToFire()) {
			bullets.push_back(enemy->launchBullet(player->position));
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

