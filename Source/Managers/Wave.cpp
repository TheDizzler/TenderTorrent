#include "Wave.h"

Wave::Wave() {
}

Wave::~Wave() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;
	//activeShips.clear();
	shipStore.clear();
	for (Bullet* bullet : bullets)
		delete bullet;
	bullets.clear();
}

void Wave::clear() {

	for (EnemyShip* enemy : shipStore)
		delete enemy;

	for (Bullet* bullet : bullets)
		delete bullet;

	//activeShips.clear();
	shipStore.clear();
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

	//activeShips.erase(remove_if(activeShips.begin(), activeShips.end(),
		//[](const Sprite* sprite) { return !sprite->isAlive; }), activeShips.end());
	bullets.erase(remove_if(bullets.begin(), bullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), bullets.end());


	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive) {
		//EnemyShip* dead
		//shipStore.push_back(dead);

		//} else {
			enemy->update(deltaTime, player);
			if (enemy->readyToFire()) {
				Bullet* bullet = enemy->launchBullet(player->getPosition());
				bullet->setDimensions(sharedBulletSprite.get());
				bullets.push_back(bullet);
			}
		}
	}

}


void Wave::draw(SpriteBatch * batch) {

	for (EnemyShip* enemy : shipStore) {
		if (enemy->isAlive)
			enemy->draw(batch, sharedShipSprite.get());
	}

	for (Bullet* bullet : bullets)
		bullet->draw(batch, sharedBulletSprite.get());

}

