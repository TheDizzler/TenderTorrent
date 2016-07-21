#include "Wave.h"

Wave::Wave() {
}

Wave::~Wave() {

	for (EnemyShip* enemy : enemyShips)
		delete enemy;

	for (Bullet* bullet : bullets)
		delete bullet;
}

void Wave::clear() {

	for (EnemyShip* enemy : enemyShips)
		delete enemy;

	for (Bullet* bullet : bullets)
		delete bullet;

	enemyShips.clear();
	bullets.clear();
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

	enemyShips.erase(remove_if(enemyShips.begin(), enemyShips.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), enemyShips.end());
	bullets.erase(remove_if(bullets.begin(), bullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), bullets.end());


	for (EnemyShip* enemy : enemyShips) {
		enemy->update(deltaTime, player);
		if (enemy->readyToFire()) {
			Bullet* bullet = enemy->launchBullet(player->getPosition());
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

